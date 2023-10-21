/*
Student No.: 109550020
Student Name: Jing-Hong Hu
Email: henryhu.cs09@nycu.edu.tw
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/

#define FUSE_USE_VERSION 30
#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <tar.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct header_posix_ustar {
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

struct my_header{
	int mode;
	int uid;
	int gid;
	int size;
	int mtime;
	int typeflag;
	// string untok_path;
	vector<string> path;
	vector<char> file;
};

map<string, my_header> headers;

void strtok_path(const char* path, vector<string> &p){
	string str = string(path);
	char* dup = strdup(str.c_str());
	if(strcmp(path, "/") != 0){
		char* pch1;
		pch1 = strtok(dup, "/");
		do{
			p.push_back(string(pch1));
		}while (pch1 = strtok(NULL, "/"));
	}
	free(dup);
}

void tar_read(){
	puts("*********** tar_read start **********");
	FILE * fp;
	fp = fopen("test.tar", "r");
	header_posix_ustar *header = (header_posix_ustar*)malloc(sizeof(header_posix_ustar));
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    rewind(fp);				
	int s;
	int cnt = 0;
	while(cnt < size){
		fread(header, 1, 512, fp);
		if(strcmp(header->name, "")==0) break;
		s = strtol(header->size, NULL, 8);
		s = s%512 ? (s/512 + 1) * 512 : s;
		cnt += 512 + s;
		
		my_header* h = new my_header;
		my_header head = *h;
		strtok_path(header->name, head.path);				
		cout << cnt << " " << header->name << endl;			
		head.mode = strtol(header->mode, NULL, 8);			
		head.uid = strtol(header->uid, NULL, 8);			
		head.gid = strtol(header->gid, NULL, 8);			
		head.size = strtol(header->size, NULL, 8);			
		head.mtime = strtol(header->mtime, NULL, 8);		
		head.typeflag = strtol(header->typeflag, NULL, 8);	
		if(s != 0){
			head.file.resize(s);
			fread(head.file.data(), 1, s, fp);					
		}
		headers[header->name] = head;
		free(h);
	}
	free(header);
	for(auto head:headers){
		for(auto p:(head.second.path)){
			printf("%s ", p.c_str());
		}
		puts("");
	}
	puts("************ tar_read done ***********");
}

bool path_check(vector<string> p1, vector<string> p2, int diff){
	if(p1.size()+diff != p2.size()) return false;
	for(int i=0; i < p1.size(); i++) 
		if(p1[i] != p2[i]) return false;
	return true;
}


int my_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
	puts("************ readdir start ***********");
	vector<string> p1;
	strtok_path(path, p1);
	printf("Path dirs: %ld\n", p1.size());
	
	for(auto head:headers){
		if(path_check(p1, head.second.path, 1)) printf("filler: %s\n", head.second.path.back().c_str());
		if(path_check(p1, head.second.path, 1)) filler(buffer, head.second.path.back().c_str(), NULL, 0);
	}
	puts("************ readdir done ************");
	return 0;
}


int my_getattr(const char *path, struct stat *st) {
	if(strcmp(path, "/") == 0){
        puts("---root---");
        fflush(stdout);
        st->st_mode = S_IFDIR | 0444;
        return 0;
	}
	// else	
	vector<string> p1;
	strtok_path(path, p1);
	printf("Path dirs: %ld\n", p1.size());
	for(auto head:headers){
		if(!path_check(p1, head.second.path, 0)) continue;
		st->st_uid = head.second.uid;
		st->st_gid = head.second.gid;
		st->st_mtime = head.second.mtime;
		st->st_size = head.second.size;
		st->st_mode = (head.second.typeflag == 5) ? S_IFDIR | head.second.mode : S_IFREG | head.second.mode;
		return 0;
	}
	
	return -ENOENT;
}

int my_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi){
	vector<string> p1;
	strtok_path(path, p1);
	cout << "Path dirs:" << p1.size();
	for(auto head:headers){
		if(!path_check(p1, head.second.path, 0)){
			continue;
		} 
		for(int t = 0; t<size; t++){
			buffer[t] = head.second.file[t + offset];
		}
		return size-1;
	}
	return size;
}

static struct fuse_operations op;

int main(int argc, char *argv[])
{
	memset(&op, 0, sizeof(op));
	op.getattr = my_getattr;
	op.readdir = my_readdir;
	op.read = my_read;
	tar_read();
	return fuse_main(argc, argv, &op, NULL);
}
