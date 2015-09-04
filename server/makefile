OBJ = main.o server.o httpserver.o simplemysql.o enrollaction.o\
	  registeraction.o httpenrollaction.o httpregisteraction.o\
	  multhreads.o httpmulthreads.o

DEBUG = -DDEBUG

main: $(OBJ)
	g++ $(DEBUG) -g -o main $(OBJ) -lmysqlclient -lpthread
main.o: server.h main.cc
	g++ $(DEBUG) -g -c main.cc
server.o: server.cc
	g++ $(DEBUG) -g -c server.cc
httpserver.o:httpserver.cc simplemysql.h enrollaction.h registeraction.h 
	g++ $(DEBUG) -g -c httpserver.cc
simplemysql.o:simplemysql.cc
	g++ $(DEBUG) -g -c simplemysql.cc
enrollaction.o:enrollaction.cc
	g++ $(DEBUG) -g -c enrollaction.cc
registeraction.o:registeraction.cc
	g++ $(DEBUG) -g -c registeraction.cc
httpenrollaction.o: simplemysql.h httpenrollaction.cc
	g++ $(DEBUG) -g -c httpenrollaction.cc
httpregisteraction.o: simplemysql.h httpregisteraction.cc
	g++ $(DEBUG) -g -c httpregisteraction.cc
multhreads.o:multhreads.cc
	g++ $(DEBUG) -g -c multhreads.cc
httpmulthreads.o:multhreads.h server.h httpmulthreads.cc
	g++ $(DEBUG) -g -c httpmulthreads.cc
.PHONY:
clear:
	-rm *.o main
