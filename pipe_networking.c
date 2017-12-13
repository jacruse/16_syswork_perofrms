#include "pipe_networking.h"

/*=========================
  server_handshake
  args: int * to_client

  Perofrms the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  char message[20];
  int fifo, wkp;
  mkfifo("well_known_pipe", 0);
  wkp = open("well_known_pipe", 0);
  *to_client = wkp;
  read(wkp, &message, sizeof(message));
  fifo = open(message, 0);
  write(fifo, "hello!", sizeof(char *));
  read(wkp, message, sizeof(message));
  close(wkp);

  return fifo;
}


/*=========================
  client_handshake
  args: int * to_server

  Perofrms the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char private[20];
  int fifo, wkp;
  char message[20];

  sprintf(private, "private_pipe_%d", getpid());
  mkfifo(private, 0644);
  fifo = open(private, 0);

  wkp = open("well_known_pipe", 0);
  *to_server = fifo;
  close(fifo);
  write(wkp, private, sizeof(private));
  read(fifo, message, 0);
  write(wkp, message, sizeof(message));
  return wkp;
}
