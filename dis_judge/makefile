judged : judged.o SOCserver.a testServer.a testClient localServ
	g++ -Wall -L/usr/local/mysql/lib/mysql -L/usr/lib/mysql -lmysqlclient -o judged judged.o testServer.a SOCserver.a

testClient : testClient.o SOCserver.a
	g++ -o testClient testClient.o SOCserver.a

judged.o : judged.cc testServer.h
	g++ -Wall -c -g -I/usr/local/mysql/include/mysql -I/usr/include/mysql -lpthread judged.cc

SOCserver.o : SOCserver.c SOCserver.h
	g++ -c SOCserver.c

SOCserver.a : SOCserver.o
	ar -rc SOCserver.a SOCserver.o
	
testServer.o : testServer.c testServer.h
	g++ -c testServer.c

testServer.a : testServer.o
	ar -rc testServer.a testServer.o
	
testClient.o : testClient.c SOCserver.h
	g++ -c testClient.c
	
localServ : localServ.c SOCserver.h
	g++ -o localServ localServ.c SOCserver.a

clean :
	rm judged judged.o SOCserver.o testServer.o testClient testClient.o SOCserver.a testServer.a localServ