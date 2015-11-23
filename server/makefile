OBJ = main.o server.o httpserver.o simplemysql.o enrollaction.o\
	  registeraction.o httpenrollaction.o httpregisteraction.o\
	  jsontransverter.o md5.o session.o\
	  sessionnode.o

DEBUG = -DDEBUG
GDB = 
STD=-std=c++11
JSONLIBPATH=./jsonlib/
main.exe: $(OBJ)
	g++ $(DEBUG) $(STD) -L $(JSONLIBPATH) ${GDB} $(OBJ) \
		-lmysqlclient -lpthread -ljson_linux-gcc-4.8_libmt -o main.exe
main.o: server.h main.cc
	g++ $(DEBUG) $(STD) ${GDB} -c main.cc
server.o: server.h server.cc
	g++ $(DEBUG) $(STD) ${GDB} -c server.cc
httpserver.o:server.h httpserver.cc simplemysql.h enrollaction.h\
	registeraction.h jsontransverter.h session.h
	g++ $(DEBUG) $(STD) ${GDB} -c httpserver.cc
simplemysql.o:simplemysql.cc
	g++ $(DEBUG) $(STD) ${GDB} -c simplemysql.cc
enrollaction.o:enrollaction.cc
	g++ $(DEBUG) $(STD) ${GDB} -c enrollaction.cc
registeraction.o:registeraction.h registeraction.cc
	g++ $(DEBUG) $(STD) ${GDB} -c registeraction.cc
httpenrollaction.o: simplemysql.h httpenrollaction.cc jsontransverter.h
	g++ $(DEBUG) $(STD) ${GDB} -c httpenrollaction.cc
httpregisteraction.o: simplemysql.h httpregisteraction.cc md5.h
<<<<<<< HEAD
	g++ $(DEBUG) $(STD) ${GDB} -c httpregisteraction.cc
multhreads.o:multhreads.cc
	g++ $(DEBUG) $(STD) ${GDB} -c multhreads.cc
httpmulthreads.o:multhreads.h server.h httpmulthreads.cc
	g++ $(DEBUG) $(STD) ${GDB} -c httpmulthreads.cc
httpregister
	g++ $(DEBUG) $(STD) -g -c httpregisteraction.cc
>>>>>>> e8a24b2bab36df06cae810cb361ec40c1dd0237f
jsontransverter.o:jsontransverter.h jsontransverter.cc
	g++ $(DEBUG) $(STD) ${GDB} -c jsontransverter.cc
md5.o:md5.h md5.cc
	g++ $(DEBUG) $(STD) ${GDB} -c md5.cc
session.o:session.h session.cc
	g++ $(DEBUG) $(STD) ${GDB} -c session.cc
sessionnode.o:session.h sessionnode.cc
	g++ $(DEBUG) $(STD) ${GDB} -c sessionnode.cc
.PHONY:
clear:
	-rm *.o *.exe
