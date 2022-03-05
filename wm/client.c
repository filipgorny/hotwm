#include "client.h"
#include "window.h"

#include <stdio.h>
#include <stdlib.h>

Client *client_create(Window *window) {
  Client *client = malloc(sizeof(Client));
  client->window = window;

  return client;
}
