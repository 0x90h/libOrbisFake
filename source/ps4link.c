static char sccsid[] = "@(#)ps4link.c";
#include <sys/cdefs.h>
__FBSDID("$FreeBSD$"); 

#include <stdio.h>
#include <stdlib.h>
//#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include <kernel.h>
#include "dirent.h" 
#include "ps4link.h"

// ps4link_dd is now an array of structs
struct {
	char *dirname; // remember to free when closing dir
	DIR *dir;
} ps4link_dd[10] = {
  { NULL, NULL }, { NULL, NULL }, { NULL, NULL }, { NULL, NULL }, { NULL, NULL },
  { NULL, NULL }, { NULL, NULL }, { NULL, NULL }, { NULL, NULL }, { NULL, NULL }
};



int ps4LinkOpen(const char *file, int flags, int mode)
{
    int fd = -1;
    fd = open(file, flags, mode);
    return(fd);
}

int ps4LinkClose(int fd)
{
    int result = -1;
    result = close(fd);
    return(result);
}

int ps4LinkRead(int fd, void *data, size_t size)
{
    int nbytes = -1;
    nbytes = read(fd, data, size);
    return(nbytes);
}

int ps4LinkWrite(int fd, const void *data, size_t size)
{
    int writtenbytes = -1;
    writtenbytes = write(fd, data, size);
    return(writtenbytes);
}

int ps4LinkLseek(int fd, int offset, int whence)
{
    int result = -1;
    result = lseek(fd, offset, whence);
    return(result);
}

int ps4LinkRemove(const char *file)
{
    return(-1);
}

int ps4LinkMkdir(const char *dirname, int mode)
{
    return(-1);
}

int ps4LinkRmdir(const char *dirname)
{
    return(-1);
}


int ps4LinkDopen(const char *dirname)
{
    int loop0 = 0;
    int dfd = -1;
    struct stat stats;

    if(dirname[0]==0){
        return(-1);
    }

    if((stat(dirname, &stats) == 0) && (S_ISDIR(stats.st_mode))){
//        for (loop0=0; loop0<10; loop0++){
//            if (ps4link_dd[loop0].dir == NULL){
                dfd = loop0;
//                break;
//            }
//        }
    }
    //printf("dfd0 = %d\n",dfd);
    if (dfd != -1){
        ps4link_dd[dfd].dirname = (char *) malloc(strlen(dirname) + 1);
        strcpy(ps4link_dd[dfd].dirname, dirname);
        ps4link_dd[dfd].dir = opendir(dirname);
    }
    //printf("dfd dirname = %s dirp = %p \n", ps4link_dd[dfd].dirname, ps4link_dd[dfd].dir);
    return(dfd);
}

int ps4LinkDclose(int dfd)
{
    int result = -1;

    result = closedir(ps4link_dd[dfd].dir);

    if(ps4link_dd[dfd].dirname){
        free(ps4link_dd[dfd].dirname);
        ps4link_dd[dfd].dirname = NULL;
    }

    ps4link_dd[dfd].dir = NULL;
    return(result);
}

int ps4LinkDread(int dfd, OrbisDirEntry *dent)
{
    DIR *dir;
    struct dirent *dirent;
    struct tm *loctime; 
    struct stat stats;

    dir = ps4link_dd[dfd].dir;
    // Perform the request. 
    dirent = readdir(dir);

    // If no more entries were found... 
    if (dirent == NULL){
        dent->namelen = dent->type = dent->mode = dent->customtype = dent->size = 0;
        dent->name[0] = 0;
    	//dent->atime.year = dent->atime.month = dent->atime.day = dent->atime.hour = dent->atime.minute = dent->atime.second = dent->atime.microsecond = 0;
    	dent->mtime.year = dent->mtime.month = dent->mtime.day = dent->mtime.hour = dent->mtime.minute = dent->mtime.second = dent->mtime.microsecond = 0;
    	//dent->ctime.year = dent->ctime.month = dent->ctime.day = dent->ctime.hour = dent->ctime.minute = dent->ctime.second = dent->ctime.microsecond = 0;
        return(0);
    }

    stat(dirent->d_name, &stats);

    dent->namelen = dirent->d_namlen;
    dent->type = dirent->d_type;
    dent->customtype = 0;
    dent->mode = stats.st_mode;
    dent->size = stats.st_size;
    strncpy(dent->name, dirent->d_name, strlen(dirent->d_name)+1);

    loctime=(struct tm *) malloc(sizeof(struct tm));

    localtime_s(&(stats.st_mtime), loctime);
	dent->mtime.year = (loctime->tm_year+1900);
	dent->mtime.month = (loctime->tm_mon+1);
	dent->mtime.day = (loctime->tm_mday);
	dent->mtime.hour = (loctime->tm_hour);
	dent->mtime.minute = (loctime->tm_min);
	dent->mtime.second = (loctime->tm_sec);
    dent->mtime.microsecond=0;

    //memset(&stats,0,sizeof(struct stat));
    free(loctime);

    printf("name = %s namelen = %d type = %d mode = 0x%x size = %d\n",dent->name, dent->namelen, dent->type, dent->mode, dent->size);
    printf("%d %d %d %d %d %d\n", dent->mtime.year, dent->mtime.month, dent->mtime.day, dent->mtime.hour, dent->mtime.minute, dent->mtime.second);
    return(1);
}
