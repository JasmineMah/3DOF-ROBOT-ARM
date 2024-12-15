#ifndef WEBSERVER_IMPL_H
#define WEBSERVER_IMPL_H

// Switch the below directive to DISABLE_WEBSERVER_STANDALONE if we 
// aren't debugging the webserver standalone.
// Switch the below directive to USE_WEBSERVER_STANDALONE, and change
// the PlatformIO environment to webserver_debugging IF we want to debug the webserver itself.
#define DISABLE_WEBSERVER_STANDALONE


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