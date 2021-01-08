#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "client_g0s1e6.c"

#define SERVER_PORT_L 22016

int main()
{
    printf("\nServer Start\n");
    int serverPort = SERVER_PORT_L; //L because local not to be confused with client port
    int rc, received;

    struct sockaddr_in serverSa;
    struct sockaddr_in clientSa;

    int clientSa_size;
    int on = 1;
    int c;
    int s = socket(PF_INET, SOCK_STREAM, 0); //TCP socket

    char msg[2000]; //Message Sent
    char Command[3] = "06#"; //SCTC2 Command
    int CommandCheck = 1; //Check for command 6

    client_main();

    /* Socket */
    rc = setsockopt(s,SOL_SOCKET, SO_REUSEADDR, &on, sizeof on); // set options on socket
    memset(&serverSa, 0, sizeof(serverSa)); // fill memory with a constant byte

    serverSa.sin_family = AF_INET;
    serverSa.sin_addr.s_addr = htonl(INADDR_ANY); // convert values between host and network byte order (hostlong)
    serverSa.sin_port = htons(serverPort); // hostshort

    /* Binding */
    printf("\nBinding...");
    rc = bind(s,(struct sockaddr *)&serverSa, sizeof(serverSa)); // bind a name to a socket
    if(rc<0){
        perror("Bind Failed");
        printf("\nBind Failed");
        exit(1);
    }
    else
    printf("\nBind Succeeded");

    /* Listening */
    printf("\nListening...");
    rc = listen(s, 10); // listen for connections on a socket
    if(rc<0){
        perror("Listen Failed");
        printf("\nListen Failed");
        return 0;
    }
    else
    printf("\nListen Succeeded");
    
    printf("\nAccepting...");
    while(1){ // infinite while loop to allow for multiple connections
        rc = accept(s,(struct sockaddr *)&clientSa,&clientSa_size); //accept a connection on a socket
        if (rc < 0)
        {
            perror("Accept Failed");
            printf("\nAccept Failed");
            break; // user timeout or disconnect
        }
        else
        printf("\nAccept Succeeded\n");

        /* Receiving */
        char *message;
        received = 1;
        while(received > 0){
            received = recv(rc, msg, 2000, 0); // receive a message from a socket
            if (received < 0){
                perror("Receive Failed");
                printf("\nReceive Failed");
                break; // user timeout or disconnect
                // this will keep socket alive
            }
            else{
            printf("\nReceive Succeeded");
                for(int i =0; i < sizeof(Command); i++){
                    if(msg[i] != Command[i]){
                        CommandCheck = 0;
                        break;
                    }else{
                        CommandCheck = 1;
                        continue;
                    }
                }
            }

            /* Sending */
            if(CommandCheck == 1){
                
                send(rc, msg_c6, strlen(msg_c6),0);
            } else{
                message = "\nNot implemented Command\n";
                send(rc, message, strlen(message),0);
            }
        }
    }
    /* Closing */
    c = rc;
    close(s);
    close(c);
     printf("\nServer End\n");
    return 0;
}