clean: 
	rm dist/client dist/server
compile:
	gcc -Wall -Wextra -O2 -g server.c -o dist/server
	gcc -Wall -Wextra -O2 -g client.c -o dist/client
