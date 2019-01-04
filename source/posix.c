static char sccsid[] = "@(#)posix.c";
#include <sys/cdefs.h>
__FBSDID("$FreeBSD$"); 

#include <stdio.h>
#include <stdlib.h>
//#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <kernel.h>
#include "dirent.h"


DIR *
opendir (const char *name)
{
	register DIR *dirp;
	register int fd;
	int rc = 0;

	//if ((fd = sceKernelOpen(name, SCE_KERNEL_O_RDONLY|SCE_KERNEL_O_DIRECTORY, 0)) == -1)
	if ((fd = open(name, O_RDONLY | O_DIRECTORY, 0)) == -1)
		return NULL;

	//rc = sceKernelFcntl(fd, F_GETFL, 1);
	rc = fcntl(fd, F_GETFL, 1);

	if (rc == -1 ||
	    (dirp = (DIR *)malloc(sizeof(DIR))) == NULL) {
		//sceKernelClose(fd);
		close(fd);
		return NULL;
	}

	dirp->dd_buf = malloc (1024);
	dirp->dd_len = 1024;

	if (dirp->dd_buf == NULL) {
		free (dirp);
		//sceKernelClose(fd);
		close(fd);
		return NULL;
	}
	dirp->dd_fd = fd;
	dirp->dd_loc = 0;
	dirp->dd_seek = 0;

	return dirp;
}

int
closedir (register DIR *dirp)
{
	int rc;

	//rc = sceKernelClose(dirp->dd_fd);
	rc = close(dirp->dd_fd);
	free((void *)dirp->dd_buf);
	free((void *)dirp);
	return rc;
}

struct dirent *
readdir (register DIR *dirp)
{
  register struct dirent *dp;
 
  for (;;) {
    if (dirp->dd_loc == 0) {

    //dirp->dd_size = sceKernelGetdents(dirp->dd_fd,
    dirp->dd_size = getdents(dirp->dd_fd,
				dirp->dd_buf,
				dirp->dd_len);

      if (dirp->dd_size <= 0) {
	return NULL;
      }
    }
    if (dirp->dd_loc >= dirp->dd_size) {
      dirp->dd_loc = 0;
      continue;
    }
    dp = (struct dirent *)(dirp->dd_buf + dirp->dd_loc);

    if (dp->d_reclen <= 0 ||
	dp->d_reclen > dirp->dd_len + 1 - dirp->dd_loc) {
      return NULL;
    }
    dirp->dd_loc += dp->d_reclen;
    if (dp->d_fileno == 0)
      continue;
    return (dp);
  }
}

/*
unsigned int
sleep(unsigned int	seconds)
{
    unsigned int ret;
    ret = sceKernelSleep(seconds);
    return(ret);
}
*/
