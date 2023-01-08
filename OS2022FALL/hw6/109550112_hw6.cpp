#define FUSE_USE_VERSION 30
#include <iostream> 
#include <fuse.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include<vector>
#define BLOCK_SIZE 512

using namespace std;

struct tar_header {
	char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char padding[12];
};


int my_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    // Open the tar file
    int fd = open("test.tar", O_RDONLY);
    if (fd < 0) {
        return -errno;
    }

    // Read the tar file header
    struct tar_header header;
    int res = read(fd, &header, sizeof(struct tar_header));
    if (res < 0) {
        return -errno;
    }

    // Skip the file content
    off_t seek_res = lseek(fd, atoi(header.size), SEEK_CUR);
    if (seek_res < 0) {
        return -errno;
    }

    // Add the file name to the directory listing
    filler(buffer, header.name, NULL, 0);

    // Repeat until end of tar file
    while (read(fd, &header, sizeof(struct tar_header)) > 0) {
        seek_res = lseek(fd, atoi(header.size), SEEK_CUR);
        if (seek_res < 0) {
            return -errno;
        }
        filler(buffer, header.name, NULL, 0);
    }

    close(fd);
    return 0;
}

int my_getattr(const char *path, struct stat *st) {
    if(lstat(path, st) < 0){
        return -errno;
    }

    // Set the values of the additional fields
    st->st_uid = getuid();
    st->st_gid = getgid();
    st->st_mtime = time(NULL);
    st->st_size = 1024; // size in bytes
    st->st_mode = S_IFDIR | 0444; // regular file, read-write for owner, read-only for others

    return 0;
}

int my_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi){
    int fd;
    int res;

    (void) fi;
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        return -errno;
    }

    res = pread(fd, buffer, size, offset);
    if (res == -1) {
        res = -errno;
    }

    close(fd);
    return res;
}

static struct fuse_operations op;

int main(int argc, char *argv[]){
    memset(&op, 0, sizeof(op));
    op.getattr = my_getattr;
    op.readdir = my_readdir;
    op.read = my_read;
    return fuse_main(argc, argv, &op, NULL);
}
