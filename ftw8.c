#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>

/* function type that is called for each filename */
typedef	int	Myfunc(const char *, const struct stat *, int);

static Myfunc	myfunc;
static int		myftw(char *, Myfunc *);
static int		dopath(Myfunc *);

static long	nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

int
main(int argc, char *argv[])
{
	int		ret;

	if (argc != 2){
		printf("usage:  ftw  <starting-pathname>");
		exit(1);
	}

	ret = myftw(argv[1], myfunc);		/* does it all */

	ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
	if (ntot == 0)
		ntot = 1;		/* avoid divide by 0; print 0 for all counts */
	printf("regular files  = %7ld, %5.2f %%\n", nreg,
	  nreg*100.0/ntot);
	printf("directories    = %7ld, %5.2f %%\n", ndir,
	  ndir*100.0/ntot);
	printf("block special  = %7ld, %5.2f %%\n", nblk,
	  nblk*100.0/ntot);
	printf("char special   = %7ld, %5.2f %%\n", nchr,
	  nchr*100.0/ntot);
	printf("FIFOs          = %7ld, %5.2f %%\n", nfifo,
	  nfifo*100.0/ntot);
	printf("symbolic links = %7ld, %5.2f %%\n", nslink,
	  nslink*100.0/ntot);
	printf("sockets        = %7ld, %5.2f %%\n", nsock,
	  nsock*100.0/ntot);
	exit(ret);
}

#ifdef  PATH_MAX
static long     pathmax = PATH_MAX;
#else
static long     pathmax = 0;
#endif

static long     posix_version = 0;
static long     xsi_version = 0;

/* If PATH_MAX is indeterminate, no guarantee this is adequate */
#define PATH_MAX_GUESS  1024

char *
path_alloc(size_t *sizep) /* also return allocated size, if nonnull */
{
        char    *ptr;
        size_t  size;

        if (posix_version == 0)
                posix_version = sysconf(_SC_VERSION);

        if (xsi_version == 0)
                xsi_version = sysconf(_SC_XOPEN_VERSION);

        if (pathmax == 0) {             /* first time through */
                errno = 0;
                if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {
                        if (errno == 0)
                                pathmax = PATH_MAX_GUESS;       /* it's indeterminate */
                        else{
                                printf("pathconf error for _PC_PATH_MAX");
				exit(2);
			}
                } else {
                        pathmax++;              /* add one since it's relative to root */
                }
        }

        /*
 *          * Before POSIX.1-2001, we aren't guaranteed that PATH_MAX includes
 *                   * the terminating null byte.  Same goes for XPG3.
 *                            */
        if ((posix_version < 200112L) && (xsi_version < 4))
                size = pathmax + 1;
        else
                size = pathmax;

        if ((ptr = malloc(size)) == NULL){
                printf("malloc error for pathname");
		exit(2);
	}

        if (sizep != NULL)
                *sizep = size;
        return(ptr);
}

/*
 * Descend through the hierarchy, starting at "pathname".
 * The caller's func() is called for every file.
 */
#define	FTW_F	1		/* file other than directory */
#define	FTW_D	2		/* directory */
#define	FTW_DNR	3		/* directory that can't be read */
#define	FTW_NS	4		/* file that we can't stat */

static char	*fullpath;		/* contains full pathname for every file */
static size_t pathlen;

static int					/* we return whatever func() returns */
myftw(char *pathname, Myfunc *func)
{
	fullpath = path_alloc(&pathlen);	/* malloc PATH_MAX+1 bytes */
										/* ({Prog pathalloc}) */
	if (pathlen <= strlen(pathname)) {
		pathlen = strlen(pathname) * 2;
		if ((fullpath = realloc(fullpath, pathlen)) == NULL){
			printf("realloc failed");
			exit(2);
		}
	}
	strcpy(fullpath, pathname);
	return(dopath(func));
}

/*
 * Descend through the hierarchy, starting at "fullpath".
 * If "fullpath" is anything other than a directory, we lstat() it,
 * call func(), and return.  For a directory, we call ourself
 * recursively for each name in the directory.
 */
static int					/* we return whatever func() returns */
dopath(Myfunc* func)
{
	struct stat		statbuf;
	struct dirent	*dirp;
	DIR				*dp;
	int				ret, n;

	if (lstat(fullpath, &statbuf) < 0)	/* stat error */
		return(func(fullpath, &statbuf, FTW_NS));
	if (S_ISDIR(statbuf.st_mode) == 0)	/* not a directory */
		return(func(fullpath, &statbuf, FTW_F));

	/*
	 * It's a directory.  First call func() for the directory,
	 * then process each filename in the directory.
	 */
	if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
		return(ret);

	n = strlen(fullpath);
	if (n + NAME_MAX + 2 > pathlen) {	/* expand path buffer */
		pathlen *= 2;
		if ((fullpath = realloc(fullpath, pathlen)) == NULL){
			printf("realloc failed");
			exit(3);
		}
	}
	fullpath[n++] = '/';
	fullpath[n] = 0;

	if ((dp = opendir(fullpath)) == NULL)	/* can't read directory */
		return(func(fullpath, &statbuf, FTW_DNR));

	while ((dirp = readdir(dp)) != NULL) {
		if (strcmp(dirp->d_name, ".") == 0  ||
		    strcmp(dirp->d_name, "..") == 0)
				continue;		/* ignore dot and dot-dot */
		strcpy(&fullpath[n], dirp->d_name);	/* append name after "/" */
		if ((ret = dopath(func)) != 0)		/* recursive */
			break;	/* time to leave */
	}
	fullpath[n-1] = 0;	/* erase everything from slash onward */

	if (closedir(dp) < 0)
		printf("can't close directory %s", fullpath);
	return(ret);
}

static int
myfunc(const char *pathname, const struct stat *statptr, int type)
{
	switch (type) {
	case FTW_F:
		switch (statptr->st_mode & S_IFMT) {
		case S_IFREG:	nreg++;		break;
		case S_IFBLK:	nblk++;		break;
		case S_IFCHR:	nchr++;		break;
		case S_IFIFO:	nfifo++;	break;
		case S_IFLNK:	nslink++;	break;
		case S_IFSOCK:	nsock++;	break;
		case S_IFDIR:	/* directories should have type = FTW_D */
			printf("for S_IFDIR for %s", pathname);
		}
		break;
	case FTW_D:
		ndir++;
		break;
	case FTW_DNR:
		printf("can't read directory %s", pathname);
		break;
	case FTW_NS:
		printf("stat error for %s", pathname);
		break;
	default:
		printf("unknown type %d for pathname %s", type, pathname);
	}
	return(0);
}
