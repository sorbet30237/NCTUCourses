#include<iostream>
#include<vector>
#include<queue>
#include<map> 
using namespace std;

struct range{
	int start;
	int end;
};

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

range jobspawn(int start, int end, map<int,int>& m){
	
	int size = end-start;
	if(start%(2*size) == 0){
		bool done = true;
		for(int i = start+size;i<start+2*size;i++){
			if(m[i] == 0){
				done = false;
				break;
			}
		}
		if(done == true){
			for(int i = start;i<start+2*size;i++){
				m[i] = 0;
			}
			range temp;
			temp.start = start;
			temp.end = end+size;
			return temp;
		}
		else{
			range temp;
			temp.start = -1;
			return temp;
		}
	}
	else if(start%(2*size) == size){
		bool done = true;
		for(int i = start-size;i<start;i++){
			if(m[i] == 0){
				done = false;
				break;
			}
		}
		if(done == true){
			for(int i = start-size;i<start+size;i++){
				m[i] = 0;
			}
			range temp;
			temp.start = start-size;
			temp.end = end;
			return temp;
		}
		else{
			range temp;
			temp.start = -1;
			return temp;
		}
	}
	else{
		range temp;
		temp.start = -1;
		return temp;
	}
//	if(end-start == 1){
//		if(start%2 == 0){
//			if(m[start+1] == 1){
//				m[start] = 0;
//				m[start+1] = 0;
//				range temp;
//				temp.start = start;
//				temp.end = end+1;
//				return temp;
//			}
//			else{
//				range temp;
//				temp.start = -1;
//				return temp;
//			}
//		}
//		else{
//			if(m[start-1] == 1){
//				m[start] = 0;
//				m[start-1] = 0;
//				range temp;
//				temp.start = start-1;
//				temp.end = end;
//				return temp;
//			}
//			else{
//				range temp;
//				temp.start = -1;
//				return temp;
//			}
//		}
//	}
//	else if(end-start == 2){
//		if(start%4 == 0){
//			if(m[start+2] == 1 and m[start+3] == 1){
//				for(int i = start;i<start+4;i++){
//					m[i] = 0;
//				}
//				range temp;
//				temp.start = start;
//				temp.end = end+2;
//				return temp;
//			}
//			else{
//				range temp;
//				temp.start = -1;
//				return temp;
//			}
//		}
//		else{
//			if(m[start-2] == 1 and m[start-1] == 1){
//				for(int i = start-2;i<=end;i++){
//					m[i] = 0;
//				}
//				range temp;
//				temp.start = start-2;
//				temp.end = end;
//				return temp;
//			}
//			else{
//				range temp;
//				temp.start = -1;
//				return temp;
//			}
//		}
//	}
//	else if(end-start == 4){
//		
//	}
//	else{
//		range temp;
//		temp.start = -1;
//		return temp;
//	}
}

int k = 8;
int main(){
	vector<int> vec;
	int size=9;
	vec.resize(size,0);
	for(int i = 0;i<size;i++){
		vec[i] = size-i;
		cout << vec[i] << " ";
	}
	cout <<endl;
	vector<range> job;
	map<int,int> m;
	for(int i = 0;i<k;i++){
		range temp;
		temp.start = (size/k)*i;
		if(i == k-1){
			temp.end = size-1;
		}
		else{
			temp.end = (size/k)*(i+1)-1;
		}
		m[i] = 0;
		job.push_back(temp);
	}
	while(job.size()!=0){
		range temp = job[0];
		job.erase(job.begin());
		bubblesort(vec,temp.start,temp.end);
		int start = temp.start/(size/k);
		int end;
		if(temp.end != size-1){
			end = (temp.end+1)/(size/k);
		}
		else{
			end = k;
		}
		for(int i = start;i<end;i++){
			m[i] = 1;
		}
		cout << start << " " << end << " ";
		range newjob = jobspawn(start, end, m);
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
		job.push_back(newjob);
	}
	for(int i = 0;i<size;i++){
		cout << vec[i]<<" ";
	}
	return 0;
}
