#ifndef MQTT_TDS_H
#define MQTT_TDS_H
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <time.h>
#include <PubSubClient.h>
#include <time.h>
#include <TZ.h>
#include <FS.h>
#include <LittleFS.h>
#include <CertStoreBearSSL.h>
#define MSG_BUFFER_SIZE (500)

class mQTT_Tds
{
  public:
    mQTT_Tds();
    ~mQTT_Tds();
    void setup_wifi();
    void setDateTime();
    void reconnect();
  private:
};

#endif
