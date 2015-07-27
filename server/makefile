OBJ = main.o server.o httpserver.o simplemysql.o enrollaction.o\
	  registeraction.o httpenrollaction.o httpregisteraction.o

main: $(OBJ)
	g++ -o main $(OBJ) -lmysqlclient

main.o: server.h

server.o: 

httpserver.o:simplemysql.h enrollaction.h registeraction.h

simplemysql.o:

enrollaction.o:

registeraction.o:

httpenrollaction.o: simplemysql.h

httpregisteraction.o: simplemysql.h

.PHONY:
clear:
	-rm *.o main
