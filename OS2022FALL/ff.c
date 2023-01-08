#include<stdio.h>
#include <sys/mman.h>
#include <unistd.h>

struct block{
	size_t size;
	int free;
	block *prev;
	block *next;
};



void *malloc(size_t size){
	int nunits;
	if (nbytes == 0){
		return NULL;
	}
	if(nbytes == 20000){
		void *mem = mmap(NULL, 20000, PROT_READ | PROT_WRITE, MAP_ANON, -1, 0);
	}
	else{
		nunits = (nbytes+sizeof(block)-1)/sizeof(block)+1;
	}
	
}

void free(void *ptr){
	
}

int main(){
	block a;
	
}
