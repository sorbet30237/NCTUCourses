/*
Student No.: 109550112
Student Name: ªLªÃ 
Email: t109550112.cs09@nctu.edu.tw
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>

struct block{
	size_t size;
	int free;
	struct block *prev;
	struct block *next;
};

struct linkedlist{
	struct block *head;
	struct block *tail;
	void *memory;
};

struct linkedlist l = {NULL,NULL,NULL};

void *malloc(size_t nbytes){
	
	if(l.memory == NULL){
//		write(1,"as",sizeof("as"));
		struct block *temp;
		l.memory = mmap(NULL, 20000, PROT_READ | PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
		temp = (struct block*)l.memory;
		temp->free = 1;
		temp->next = NULL;
		temp->prev = NULL;
		temp->size = 20000-32;
		l.head = temp;
		l.tail = temp;
	}

	struct block *current = l.tail;
	if(nbytes == 0){
//		write(1,"A",sizeof("A"));
		int largest = 0;
		while(current != NULL){
			if(current->size > largest && current->free == 1){
				largest = current->size;
			}
			current = current->prev;
		}
		char t[5];
	    sprintf(t,"%d\n",largest);
	    char s[] = "Max Free Chunk Size = ";
	    strcat(s,t);
	    write(1,s,sizeof(s)+sizeof(t));
	    munmap(l.memory, 20000);
		return NULL;
	}
	nbytes = 32*((nbytes+32-1)/32);

	while(1){
		if(current == NULL){
			write(1,"Size too big", sizeof("Size too big"));
			return NULL;
		}
		else if(current->size >= nbytes && current->free == 1){
			struct block *temp;
			temp = (current+(current->size+32)/32)-(32+nbytes)/32;
			if(temp == current){
				current->free = 0;
				return (void*)current+32;
			}
			temp->size = nbytes;
			if(current->next != NULL){
				temp->next = current->next;
				current->next->prev = temp;
				current->next = temp;
				temp->prev = current;
			}
			else{
				current->next = temp;
				temp->prev = current;
				temp->next = NULL;
				l.tail = temp;
			}
			temp->free = 0;
			if(temp != current){
				current->size = current->size - 32 - nbytes;
			}
			char msg[100];
			memset(msg, 0, 100);
//			sprintf(msg, "\n%d %d\n", current,current->next);
			write(1, msg, sizeof(msg));
			return (void *)temp+32; 
		}
		current = current->prev;
	}
	
}

void free(void *ptr){
	struct block *current = l.head;
	int target = ptr - (void*)current - 32;
	int position = 0;
	while(1){
		if(position == target){
			if(current->prev != NULL && current->next != NULL){
//				write(1,"C\n",sizeof("C\n"));
				if(current->prev->free == 0 && current->next->free == 0){
					current->free = 1;
				}
				else if(current->prev->free == 1 && current->next->free == 1){
					current->prev->size = current->prev->size+current->size+current->next->size+64;
					if(current->next->next!=NULL){
						current->prev->next = current->next->next;
						current->next->next->prev = current->prev;
					}
				}
				else{
					if(current->prev->free == 1 && current->next->free == 0){
						current->prev->size = current->prev->size + current->size+32;
						current->next->prev = current->prev;
						current->prev->next = current->next;
					}
					else{
						current->size = current->size + current->next->size + 32;
						current->free = 1;
						if(current->next == l.tail){
							l.tail = current;
						}
						if(current->next->next != NULL){
							current->next->next->prev = current;
							current->next = current->next->next;
						}
					}
				}
			}
			else if(current == l.head){
				
				if(current->next != NULL && current->next->free == 1){
					current->size = current->size+current->next->size+32;
					if(current->next->next!=NULL){
						current->next->next->prev = current;
						current->next = current->next->next;
					}
				}
				current->free = 1;
			}
			else if(current == l.tail){
//				write(1,"D",sizeof("D"));
				if(current->prev != NULL && current->prev->free == 1){
					current->prev->size = current->prev->size+current->size+32;
					l.tail = current->prev;
				}
				else{
					current->free = 1;
				}
			}
			break;
		}
		position = position + current->size + 32;
		current = current->next;
		
	}
}

int main(){
	
}
