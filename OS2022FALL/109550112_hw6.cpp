#include <stdio.h>
#include <fuse.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
using namespace std;
#define FUSE_USE_VERSION 30

int readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
	printf("--> Getting The List of Files of %s\n", path);

    filler(buffer, ".", NULL, 0, FUSE_FILL_DIR_PLUS);  // Current Directory
    filler(buffer, "..", NULL, 0, FUSE_FILL_DIR_PLUS); // Parent Directory

    if (strcmp(path, "/") == 0) // If the user is trying to show the files/directories of the root directory show the following
    {
        filler(buffer, "file54", NULL, 0, FUSE_FILL_DIR_PLUS);
        filler(buffer, "file349", NULL, 0, FUSE_FILL_DIR_PLUS);
    }

    return 0;
}

int getattr(const char *path, struct stat *st){
	st->st_uid = getuid(); 
	st->st_gid = getgid(); 
	st->st_atime = time( NULL ); 
	st->st_mtime = time( NULL ); 
	
	if ( strcmp( path, "/" ) == 0 )
	{
		st->st_mode = S_IFDIR | 0755;
		st->st_nlink = 2; 
	}
	else
	{
		st->st_mode = S_IFREG | 0644;
		st->st_nlink = 1;
		st->st_size = 1024;
	}
	
	return 0;
}

int read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi){
	char file54Text[] = "Hello World From File54!";
    char file349Text[] = "Hello World From File349!";
    char *selectedText = NULL;
    if (strcmp(path, "/file54") == 0)
        selectedText = file54Text;
    else if (strcmp(path, "/file349") == 0)
        selectedText = file349Text;
    else
        return -1;

    memcpy(buffer, selectedText + offset, size);
    return strlen(selectedText) - offset;
} 

static struct fuse_operations op;

int main(int argc, char *argv[]){
	memset(&op, 0, sizeof(op));
	op.getattr = my_getattr;
	op.readdir = my_readdir;
	op.read = my_read;
	return fuse_main(argc, argv, &op, NULL);
} 
