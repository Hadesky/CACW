OBJ = main.o server.o httpserver.o simplemysql.o enrollaction.o\
	  registeraction.o httpenrollaction.o httpregisteraction.o\
	  multhreads.o httpmulthreads.o

main: $(OBJ)
	g++ -o main $(OBJ) -lmysqlclient -lpthread

main.o: server.h

server.o: 

httpserver.o:simplemysql.h enrollaction.h registeraction.h

simplemysql.o:

enrollaction.o:

registeraction.o:

httpenrollaction.o: simplemysql.h

httpregisteraction.o: simplemysql.h

multhreads.o:

httpmulthreads.o:multhreads.h server.h

.PHONY:
clear:
	-rm *.o main
