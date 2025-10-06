# An open REDIS server written in C

## Notes about the code

### Utilizes the following linux systrem calls:
- socket() : returns an FD, which is an integer that refers to something in the Linux kernel, like a TCP connection, a disk file, a listening port, or some other resources, etc
- bind() : associates an address to the socket fd
- listen() : accepts connections to the bind address
- accept() : takes a listening fd, when a client makes a connection to the listening address and returns an fd that represents the connection socket.

