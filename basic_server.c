#include "pipe_networking.h"
#include <ctype.h>

int main() {

  int to_client;
  int from_client;
  //int i;
  char text[BUFFER_SIZE];

  printf("Server started, waiting for handshake...\n");

  from_client = server_handshake( &to_client );
  while (1) {
    read(from_client, text, sizeof(text));
    printf("TEXT READ: %s\n", text);
    char *s = text;
    while(*s) {
      *s = toupper(*s);
      s ++;
    }
    printf("TEXT TO SEND: %s\n", text);
    write(to_client, text, sizeof(text));
  }
}
