#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
	int server_sockfd;
	struct sockaddr_in server_addr;
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(80);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if ( (server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket error%s\n", strerror(errno));
		close(server_sockfd);
		return -1;
	}
	if (bind(server_sockfd, 
			 (struct sockaddr *)&server_addr,
			 sizeof(server_addr)) < 0) {
		printf("bind error :%s\n", strerror(errno));
		close(server_sockfd);
		return -1;
	}
	{
		socklen_t sockopt;
		setsockopt(server_sockfd, 
				   IPPROTO_TCP,
				   SO_REUSEADDR,
				   &sockopt,
				   sizeof(sockopt));
	}
	if (listen(server_sockfd, 10) < 0) {
		printf("listen error: %s\n", strerror(errno));
		close(server_sockfd);
		return -1;
	}
#define BUFSIZE 256
	char buf[BUFSIZE];
	struct sockaddr_in client_addr;
	char html[] = 
		"<html>/r/n"
		"<head>/r/n"
		"<title>fuck</title>/r/n"
		"<//head>/r/n"
		"</html>/r/n";
	while(1) {
		int client_sockfd;
		socklen_t client_addr_len;
		if ( (client_sockfd  = accept(server_sockfd, 
						(struct sockaddr *)&client_addr,
						&client_addr_len)) < 0) {
			printf("accept error: %s \n", strerror(errno));
			close(server_sockfd);
		}
		int n = recv(client_sockfd, buf, BUFSIZE, MSG_ERRQUEUE);
		buf[n] = '\0';
		printf("%s\n", buf);
		send(client_sockfd, html, strlen(html), 0);
		close(client_sockfd);
	}

	return 0;
}

