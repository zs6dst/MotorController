#ifndef __WEBSERVER__
#define __WEBSERVER__

#include <WebServer.h>
#include <WebSocketsServer.h>
#include "main.h"

void setupWeb(WebServer&, WebSocketsServer&, WebSocketsServerCore::WebSocketServerEvent);
void sendData(WebSocketsServer&, const Data *);

#endif