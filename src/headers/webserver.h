#ifndef WEBSERVER_IMPL_H
#define WEBSERVER_IMPL_H

#include <WiFi.h>
#include <WebServer.h>

extern bool grab, useXYZ;
extern float v0, v1, v2;

extern float X, Y, Z;
extern float base, elbow, wrist;

#ifdef USE_WEBSERVER_STANDALONE
void setup();
void loop();
#endif

void initWebServer();
void handleWebServer();
void unpackData();

#endif