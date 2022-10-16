#include<iostream>
#include<vector>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
using namespace std;

int main(){
    int size;
    cout <<"Input the matrix dimension: ";
    cin>>size;
    
    vector<vector<unsigned int> > a(size);
    vector<vector<unsigned int> > b(size);
//    vector<unsigned int> c;
    int matrixC =  shmget(0, 2600000, IPC_CREAT|0660);
    unsigned int *c = (unsigned int *)shmat(matrixC, NULL, 0);
    
    for(int i = 0;i<size;i++){
        a[i].resize(size,0);
        b[i].resize(size,0);
//        c.resize(size*size,0);
    }
    
    for(unsigned int i = 0;i<size;i++){
        for(unsigned int j = 0;j<size;j++){
            a[i][j] = size*i+j;
            b[i][j] = size*i+j;
        }
    }
    struct timeval s, e;
    pid_t pid,wpid;
    for(int i = 1;i<=16;i++){
        for(int j = 0; j < size*size; j++){
    		c[j] = 0;
        }
        gettimeofday(&s, 0);
        for(int j = 0;j<i;j++){
            pid = fork();
			if(pid == 0){
				int start = (size/i)*j;
				int end = start+size/i;
				if(j==i-1){
				    end = size;
                }
			    for(int x = start;x<end;x++){
                    for(int y = 0;y<size;y++){
                        for(int k = 0;k<size;k++){
                            c[size*x+y] += a[x][k]*b[k][y];
                        }
                    }
                }
				exit(0);
			}
			else if(pid < 0){
				cout << "Wrong fork";
				exit(-1);
			}
			else{
			}
        }
        int status = 0;
		while ((wpid = wait(&status)) > 0);
		gettimeofday(&e,0);
		unsigned int sum = 0;
        int start = 0*size;
        int end = (799+1)*size;
        for(int i = start;i<end;i++){
            sum+=c[i];
        }
        float sec = e.tv_sec - s.tv_sec;
		float usec = e.tv_usec - s.tv_usec;
		cout << "Multiplying matrices using "<< i << " process"<<endl;
		cout << "Elapsed time: "<<sec + (usec/1000000.0)<<" sec, Checksum: "<<sum<<endl;
    }
    
    
    return 0;
}
