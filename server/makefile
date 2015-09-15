OBJ = main.o server.o httpserver.o simplemysql.o enrollaction.o\
	  registeraction.o httpenrollaction.o httpregisteraction.o\
	  multhreads.o httpmulthreads.o jsontransverter.o

DEBUG = -DDEBUG
STD=-std=c++11
JSONLIBPATH=./jsonlib/

main: $(OBJ)
	g++ $(DEBUG) $(STD) -L $(JSONLIBPATH) -g -o main $(OBJ) \
		-lmysqlclient -lpthread -ljson_linux-gcc-4.8_libmt 
main.o: server.h main.cc
	g++ $(DEBUG) $(STD) -g -c main.cc
server.o: server.cc
	g++ $(DEBUG) $(STD) -g -c server.cc
httpserver.o:httpserver.cc simplemysql.h enrollaction.h registeraction.h 
	g++ $(DEBUG) $(STD) -g -c httpserver.cc
simplemysql.o:simplemysql.cc
	g++ $(DEBUG) $(STD) -g -c simplemysql.cc
enrollaction.o:enrollaction.cc
	g++ $(DEBUG) $(STD) -g -c enrollaction.cc
registeraction.o:registeraction.cc
	g++ $(DEBUG) $(STD) -g -c registeraction.cc
httpenrollaction.o: simplemysql.h httpenrollaction.cc jsontransverter.h
	g++ $(DEBUG) $(STD) -g -c httpenrollaction.cc
httpregisteraction.o: simplemysql.h httpregisteraction.cc
	g++ $(DEBUG) $(STD) -g -c httpregisteraction.cc
multhreads.o:multhreads.cc
	g++ $(DEBUG) $(STD) -g -c multhreads.cc
httpmulthreads.o:multhreads.h server.h httpmulthreads.cc
	g++ $(DEBUG) $(STD) -g -c httpmulthreads.cc
jsontransverter.o:
	g++ $(DEBUG) $(STD) -g -c jsontransverter.cc
.PHONY:
clear:
	-rm *.o main
