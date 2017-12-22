sa:Main.cpp ../libleveldb.a ./src/hashutil.o
	g++ -msse3 -o sa Main.cpp ../libleveldb.a ./src/hashutil.o  -lpthread -lssl -lcrypto -o $@
clean:
	rm -rf tdb *.a sa load zread uread

load:load.cpp ../libleveldb.a ./src/hashutil.o
	g++ -msse3 -o load load.cpp ../libleveldb.a ./src/hashutil.o  -lpthread -lssl -lcrypto -o $@
	
zread:zread.cpp ../libleveldb.a ./src/hashutil.o
	g++ -msse3 -o zread zread.cpp ../libleveldb.a ./src/hashutil.o  -lpthread -lssl -lcrypto -o $@

uread:uread.cpp ../libleveldb.a ./src/hashutil.o
	g++ -msse3 -o uread uread.cpp ../libleveldb.a ./src/hashutil.o  -lpthread -lssl -lcrypto -o $@
