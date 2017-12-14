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
  char message1[10];
  char message2[10];
  int fifo, wkp;

  //sets up wkp and blocks until sent data from client
  if (mkfifo("well_known_pipe", 0666) == -1) {
    printf("1: %s\n", strerror(errno));
    exit(0);
  }
  if ((wkp = open("well_known_pipe", O_RDONLY)) == -1) {
    printf("2: %s\n", strerror(errno));
    exit(0);
  }  
  read(wkp, message1, HANDSHAKE_BUFFER_SIZE);
  printf("server: received %s from client\n", message1);
  remove("well_known_pipe");

  //once data is received, server writes to client
  fifo = open(message1, O_WRONLY);
  *to_client = fifo;
  printf("server: writing %s to client\n", message1);
  write(fifo, message1, HANDSHAKE_BUFFER_SIZE);

  //receive sent data back from pp and confirm it is correct
  read(wkp, message2, HANDSHAKE_BUFFER_SIZE);
  printf("server: received %s from client\n", message2);
  
  return wkp;
}


/*=========================
  client_handshake
  args: int * to_server

  Perofrms the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char private[10];
  int fifo, wkp;
  char message[10];

  //create message to send
  sprintf(private, "%d", getpid());

  //write to well known pipe the name of our private pipe
  wkp = open("well_known_pipe", O_WRONLY);
  printf("client: writing %s to server\n", private);
  write(wkp, private, HANDSHAKE_BUFFER_SIZE);
  *to_server = wkp;

  //client pipe is set up
  mkfifo(private, 0666);
  fifo = open(private, 0);

  //read data from wkp to confirm it passed successfully
  read(fifo, message, HANDSHAKE_BUFFER_SIZE);
  printf("client: received %s from server\n", message);
  remove(private);

  //write data to wkp to confirm it worked properly
  printf("client: writing %s to server\n", message);
  write(wkp, message, HANDSHAKE_BUFFER_SIZE);
  
  return fifo;
}
