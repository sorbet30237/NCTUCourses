#include<iostream>
#include<vector>
#include<queue>
#include<map> 
#include<string>
#include<pthread.h>
#include<semaphore.h>
using namespace std;

vector<int> vec;
vector<range> job;
map<int,string> m;
sem_t s1, s2, t;



void* work(void *){
	 ZZZZZzzz
	while(true){
		
		sem_wait(&s1);
		sem_wait(&t);
		range temp = job[job.size()-1];
		job.pop_back();
		sem_post(&t);
		int start = temp.start/(size/k);
		int end;
		if(temp.end != size-1){
			end = (temp.end+1)/(size/k);
		}
		else{
			end = k;
		}
		if(end-start == 1){
			bubblesort(vec,temp.start,temp.end);
		}
		else{
			merge(vec,start,end,k,size);
		}
		for(int i = start;i<end;i++){
			m[i] = "1";
			if(end-start == 1){
				m[i]+="1";
			}
			else if(end-start == 2){
				m[i]+="2";
			}
			else if(end-start == 4){
				m[i]+="3";
			}
			else if(end-start == 8){
				m[i]+="4";
			}
		}
		sem_post(&s2);
	}
}

void bubblesort(vector<int> &vec, int start,int end){
	for(int i = start;i<end;i++){
		for(int i = start;i<end;i++){
			if(vec[i]>vec[i+1]){
				int temp = vec[i];
				vec[i] = vec[i+1];
				vec[i+1] = temp;
			}
		}
	}
}

void merge(vector<int> &vec, int start, int end, int k, int size){
	int mid = (size/k)*((start+end)/2);
	start = (size/k)*start;
	if(end != k){
			end = (size/k)*end;
		}
		else{
			end = size;
	}
	vector<int> v1;
	vector<int> v2;
	for(int i = start;i<mid;i++){
		v1.push_back(vec[i]);
	}
	for(int i = mid;i<end;i++){
		v2.push_back(vec[i]);
	}
	int p1 = 0;
	int p2 = 0;
	int p = start;
	while(p1 != v1.size() and p2 != v2.size()){
		if(v1[p1]<v2[p2]){
			vec[p] = v1[p1];
			p1++;
			p++; 
		}
		else{
			vec[p] = v2[p2];
			p2++;
			p++;
		}
	}
	if(p1 != v1.size()){
		for(int i = p;i<end;i++){
			vec[i] = v1[p1];
			p1++;
		}
	}
	if(p2 != v2.size()){
		for(int i = p;i<end;i++){
			vec[i] = v2[p2];
			p2++;
		}
	}
}

range jobspawn(map<int,string>& m){
	for(int i = 0;i<8;i++){
		if(m[i] != "00"){
			if(m[i][1] == '1'){
				if(i%2 == 1 and m[i-1] == "11"){
					for(int j = i-1;j<i+1;j++){
						m[j] = "00";
					}
					range temp;
					temp.start = i-1;
					temp.end = i+1;
					return temp;
				}
				else if(i%2 == 0 and m[i+1] == "11"){
					for(int j = i;j<i+2;j++){
						m[j] = "00";
					}
					range temp;
					temp.start = i;
					temp.end = i+2;
					return temp;
				}
				else{
					range temp;
					temp.start = -1;
					return temp;
				}
			}
			else if(m[i][1] == '2'){
				if(i%4 == 2){
					bool done = true;
					for(int j = i-2;j<i;j++){
						if(m[j] != "12"){
							done = false;
							break;
						}
					}
					if(done == true){
						for(int j = i-2;j<i+2;j++){
							m[j] = "00";
						}
						range temp;
						temp.start = i-2;
						temp.end = i+2;
						return temp;
					}
					else{
						range temp;
						temp.start = -1;
						return temp;
					}
				}
				else if(i%4 == 0){
					bool done = true;
					for(int j = i+2;j<i+4;j++){
						if(m[j] != "12"){
							done = false;
							break;
						}
					}
					if(done == true){
						for(int j = i;j<i+4;j++){
							m[j] = "00";
						}
						range temp;
						temp.start = i;
						temp.end = i+4;
						return temp;
					}
					else{
						range temp;
						temp.start = -1;
						return temp;
					}
				}
			}
			else{
				if(i%8 == 4){
					bool done = true;
					for(int j = i-4;j<i;j++){
						if(m[j] != "13"){
							done = false;
							break;
						}
					}
					if(done == true){
						for(int j = i-4;j<i+4;j++){
							m[j] = "00";
						}
						range temp;
						temp.start = i-4;
						temp.end = i+4;
						return temp;
					}
					else{
						range temp;
						temp.start = -1;
						return temp;
					}
				}
				else if(i%8 == 0){
					bool done = true;
					for(int j = i+4;j<i+8;j++){
						if(m[j] != "13"){
							done = false;
							break;
						}
					}
					if(done == true){
						for(int j = i;j<i+8;j++){
							m[j] = "00";
						}
						range temp;
						temp.start = i;
						temp.end = i+8;
						return temp;
					}
					else{
						range temp;
						temp.start = -1;
						return temp;
					}
				}
			}
		}
	}
	range temp;
	temp.start = -1;
	return temp;

}



int main(){
	int n = 5;
	int k = 8;
	sem_init(&t, 0, 1);
	sem_init(&s1, 0, 0);
	sem_init(&s2, 0, 0);
	for(int i = 0;i<n;i++){
		pthread_t t;
	    if (pthread_create(&t, NULL, work, NULL) != 0) {
//	        cerr << "Error: pthread_create\n";
	    }
	    pthread_detach(t);
	}
	
	int size=75;
	vec.resize(size,0);
	for(int i = 0;i<size;i++){
		vec[i] = size-i;
		cout << vec[i] << " ";
	}
	cout <<endl;
	for(int i = 0;i<k;i++){
		range temp;
		temp.start = (size/k)*i;
		if(i == k-1){
			temp.end = size-1;
		}
		else{
			temp.end = (size/k)*(i+1)-1;
		}
		m[i] = "00";
		sem_wait(&t);
		job.push_back(temp);
		sem_post(&t);
		sem_post(&s1);
	}
	
	bool flag = true;
	while(true){
		 
		sem_wait(&s2);
		for(int i = 0;i<k;i++){
			if(m[i] != "14"){
				break;
			}
			else{
				if(i==k-1){
					flag = false;
				}
			}
		}
		if(flag == false){
			break;
		}
		range newjob = jobspawn(m);
		if(newjob.start == -1){
			continue;
		}
		newjob.start = newjob.start*(size/k);
		if(newjob.end != k){
			newjob.end = newjob.end*(size/k)-1;
		}
		else{
			newjob.end = size-1;
		}
		sem_wait(&t);
		job.push_back(newjob);
		sem_post(&t);
		sem_post(&s1);
		
	}

	for(int i = 0;i<size;i++){
		cout << vec[i]<<" ";
	}
	return 0;
}
