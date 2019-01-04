/*
enum
  {
    DT_UNKNOWN = 0,
# define DT_UNKNOWN	DT_UNKNOWN
    DT_FIFO = 1,
# define DT_FIFO	DT_FIFO
    DT_CHR = 2,
# define DT_CHR		DT_CHR
    DT_DIR = 4,
# define DT_DIR		DT_DIR
    DT_BLK = 6,
# define DT_BLK		DT_BLK
    DT_REG = 8,
# define DT_REG		DT_REG
    DT_LNK = 10,
# define DT_LNK		DT_LNK
    DT_SOCK = 12,
# define DT_SOCK	DT_SOCK
    DT_WHT = 14
# define DT_WHT		DT_WHT
  };
*/

/*
 * The dirent structure defines the format of directory entries returned by
 * the getdirentries(2) system call.
 *
 * A directory entry has a struct dirent at the front of it, containing its
 * inode number, the length of the entry, and the length of the name
 * contained in the entry.  These are followed by the name padded to a 4
 * byte boundary with null bytes.  All names are guaranteed null terminated.
 * The maximum length of a name in a directory is MAXNAMLEN.
 */

//struct dirent {
//	__uint32_t d_fileno;		/* file number of entry */
//	__uint16_t d_reclen;		/* length of this record */
//	__uint8_t  d_type; 		/* file type, see below */
//	__uint8_t  d_namlen;		/* length of string in d_name */
//	char	d_name[255 + 1];	/* name must be no longer than this */
//};


/* structure describing an open directory. */
typedef struct _dirdesc {
	int	dd_fd;		/* file descriptor associated with directory */
	long	dd_loc;		/* offset in current buffer */
	long	dd_size;	/* amount of data returned by getdirentries */
	char	*dd_buf;	/* data buffer */
	int	dd_len;		/* size of data buffer */
	long	dd_seek;	/* magic cookie returned by getdirentries */
	long	dd_rewind;	/* magic cookie for rewinding */
	int	dd_flags;	/* flags for readdir */
	struct pthread_mutex	*dd_lock;	/* lock */
	struct _telldir *dd_td;	/* telldir position recording */
} DIR;

extern DIR *opendir (const char *__name) __nonnull ((1)); 
extern int closedir (DIR *__dirp) __nonnull ((1));
extern struct dirent *readdir (DIR *__dirp) __nonnull ((1)); 
