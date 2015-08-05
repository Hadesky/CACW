OBJ = main.o server.o httpserver.o simplemysql.o enrollaction.o\
	  registeraction.o httpenrollaction.o httpregisteraction.o\
	  multhreads.o httpmulthreads.o

main: $(OBJ)
	g++ -g -o main $(OBJ) -lmysqlclient -lpthread
main.o: server.h main.cc
	g++ -g -c main.cc
server.o: server.cc
	g++ -g -c server.cc
httpserver.o:httpserver.cc simplemysql.h enrollaction.h registeraction.h 
	g++ -g -c httpserver.cc
simplemysql.o:simplemysql.cc
	g++ -g -c simplemysql.cc
enrollaction.o:enrollaction.cc
	g++ -g -c enrollaction.cc
registeraction.o:registeraction.cc
	g++ -g -c registeraction.cc
httpenrollaction.o: simplemysql.h httpenrollaction.cc
	g++ -g -c httpenrollaction.cc
httpregisteraction.o: simplemysql.h httpregisteraction.cc
	g++ -g -c httpregisteraction.cc
multhreads.o:multhreads.cc
	g++ -g -c multhreads.cc
httpmulthreads.o:multhreads.h server.h httpmulthreads.cc
	g++ -g -c httpmulthreads.cc
.PHONY:
clear:
	-rm *.o main
