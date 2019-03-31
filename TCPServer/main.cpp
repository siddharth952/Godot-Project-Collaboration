
//  main.cpp
//  TCPServer
//
//  Created by Siddharth Sen on 29/03/19.
//  Copyright © 2019 Siddharth Sen. All rights reserved.


#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


int main() {
    
        
    //Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);//Ver. 4
    if (listening == -1){
        std::cerr << "Not able to create socket!";
        return -1;
    }
    
    //Bind the socket to IP or port
    sockaddr_in hint;                     ///Binding to any IP Address for now ,Any Network (54000 ,54001..54003)
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000); //htons(Converting to what network understands from machine) flips bits
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); //192.168.1.1 ,( AF_INET,anyip,buffer)
    
    if (::bind(listening, (sockaddr*)&hint, sizeof(hint))) {
        
        std::cerr<<"Cannot Bind to Port/IP";
        return -2;
    }
    
    //mark such that socket is listening
    if (listen(listening, SOMAXCONN) == -1 ){
        std::cerr<<"Not able to listen";
        return -3;
    }
    
    //accept calls
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
                                  
                                  if (clientSocket == -1){
                                      std::cerr<<"Client not connecting!";
                                      return -4;
                                  }
    
    
    //close listening port
    close(listening);
    
    memset(host, 0, NI_MAXHOST); //To Clean up garbage
    memset(svc, 0, NI_MAXSERV); //To Clean up garbage
    
    
    
    int result = getnameinfo( (sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);   // Name of Computer
    if (result) {
        std::cout<<host<<" Connected On " << svc << std::endl;
        
    }else{
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST); //Opp. of ptop num. to string
        std::cout<<host<<" Connected On " <<ntohs(client.sin_port) << std::endl;
    }

    
    //while receiving display message ie echo the message
    
    char buff[4096];
    while (true) {
        //Clear the buffer
        memset(buff, 0, 4096);
        //Wait for message
        int bytesReceived = recv(clientSocket, buff, 4096, 0);
        if (bytesReceived == -1) {
            std::cout<<"Connection Issue!"<<std::endl;
            break;
        }
        if (bytesReceived == 0) {
            std::cout<<"Client Disconnected!"<<std::endl;
            break;
        }
        //Display Message
        std::cout<<"Received: "<<std::string(buff,0,bytesReceived)<<std::endl; //Display Client Sent
        
        //Resend message
        send(clientSocket, buff, bytesReceived+1, 0); // +1 for 0 at end
        
        
    }
    
    //close socket
    close(clientSocket);
    
    return 0;
}
