#include "mQTT_Tds.h"

const char *ssid = "ZEUS_2G";
const char *password = "tito2000";
char msg_tds[MSG_BUFFER_SIZE];
extern PubSubClient * client;
mQTT_Tds::mQTT_Tds()
{
}

mQTT_Tds::~mQTT_Tds()
{
}
void mQTT_Tds::setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, "tito2000");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("CONECTADO AO WiFi");
  delay(10);
}
void mQTT_Tds::setDateTime()
{
  configTime(TZ_America_Sao_Paulo, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2)
  {
    delay(100);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println();
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.printf("%s %s", tzname[0], asctime(&timeinfo));
}
void mQTT_Tds::reconnect()
{
  // Loop until we’re reconnected
  while (!client->connected())
  {
    Serial.print("Attempting MQTT connection…");
    String clientId = "ESP8266Client - MyClient";
    // Attempt to connect
    // Insert your password
    // if (client->connect(clientId.c_str(), "YOUR_USERNAME", "YOUR_PASSWORD")) {
    if (client->connect(clientId.c_str(), "TitoUFU", "Tito1k58!"))
    {
      Serial.println("connected");

      // Once connected, publish an announcement…
      snprintf(msg_tds, MSG_BUFFER_SIZE, "%4.1f%%", 1414.0);
      client->publish("TdsTopic", msg_tds);
      // … and resubscribe
      client->subscribe("TdsTopic");
    }
    else
    {
      Serial.print("failed, rc = ");
      Serial.print(client->state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
