#include "server.h"

int main() {
	Server *server = new HttpServer();
	server->Init();
	server->Start();

	delete server;
}
