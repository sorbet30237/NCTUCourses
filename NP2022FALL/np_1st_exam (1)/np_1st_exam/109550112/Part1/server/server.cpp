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



struct User{
    string username = "";
    string email = "";
    string password = "";
    bool islogin = false;
    string num = "";
    int guesstime = 5;
};



int main(int argc, char** argv){
    
    
    
    vector<User> user_vec;
    vector<int> socket_tcp_new_vec;
    
    map<int,string> logginguser;
    
    
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
    
    int socket_udp = socket(PF_INET, SOCK_DGRAM, 0);
    if(socket_udp == -1){
        cout << "UDP socket failed!" << endl;
        exit(-1);
    }
    setsockopt(socket_udp, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    int flags_udp = fcntl(socket_udp, F_GETFL, 0);
    fcntl(socket_udp, F_SETFL, flags_udp | O_NONBLOCK);
    
    
    
    if(bind(socket_udp, (const struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        cout << "UDP socket bind failed!" << endl;
        close(socket_udp);
        exit(-1);
    }
    
    cout << "UDP server is running" << endl;
    
    while(true){
        
        char msg_recv[10000] = {'\0'};
        
        struct sockaddr_in client_addr_udp;
        socklen_t client_len_udp = sizeof(client_addr_udp);
        
        if(recvfrom(socket_udp, msg_recv, sizeof(msg_recv), 0, (struct sockaddr*)&client_addr_udp, &client_len_udp) != -1){
            
            vector<string> cmd_vec;
            char* cmd = strtok(msg_recv, " ");
            while(cmd != NULL) {
                string cmd_ = cmd;
                cmd_vec.push_back(cmd_);
                cmd = strtok(NULL, " ");
            }

            if(cmd_vec[0] == "exit"){

			}
			else{
				
			}
        }
    
    }



    if(close(socket_udp) < 0) {
        cout << "UDP socket close failed!" << endl;
        exit(-1);
    }

    return 0;
}
