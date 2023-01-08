#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>



using namespace std;

class Client{
	public:
		
		struct sockaddr_in server_addr;
		int socket_udp;
		int socket_tcp;
		
		Client(char* server_ip, int server_port){
			
		    server_addr.sin_family = AF_INET;
		    server_addr.sin_addr.s_addr = inet_addr(server_ip);
		    server_addr.sin_port = htons(server_port);
		    
		    const int enable = 1;
		    
		    socket_udp = socket(PF_INET, SOCK_DGRAM, 0);
		    if(socket_udp == -1){
		        cout << "UDP socket failed!" << endl;
		        exit(-1);
		    }
		    setsockopt(socket_udp, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
		    
		    socket_tcp = socket(PF_INET, SOCK_STREAM, 0);
		    if(socket_tcp == -1){
		        cout << "TCP socket failed!" << endl;
		        exit(-1);
		    }
		    setsockopt(socket_tcp, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
		    
		}
	    void runclient(){
	    	if(connect(socket_tcp, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		        cout << "TCP socket connect failed!" << endl;
		        close(socket_udp);
		        close(socket_tcp);
		        exit(-1);
		    }
		    
		    char msg_recv_[10000] = {'\0'};
		    if(recv(socket_tcp, msg_recv_, sizeof(msg_recv_), 0) == -1) {
		        cout << "TCP socket recv failed!"<< endl;
		        close(socket_udp);
		        close(socket_tcp);
		        exit(-1);
		    }
		    cout << msg_recv_ << endl;
	    	while(true){
	    		char msg_send[10000] = {'\0'};
		        cin.getline(msg_send, 10000);
		        char msg_send_[10000] = {'\0'};
		        strcpy(msg_send_, msg_send);
		        
		        bool use_udp = false;
		        char* cmd = strtok(msg_send_, " ");
		        if(strcmp(cmd, "register") == 0){
		            use_udp = true;
		        }
		        
		        
		        char msg_recv[10000] = {'\0'};
		        if(use_udp){
		            
		            sendto(socket_udp, msg_send, sizeof(msg_send), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
		            
		            if(recvfrom(socket_udp, msg_recv, sizeof(msg_recv), 0, NULL, NULL) == -1){
		                cout << "UDP socket recv failed!"<< endl;
		                close(socket_udp);
		                close(socket_tcp);
		                exit(-1);
		            }
		            cout << msg_recv << endl;
		        }
		        else{
		            
		            send(socket_tcp, msg_send, sizeof(msg_send), 0);
		            if(strcmp(msg_send, "exit") == 0){
		                close(socket_udp);
		                close(socket_tcp);
		                exit(0);
		            }
		    
		            if(recv(socket_tcp, msg_recv, sizeof(msg_recv), 0) == -1) {
		                cout << "TCP socket recv failed!"<< endl;
		                close(socket_udp);
		                close(socket_tcp);
		                exit(-1);
		            }
		            cout << msg_recv << endl;
		        }
			}
			if(close(socket_udp) < 0) {
		        cout << "UDP socket close failed!" << endl;
		        exit(-1);
		    }
		    if(close(socket_tcp) < 0) {
		        cout << "TCP socket close failed!" << endl;
		        exit(-1);
		    }
		}
	    
};

int main(int argc, char** argv){
    char* server_ip = argv[1];
	int server_port = atoi(argv[2]);
    Client c(server_ip, server_port);
	c.runclient();
    return 0;
}
