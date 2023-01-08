/*
Student No.: 109550112
Student Name: ªLªÃ 
Email: t109550112.cs09@nctu.edu.tw
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/
#include<iostream>
#include<map>
#include<unordered_map>
#include<vector>
#include<fstream>
#include <iomanip>
#include <sys/time.h>

using namespace std;

struct Node{
	int pagenum = 0;
	Node* prev = NULL;
	Node* next = NULL;
};

struct Leaf{
	int pagenum = 0;
	int refcount = 0;
	int refnum = 0;
};

class heap{
	public:
		vector<Leaf> pool;
		unordered_map<int,int> lookup;
		int size;
		double hit;
		double miss;
		int maxsize;
		int reftime;
		
		heap(int volume){
			size = 0;
			hit = 0;
			miss = 0;	
			maxsize = volume;
			reftime = 0;
		}
		
		void headremove(){
			lookup.erase(pool[0].pagenum);
			pool[0].refcount = pool[size-1].refcount;
			pool[0].pagenum = pool[size-1].pagenum;
			pool[0].refnum = pool[size-1].refnum;
			lookup[pool[0].pagenum] = 0;
			pool.pop_back();
			int index = 0;
			while((2*index+1) <= size-1){
                int minindex = 0;
                if(2*index+2 <= size-1){
                    if(pool[2*index+1].refcount<pool[2*index+2].refcount){
                        minindex = 2*index+1;
                    }
                    else if(pool[2*index+1].refcount == pool[2*index+2].refcount and pool[2*index+1].refnum < pool[2*index+2].refnum){
						minindex = 2*index+1;
                    }
                    else{
                    	minindex = 2*index+2;
					}
                }
                else{
                    minindex = 2*index+1;
                }
                if(pool[index].refcount>pool[minindex].refcount){
                    int m = lookup[pool[index].pagenum];
                	lookup[pool[index].pagenum] = lookup[pool[minindex].pagenum];
                	lookup[pool[minindex].pagenum] = m;
                    Leaf temp = pool[minindex];
                    pool[minindex] = pool[index];
                    pool[index] = temp;
                    index = minindex;
                }
                else if(pool[index].refcount == pool[minindex].refcount and pool[index].refnum> pool[minindex].refnum){
                    int m = lookup[pool[index].pagenum];
                	lookup[pool[index].pagenum] = lookup[pool[minindex].pagenum];
                	lookup[pool[minindex].pagenum] = m;
                    Leaf temp = pool[minindex];
                    pool[minindex] = pool[index];
                    pool[index] = temp;
                    index = minindex;
                }
                else{
                	break;
				}
            } 
		}
		
		void insert(int num){
			reftime++;
			if(lookup.count(num) == 0){
				miss++;
				int index;
				if(size < maxsize){
					index = size;
					lookup[num] = size;
					Leaf a;
					a.refcount = 1;
					a.refnum = reftime;
					a.pagenum = num;
					pool.push_back(a);
					size++;
				}
				else{
					headremove();
					Leaf a;
					a.refcount = 1;
					a.refnum = reftime;
					a.pagenum = num;
					pool.push_back(a);
					lookup[num] = size-1;
					index = size-1;
				}
				
				while((index-2)/2>=0){
					
					if(index%2 == 1){
	                    if(pool[index].refcount<pool[(index-1)/2].refcount){
	                    	int m = index;
	                    	lookup[pool[index].pagenum] = (index-1)/2;
	                    	lookup[pool[(index-1)/2].pagenum] = m;
	                    	Leaf n = pool[index];
	                    	pool[index] = pool[(index-1)/2];
	                    	pool[(index-1)/2] = n;
	    					index = (index-1)/2;
						}
						else if(pool[index].refcount == pool[(index-1)/2].refcount and pool[index].refnum < pool[(index-1)/2].refnum){
							int m = index;
	                    	lookup[pool[index].pagenum] = (index-1)/2;
	                    	lookup[pool[(index-1)/2].pagenum] = m;
							Leaf n = pool[index];
	                    	pool[index] = pool[(index-1)/2];
	                    	pool[(index-1)/2] = n;
	                    	index = (index-1)/2;
						}
						else{
							break;
						}
	                }
	                else{          
	                    if(pool[index].refcount<pool[(index-2)/2].refcount){
	                    	int m = index;
	                    	lookup[pool[index].pagenum] = (index-1)/2;
	                    	lookup[pool[(index-2)/2].pagenum] = m;
							Leaf n = pool[index];
	                    	pool[index] = pool[(index-2)/2];
	                    	pool[(index-2)/2] = n;
	                    	index = (index-2)/2;
						}
						else if(pool[index].refcount == pool[(index-2)/2].refcount and pool[index].refnum < pool[(index-2)/2].refnum){
							int m = index;
	                    	lookup[pool[index].pagenum] = (index-2)/2;
	                    	lookup[pool[(index-2)/2].pagenum] = m;
							Leaf n = pool[index];
	                    	pool[index] = pool[(index-2)/2];
	                    	pool[(index-2)/2] = n;
	                    	index = (index-2)/2;
						}
						else{
							break;
						}
	                }
				}
			}
			else{
				hit++;
				pool[lookup[num]].refcount++;
				pool[lookup[num]].refnum = reftime;
				int index = lookup[num];
				while((2*index+1) <= size-1){
	                int minindex = 0;
	                if(2*index+2 <= size-1){
	                    if(pool[2*index+1].refcount<pool[2*index+2].refcount){
	                        minindex = 2*index+1;
	                    }
	                    else if(pool[2*index+1].refcount == pool[2*index+2].refcount and pool[2*index+1].refnum < pool[2*index+2].refnum){
							minindex = 2*index+1;
	                    }
	                    else{
	                    	minindex = 2*index+2;
						}
	                }
	                else{
	                    minindex = 2*index+1;
	                }
	                if(pool[index].refcount>pool[minindex].refcount){
	                	int m = index;
                    	lookup[pool[index].pagenum] = minindex;
                    	lookup[pool[minindex].pagenum] = m;
	                    Leaf temp = pool[minindex];
	                    pool[minindex] = pool[index];
	                    pool[index] = temp;
	                    index = minindex;
	                }
	                else if(pool[index].refcount == pool[minindex].refcount and pool[index].refnum> pool[minindex].refnum){
	                    int m = index;
                    	lookup[pool[index].pagenum] = minindex;
                    	lookup[pool[minindex].pagenum] = m;
	                    Leaf temp = pool[minindex];
	                    pool[minindex] = pool[index];
	                    pool[index] = temp;
	                    index = minindex;
	                }
	                else{
	                	break;
					}
            	}
			}
		}
};

class linkedlist{
	public:
		unordered_map<int,Node*> lookup;
		Node* head;
		Node* tail;
		int size;
		int maxsize;
		double hit;
		double miss;
		
		linkedlist(int volume){
			head = NULL;
			tail = NULL;
			size = 0;
			maxsize = volume;
			hit = 0;
			miss = 0;
		}
		
		void insert(int num){
//			cout << num <<" ";
			if(lookup.count(num) == 0){
				miss = miss+1;
				if(size == maxsize){
					Node *temp = new Node;
					temp->pagenum = num;
					temp->next = head;
					head = temp;
					temp->next->prev = temp;
					lookup[num] = temp;
					lookup.erase(tail->pagenum);
					tail = tail->prev;
					delete tail->next;
					tail->next = NULL;
				}
				else{
					Node *temp = new Node;
					temp->pagenum = num;
					if(head != NULL){
						temp->next = head;
						head = temp;
						temp->next->prev = temp;
					}
					else{
						head = temp;
						tail = temp;
					}
					size++;
					lookup[num] = temp;
				}
			}
			else{
				hit = hit+1;
				if(lookup[num] != head){
					if(lookup[num] == tail){
						tail = tail->prev;
						delete tail->next;
						tail->next = NULL;
						Node *t = new Node;
						t->pagenum = num;
						t->next = head;
						head = t;
						t->next->prev = t;
						lookup[num] = t;
					}
					else{
						lookup[num]->prev->next = lookup[num]->next;
						lookup[num]->next->prev = lookup[num]->prev;
						delete lookup[num];
						Node *temp = new Node;
						temp->pagenum = num;
						temp->next = head;
						head = temp;
						temp->next->prev = temp;
						lookup[num] = temp;
					}
				}
			}
		}
		
};

int main(int argc, char *argv[]){
	cout << "LFU policy:" << endl;
	cout << "Frame\tHit\t\tMiss\t\tPage fault ratio\n";
	struct timeval s, e;
	gettimeofday(&s, 0);
	for(int i = 64;i<=512;i=i*2){
		ifstream file;
		file.open(argv[1]);
		heap h(i);
		int a;
		while(file >> a){
			h.insert(a);
		}
		cout <<i<<"\t"<< int(h.hit) << "\t\t"<< int(h.miss) <<"\t\t" <<fixed<<setprecision(10)<<h.miss/(h.hit+h.miss)<<endl;
		file.close();
	}
	gettimeofday(&e,0);
	float sec = e.tv_sec - s.tv_sec;
	float float_sec = e.tv_usec - s.tv_usec;
	cout << "Total elapsed time "<<fixed<<setprecision(4)<<sec + (float_sec/1000000.0)<<" sec\n";
	
	cout << "\n"; 
	
	cout << "LRU policy:" << endl;
	cout << "Frame\tHit\t\tMiss\t\tPage fault ratio\n";
	gettimeofday(&s, 0);
	for(int i = 64;i<=512;i=i*2){
		ifstream file;
		file.open(argv[1]);
		linkedlist l(i);
		int a;
		while(file >> a){
			l.insert(a);
		}
		cout <<i<<"\t"<< int(l.hit) << "\t\t"<< int(l.miss) <<"\t\t" <<fixed<<setprecision(10)<<l.miss/(l.hit+l.miss)<<endl;
		file.close();
	}
	gettimeofday(&e,0);
	sec = e.tv_sec - s.tv_sec;
	float_sec = e.tv_usec - s.tv_usec;
	cout << "Total elapsed time "<<fixed<<setprecision(4)<<sec + (float_sec/1000000.0)<<" sec\n";
	return 0; 
} 
