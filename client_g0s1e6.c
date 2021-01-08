#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define SERVER_PORT 80

char msg_c6[] = "GET / HTTP/1.0\r\n\r\n"; // global variable so it can be used by server

int client_main()
{
	FILE *fp;
	fp = fopen("index.html", "w");

    printf("\n\n\tClient Start\n");
	int sock_fd = -1;
	struct sockaddr_in6 server_addr;
	int ret;
 
	/* Create socket for communication with server */
	sock_fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP); // create an endpoint for communication, Domain AF_INET6 = IPv6 Internet Protocols, SOCK_STREAM = two-way connection
	if (sock_fd == -1) { // If socket creation fails
		perror("socket()");
		return EXIT_FAILURE;
	}
    printf("\tSocket created successfully!\n");

	/* Connect to server running on localhost */
	server_addr.sin6_family = AF_INET6; // IPv6 Internet Protocols
	inet_pton(AF_INET6, "2001:2e8:665:0:2:1:0:2d", &server_addr.sin6_addr); // Convert IPv6 addresses from text to binary form
	server_addr.sin6_port = htons(SERVER_PORT); // convert values between host and network byte order
    printf("\tConnection to server was successful!\n");

	/* Try to do TCP handshake with server */
	ret = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)); // initiate a connection on a socket
	if (ret == -1) {
		perror("connect()");
		close(sock_fd);
		return EXIT_FAILURE;
	}
    printf("\tHandshake successful!\n");

	/* Send data to server */
	ret = write(sock_fd, msg_c6, strlen(msg_c6));
	if (ret == -1) {
		perror("write");
		close(sock_fd);
		return EXIT_FAILURE;
	}
    printf("\tSent \n\n\tWaiting...\n\n");

	/* Wait for data from server */
	ret = read(sock_fd, msg_c6, strlen(msg_c6));
	if (ret == -1) {
		perror("read()");
		close(sock_fd);
		return EXIT_FAILURE;
	}
	printf("\tReceived: %s \n", msg_c6);
	printf("\n\tEnd of message!");
	fprintf(fp, msg_c6);

	/* DO TCP teardown */
	ret = close(sock_fd);
	if (ret == -1) {
		perror("close()");
		return EXIT_FAILURE;
	}
 
	printf("\n\tClient End\n\n");
	fclose(fp);
	return EXIT_SUCCESS;
}