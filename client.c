#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;
  char text[BUFFER_SIZE];

  from_server = client_handshake( &to_server );

  while (1) {
    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);
    text[strlen(text) - 1] = '\0';

    write(to_server, text, sizeof(text));
    read(from_server, text, sizeof(text));
    printf("RESPONSE: %s\n", text);
  }
  return 0;
}
