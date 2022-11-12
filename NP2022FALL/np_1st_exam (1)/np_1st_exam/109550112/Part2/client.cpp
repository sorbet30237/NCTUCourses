#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>



using namespace std;



int main(int argc, char** argv){
    
    char* server_ip = argv[1];
    int server_port = atoi(argv[2]);
    
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
    
    
    
    if(connect(socket_tcp, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        cout << "TCP socket connect failed!" << endl;
        close(socket_tcp);
        exit(-1);
    }
    
    char msg_recv_[10000] = {'\0'};
    if(recv(socket_tcp, msg_recv_, sizeof(msg_recv_), 0) == -1) {
        cout << "TCP socket recv failed!"<< endl;
        close(socket_tcp);
        exit(-1);
    }
    cout << msg_recv_ << endl;
    
    

    while(true){
        
        char msg_send[10000] = {'\0'};
        cin.getline(msg_send, 10000);
        char msg_send_[10000] = {'\0'};
        strcpy(msg_send_, msg_send);
        
        char msg_recv[10000] = {'\0'};

        send(socket_tcp, msg_send, sizeof(msg_send), 0);
        if(strcmp(msg_send, "exit") == 0){
            close(socket_tcp);
            exit(0);
        }
		if(strcmp(msg_send, "list-users"){
			while(recv(socket_tcp, msg_recv, sizeof(msg_recv), 0) =! -1){
	            cout << "TCP socket recv failed!"<< endl;
	            close(socket_tcp);
	            exit(-1);
	        }
	        cout << msg_recv << endl;
		}
        if(recv(socket_tcp, msg_recv, sizeof(msg_recv), 0) == -1) {
            cout << "TCP socket recv failed!"<< endl;
            close(socket_tcp);
            exit(-1);
        }
        cout << msg_recv << endl;
    }

    if(close(socket_tcp) < 0) {
        cout << "TCP socket close failed!" << endl;
        exit(-1);
    }

    return 0;
}
