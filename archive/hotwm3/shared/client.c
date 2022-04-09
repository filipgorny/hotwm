#include "client.h"
#include "window.h"

#include <stdio.h>
#include <stdlib.h>

Client *client_create(Window *window) {
  Client *client = (Client *)malloc(sizeof(Client));
  client->window = window;
  client->has_float_cords = false;
  client->name = "";

  return client;
}
