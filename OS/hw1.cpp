#include<iostream>
#include<cstring>
#include<string>
#include<vector>
#include<unistd.h>
#include<sys/types.h>
#include <wait.h>

using namespace std;
int main(){
    while(true){
        string s;
        cout << ">";
        getline(cin, s);
        const char *temp;
        temp = s.c_str();
        char *str;
        str = strdup(temp);
        char *p;
        const char *d = " ";
        p = strtok(str, d);
        vector<char*> S;
        while (p != NULL) {
            S.push_back(p);
            p = strtok(NULL, d);		   
        }
        char **cmd = new char*[S.size()+1];
        cmd[S.size()] = NULL;
        for(int i = 0;i<S.size();i++){
            cmd[i] = S[i];
//            cout << cmd[i] << " ";
        }
        pid_t pid = fork();
        if(pid < 0){
            cout << "fork failed";
            exit(-1);
        }
        else if(pid == 0){
            pid = fork();
            if(pid < 0){
                cout << "fork failed";
                exit(-1);
            } 
            else if(pid == 0){
                execvp(cmd[0],cmd);
            }
            else{
                if(cmd[S.size()-1] != "&"){
                    wait(NULL);
                }
                exit(-1);
            }
        }
        else{
            wait(NULL);
        }
        cout << endl;
    }
    return 0;
}
