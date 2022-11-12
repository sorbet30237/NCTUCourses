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
    
    int socket_udp = socket(PF_INET, SOCK_DGRAM, 0);
    if(socket_udp == -1){
        cout << "UDP socket failed!" << endl;
        exit(-1);
    }
    setsockopt(socket_udp, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    
    
    

    while(true){
        cout << "% ";
        char msg_send[10000] = {'\0'};
        cin.getline(msg_send, 10000);

        
        char msg_recv[10000] = {'\0'};
            
        sendto(socket_udp, msg_send, sizeof(msg_send), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        
        if(strcmp(msg_send, "exit") == 0){
                close(socket_udp);
                
                exit(0);
        }
            
        if(recvfrom(socket_udp, msg_recv, sizeof(msg_recv), 0, NULL, NULL) == -1){
            cout << "UDP socket recv failed!"<< endl;
            close(socket_udp);
            exit(-1);
        }
        cout << msg_recv << endl;
    }

    if(close(socket_udp) < 0) {
        cout << "UDP socket close failed!" << endl;
        exit(-1);
    }

    return 0;
}
