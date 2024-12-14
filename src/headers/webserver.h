#ifndef WEBSERVER_IMPL_H
#define WEBSERVER_IMPL_H

#include <WiFi.h>
#include <WebServer.h>

extern bool grab, useXYZ;
extern float v0, v1, v2;

#ifdef USE_WEBSERVER_STANDALONE
void setup();
void loop();
#endif

void initWebServer();
void handleWebServer();

#endif