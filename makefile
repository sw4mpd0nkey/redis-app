.IGNORE: clean

all: clean compile

clean: 
	rm dist/server dist/client

server: server.c
	gcc -Wall -Wextra -O2 -g redis.c server.c -o dist/server

client: client.c
	gcc -Wall -Wextra -O2 -g redis.c client.c -o dist/client

compile: server client
