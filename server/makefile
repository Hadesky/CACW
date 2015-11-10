OBJ = main.o server.o httpserver.o simplemysql.o enrollaction.o\
	  registeraction.o httpenrollaction.o httpregisteraction.o\
	  multhreads.o httpmulthreads.o jsontransverter.o md5.o session.o\
	  sessionnode.o smtpmail.o

DEBUG = -DDEBUG
STD=-std=c++11
JSONLIBPATH=./jsonlib/

main: $(OBJ)
	g++ $(DEBUG) $(STD) -L $(JSONLIBPATH) -g $(OBJ) \
		-lmysqlclient -lpthread -ljson_linux-gcc-4.8_libmt -o main 
main.o: server.h main.cc
	g++ $(DEBUG) $(STD) -g -c main.cc
server.o: server.h server.cc
	g++ $(DEBUG) $(STD) -g -c server.cc
httpserver.o:server.h httpserver.cc simplemysql.h enrollaction.h\
	registeraction.h jsontransverter.h session.h
	g++ $(DEBUG) $(STD) -g -c httpserver.cc
simplemysql.o:simplemysql.cc
	g++ $(DEBUG) $(STD) -g -c simplemysql.cc
enrollaction.o:enrollaction.cc
	g++ $(DEBUG) $(STD) -g -c enrollaction.cc
registeraction.o:registeraction.h registeraction.cc
	g++ $(DEBUG) $(STD) -g -c registeraction.cc
httpenrollaction.o: simplemysql.h httpenrollaction.cc jsontransverter.h
	g++ $(DEBUG) $(STD) -g -c httpenrollaction.cc
httpregisteraction.o: simplemysql.h httpregisteraction.cc md5.h smtpmail.h
	g++ $(DEBUG) $(STD) -g -c httpregisteraction.cc
multhreads.o:multhreads.cc
	g++ $(DEBUG) $(STD) -g -c multhreads.cc
httpmulthreads.o:multhreads.h server.h httpmulthreads.cc
	g++ $(DEBUG) $(STD) -g -c httpmulthreads.cc
jsontransverter.o:jsontransverter.h jsontransverter.cc
	g++ $(DEBUG) $(STD) -g -c jsontransverter.cc
session.o:session.h session.cc
	g++ $(DEBUG) $(STD) -g -c session.cc
sessionnode.o:session.h sessionnode.cc
	g++ $(DEBUG) $(STD) -g -c sessionnode.cc
md5.o:md5.h md5.cc
	g++ $(DEBUG) $(STD) -g -c md5.cc
smtpmail.o:smtpmail.h smtpmail.cc
	g++ $(DEBUG) $(STD) -g -c smtpmail.cc
.PHONY:
clear:
	-rm *.o main
