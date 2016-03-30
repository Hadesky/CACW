#include "server.h"


#ifdef DEBUG
#include <cstdio>
#endif // ! DEBUG
int main() {
	Server *server = new HttpServer();
#ifdef DEBUG
	printf("Init Server...\n");
#endif // ! DEBUG
	server->Init();
#ifdef DEBUG
	printf("Start Server...\n");
#endif // ! DEBUG
	server->Start();
	delete server;

	return 0;
}
