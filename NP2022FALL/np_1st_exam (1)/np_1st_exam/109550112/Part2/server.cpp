#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <unistd.h>



using namespace std;

int count = 1;

int main(int argc, char** argv){

    vector<int> socket_tcp_new_vec;
    
    map<int,int> logginguser;
    
    
    string digits = "0123456789";
	srand( time(NULL) );
    
    int max_n_client = 100;
    
    char* server_ip = "127.0.0.1";
    int server_port = atoi(argv[1]);
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);
    
    

    const int enable = 1;
    
    int socket_tcp = socket(PF_INET, SOCK_STREAM, 0);
    if(socket_tcp == -1){
        cout << "TCP socket failed!" << endl;
        exit(-1);
    }
    setsockopt(socket_tcp, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    int flags_tcp = fcntl(socket_tcp, F_GETFL, 0);
    fcntl(socket_tcp, F_SETFL, flags_tcp | O_NONBLOCK);
    
    if(bind(socket_tcp, (const struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        cout << "TCP socket bind failed!" << endl;
        close(socket_tcp);
        exit(-1);
    }
    if(listen(socket_tcp, max_n_client) == -1){
        cout << "TCP socket listen failed!" << endl;
        close(socket_tcp);
        exit(-1);
    }
    
    cout << "TCP server is running" << endl;
    
    while(true){
        
        char msg_recv[10000] = {'\0'};
        
        struct sockaddr_in client_addr_tcp;
        socklen_t client_len_udp = sizeof(client_addr_tcp);
        
        int socket_tcp_new = accept(socket_tcp, (struct sockaddr *)&client_addr_tcp, &client_len_udp);
//        cout << client_addr_tcp.sin_addr.s_addr;
        
        if(socket_tcp_new != -1){
            
            setsockopt(socket_tcp_new, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
            int flags_tcp_new = fcntl(socket_tcp_new, F_GETFL, 0);
            fcntl(socket_tcp_new, F_SETFL, flags_tcp_new | O_NONBLOCK);
            
            cout << "New connection from user"<< count << endl;
            string message = "Welcome, you are user";
            message += to_string(count);
            logginguser[socket_tcp_new] = count;
            count++;
            send(socket_tcp_new, message.c_str(), sizeof(message), 0);

            socket_tcp_new_vec.push_back(socket_tcp_new);
        }
        
        
        
        for(int i = 0; i < socket_tcp_new_vec.size(); i++){
            
            memset(msg_recv, '\0', sizeof(msg_recv));
            
            if(recv(socket_tcp_new_vec[i], msg_recv, sizeof(msg_recv), 0) != -1){

                vector<string> cmd_vec;
                char* cmd = strtok(msg_recv, " ");
                while(cmd != NULL) {
                    string cmd_ = cmd;
                    cmd_vec.push_back(cmd_);
                    cmd = strtok(NULL, " ");
                }
                if(cmd_vec[0] == "exit"){
                    if(close(socket_tcp_new_vec[i]) < 0) {
                        cout << "TCP socket new close failed!" << endl;
                        exit(0);
                    }
                    socket_tcp_new_vec.erase(socket_tcp_new_vec.begin() + i);
                    cout << "user " << logginguser[socket_tcp_new_vec[i]] <<" disconnected"<<endl;
                    logginguser.erase(socket_tcp_new_vec[i]);
            	}
            	else if(cmd_vec[0] == "list-users"){
            		for(int i = 0; i < socket_tcp_new_vec.size(); i++){
            			string s;
            			s = "user";
            			s+=to_string(logginguser[socket_tcp_new_vec[i]]);
            			char msg_send[] = "";
            			strcpy(msg_send, s.c_str());
	                    send(socket_tcp_new_vec[i], msg_send, sizeof(msg_send), 0);
					}
				}
            }
        }
    }

    if(close(socket_tcp) < 0) {
        cout << "TCP socket close failed!" << endl;
        exit(-1);
    }
    
    return 0;
}
