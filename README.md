# Redis Server
It is what it is, written in C, started as an example in a book and expanded as I see fit.

## The Event Loop and Nonblocking IO
Will be using an event loop as a part of the design.

- The syscall for setting an fd to nonblocking mode is fcntl
- 