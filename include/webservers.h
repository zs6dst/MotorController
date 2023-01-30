#ifndef __WEBSERVER__
#define __WEBSERVER__

#include <WebServer.h>
#include "main.h"
#include "motor.h"

void setupServers();
void onConnectHandler();
void onWebSocketEvent(byte num, WStype_t type, uint8_t *payload, size_t length);
void broadcastData(const Data *);

#endif