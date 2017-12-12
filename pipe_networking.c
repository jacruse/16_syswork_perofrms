#include "pipe_networking.h"
#define READ 0
#define WRITE 1

/*=========================
  server_handshake
  args: int * to_client

  Perofrms the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  return 0;
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
  read(fifo, &message, sizeof(message));

  wkp = open("well_known_pipe", 0);
  write(wkp, &private, sizeof(private));

  close(fifo);
  write(wkp, &message, sizeof(message));

  return 0;
}
