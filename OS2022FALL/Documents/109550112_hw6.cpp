/*
Student No.: 109550112
Student Name: ªLªÃ
Email: 109550112.cs09@nycu.edu.tw
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/

#define FUSE_USE_VERSION 30
#include <fuse.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <tar.h>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

struct tar_header {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char checksum[8];
    char typeflag[1];
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char pad[12];
};

int octal_to_int(char *current_char, unsigned int size){
    unsigned int output = 0;
    while(size > 0){
        if (*current_char < '0' || *current_char > '9'){
            break;
        }
        output = output * 8 + *current_char - '0';
        current_char++;
        size--;
    }
    return output;
}


int my_getattr(const char *path, struct stat *st) {
    memset(st, 0, sizeof(struct stat));
    int ret = 0;
    char *temp_path = strdup(path);
    if(temp_path[0] == '/'){
    	temp_path++;
	} 

    int fd = open("test.tar", O_RDONLY);
    if(fd < 0) {
        free(temp_path);
        return -ENOENT;
    }

    char buf[512];
    ssize_t read_size;
    while((read_size = read(fd, buf, 512)) > 0) {
        tar_header *header = (struct tar_header *)buf;
        if(strcmp(temp_path, header->name) == 0) {
            st->st_mode = S_IFDIR | 0444;
            st->st_nlink = 1;
            st->st_size = octal_to_int(header->size,11);
            st->st_uid = octal_to_int(header->uid,11);
            st->st_gid = octal_to_int(header->gid,11);
            st->st_mtime = octal_to_int(header->mtime,11);
            break;
        }
        lseek(fd, 512 + octal_to_int(header->size,11), SEEK_CUR);
    }
    if(read_size < 0){
    	ret = -ENOENT;
	} 
    close(fd);
    free(temp_path);
    return ret;
}

int my_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    char *temp_path = strdup(path);
    if(temp_path[0] == '/'){
    	temp_path++;
	} 
    if(strcmp(temp_path, "") != 0){
    	return -ENOENT;
	} 
    filler(buffer, ".", NULL, 0);

    int fd = open("test.tar", O_RDONLY);
    if(fd < 0) {
        free(temp_path);
        return -ENOENT;
    }

    char buf[512];
    ssize_t read_size;
    while((read_size = read(fd, buffer, 512)) > 0) {
        tar_header *header = (struct tar_header *)buffer;
        filler(buffer, header->name, NULL, 0);
        lseek(fd, 512 + octal_to_int(header->size,11), SEEK_CUR);
    }
    close(fd);
    free(temp_path);
    return 0;
}

int my_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi) {
    char *temp_path = strdup(path);
    if(temp_path[0] == '/') temp_path++;

    int fd = open("test.tar", O_RDONLY);
    if(fd < 0) {
        free(temp_path);
        return -ENOENT;
    }

    char buf[512];
    ssize_t read_size;
    while((read_size = read(fd, buffer, 512)) > 0) {
        tar_header *header = (struct tar_header *)buffer;
        if(strcmp(temp_path, header->name) == 0) {
            size_t filesize = octal_to_int(header->size,11);
            if(offset >= filesize){
            	return 0;
			} 
            if(offset + size > filesize){
            	size = filesize - offset;
			} 
            lseek(fd, 512 + offset, SEEK_CUR);
            read_size = read(fd, buffer, size);
            if(read_size < 0){
            	read_size = 0;
			} 
            break;
        }
        lseek(fd, 512 + octal_to_int(header->size,11), SEEK_CUR);
    }
    close(fd);
    free(temp_path);
    return read_size;
}

static struct fuse_operations op;

int main(int argc, char *argv[])
{
	memset(&op, 0, sizeof(op));
	op.getattr = my_getattr;
	op.readdir = my_readdir;
	op.read = my_read;
	return fuse_main(argc, argv, &op, NULL);
}
