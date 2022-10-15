#include <iostream>
#include <string>
#include <cstring>

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
};



int main(){
    
    vector<User> user_vec;
    vector<int> socket_tcp_new_vec;
    
    map<int,string> logginguser;
    
    int max_n_client = 100;
    
    char* server_ip = "127.0.0.1";
    int server_port = 8888;
    
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
    
    int socket_tcp = socket(PF_INET, SOCK_STREAM, 0);
    if(socket_tcp == -1){
        cout << "TCP socket failed!" << endl;
        exit(-1);
    }
    setsockopt(socket_tcp, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    int flags_tcp = fcntl(socket_tcp, F_GETFL, 0);
    fcntl(socket_tcp, F_SETFL, flags_tcp | O_NONBLOCK);
    
    
    
    if(bind(socket_udp, (const struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        cout << "UDP socket bind failed!" << endl;
        close(socket_udp);
        close(socket_tcp);
        exit(-1);
    }
    if(bind(socket_tcp, (const struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        cout << "TCP socket bind failed!" << endl;
        close(socket_udp);
        close(socket_tcp);
        exit(-1);
    }
    if(listen(socket_tcp, max_n_client) == -1){
        cout << "TCP socket listen failed!" << endl;
        close(socket_udp);
        close(socket_tcp);
        exit(-1);
    }
    
    
    
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

            if(cmd_vec[0] == "register"){
                
                if(cmd_vec.size() != 4){
                    char msg_send[] = "Usage: register <username> <email> <password>";
                    sendto(socket_udp, msg_send, sizeof(msg_send), 0, (struct sockaddr*)&client_addr_udp, sizeof(client_addr_udp));
                }
                else{
                    
                    bool success = true;
                    for(int i = 0; i < user_vec.size(); i++) {
                        if(cmd_vec[1] == user_vec[i].username){
                            success = false;
                            char msg_send[] = "Username is already used.";
                            sendto(socket_udp, msg_send, sizeof(msg_send), 0, (struct sockaddr*)&client_addr_udp, sizeof(client_addr_udp));
                            break;
                        }
                        if(cmd_vec[2] == user_vec[i].email){
                            success = false;
                            char msg_send[] = "Email is already used.";
                            sendto(socket_udp, msg_send, sizeof(msg_send), 0, (struct sockaddr*)&client_addr_udp, sizeof(client_addr_udp));
                            break;
                        }
                    }
                    
                    if(success){

                        struct User user;
                        user.username = cmd_vec[1];
                        user.email = cmd_vec[2];
                        user.password = cmd_vec[3];
                        user_vec.push_back(user);

                        char msg_send[] = "Register successfully.";
                        sendto(socket_udp, msg_send, sizeof(msg_send), 0, (struct sockaddr*)&client_addr_udp, sizeof(client_addr_udp));
                    }
                }
            }
            else if(cmd_vec[0] == "game-rule"){
                if(cmd_vec.size()!= 1){
                    char msg_send[] = "Usage: game-rule";
                    sendto(socket_udp, msg_send, sizeof(msg_send), 0, (struct sockaddr*)&client_addr_udp, sizeof(client_addr_udp));
                }
                else{
                    char msg_send[] = "1. Each question is a 4-digit secret number.\n2. After each guess, you will get a hint with the following information:\n2.1 The number of \"A\", which are digits in the guess that are in the correct position.\n2.2 The number of \"B\", which are digits in the guess that are in the answer but are in the wrong position.\nThe hint will be formatted as \"xAyB\".\n3. 5 chances for each question.";
                    sendto(socket_udp, msg_send, sizeof(msg_send), 0, (struct sockaddr*)&client_addr_udp, sizeof(client_addr_udp));
                }
            }
        }
        
        
        
        int socket_tcp_new = accept(socket_tcp, NULL, NULL);
        
        if(socket_tcp_new != -1){
            
            setsockopt(socket_tcp_new, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
            int flags_tcp_new = fcntl(socket_tcp_new, F_GETFL, 0);
            fcntl(socket_tcp_new, F_SETFL, flags_tcp_new | O_NONBLOCK);
            
            cout << "New Connection." << endl;
            send(socket_tcp_new, "*****Welcome to Game 1A2B*****", sizeof("*****Welcome to Game 1A2B*****"), 0);

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
                if(cmd_vec[0] == "login"){
                    if(cmd_vec.size() != 3){
                        char msg_send[] = "Usage: login <username> <password>";
                        send(socket_tcp_new_vec[i], msg_send, sizeof(msg_send), 0);
                    }
                    else{
                        bool isuser = false;
                        bool isbreak = false;
                        for(int j = 0;j<user_vec.size();j++){
                            if(user_vec[j].username == cmd_vec[1]){
                                isuser = true;
                                if(user_vec[j].password != cmd_vec[2]){
                                    char msg_send[] = "Password not correct";
                                    send(socket_tcp_new_vec[i], msg_send, sizeof(msg_send), 0);
                                    isbreak = true;
                                    break;
                                }
                                if(user_vec[j].islogin == true){
                                    char msg_send[] = "Please logout first";
                                    send(socket_tcp_new_vec[i], msg_send, sizeof(msg_send), 0);
                                    isbreak = true;
                                    break;
                                }
                            }
                        }
                        if(isuser == false){
                            char msg_send[] = "Username not found.";
                            send(socket_tcp_new_vec[i], msg_send, sizeof(msg_send), 0);
                        }
                        else{
                            if(isbreak == false and isuser == true){
                                string temp = "Welcome, " + cmd_vec[1] + ".";
                                char msg_send[temp.size()+1];
                                strcpy(msg_send,temp.c_str());
                                logginguser[socket_tcp_new] = cmd_vec[1];
                                send(socket_tcp_new_vec[i], msg_send, sizeof(msg_send), 0);
                            }
                        }
                    }
                }
                else if(cmd_vec[0] == "logout"){
                    if(cmd_vec.size() != 1){
                        char msg_send[] = "Usage: logout";
                        send(socket_tcp_new_vec[i], msg_send, sizeof(msg_send), 0);
                    }
                    else{
                        if(logginguser[socket_tcp_new] == ""){
                            char msg_send[] = "Please login first.";
                            send(socket_tcp_new_vec[i], msg_send, sizeof(msg_send), 0);
                        }
                        else{
                            for(int j = 0;j<user_vec.size();j++){
                                if(logginguser[socket_tcp_new] == user_vec[j].username){
                                    user_vec[j].islogin = false;
                                    logginguser[socket_tcp_new] = "";
                                    string temp = "Bye, " + user_vec[j].username +".";
                                    char msg_send[temp.size()+1];
                                    strcpy(msg_send,temp.c_str());
                                    send(socket_tcp_new_vec[i], msg_send, sizeof(msg_send), 0);
                                    break;
                                } 
                            }
                        }
                    }
                    
                }
                else if(cmd_vec[0] == "exit"){
                    if(cmd_vec.size() != 1){
                        char msg_send[] = "Usage: exit";
                        send(socket_tcp_new_vec[i], msg_send, sizeof(msg_send), 0);
                    }
                    else{
                        if(logginguser[socket_tcp_new] != ""){
                            for(int j = 0;j<user_vec.size();j++){
                                if(logginguser[socket_tcp_new] == user_vec[j].username){
                                    user_vec[j].islogin = false;
                                    logginguser[socket_tcp_new] = "";
                                }
                            }
                        }
                        if(close(socket_tcp_new_vec[i]) < 0) {
                            cout << "TCP socket new close failed!" << endl;
                            exit(0);
                        }
                        socket_tcp_new_vec.erase(socket_tcp_new_vec.begin() + i);
                    }
                }
            }
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
    
    return 0;
}
