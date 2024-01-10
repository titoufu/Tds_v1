#include "Tds_class.h"
#include "mQTT_Tds.h"
float averageVoltage = 0, tdsValue = 0, temperature = 25;
const char *mqtt_server = "27ea24e7656546e9b7acaf07c705308f.s1.eu.hivemq.cloud";
//const char *mqtt_server = "bdc89afc1d5c447e8cbd83f65005162d.s2.eu.hivemq.cloud";
BearSSL::CertStore certStore;
WiFiClientSecure espClient;
unsigned long lastMsg = 0;

int value = 0;

Tds_class Tds;
mQTT_Tds Mqs;
PubSubClient *client;
void setup()
{

  Serial.begin(115200);
  pinMode(TdsCalibraPin, OUTPUT);
  pinMode(TdsComunicaPin, OUTPUT);
  pinMode(TdsSensorPin, INPUT);
  Tds.begin(); // initialization

  Mqs.setup_wifi();
  Mqs.setDateTime();

  ////
  LittleFS.begin();
  int numCerts = certStore.initCertStore(LittleFS, PSTR("/certs.idx"), PSTR("/certs.ar"));
  Serial.printf("Number of CA certs read: %d\n", numCerts);
  if (numCerts == 0)
  {
    Serial.printf("No certs found. Did you run certs-from-mozilla.py and upload the LittleFS directory before running?\n");
    return; // Can't connect to anything w/o certs!
  }

  BearSSL::WiFiClientSecure *bear = new BearSSL::WiFiClientSecure();
  // Integrate the cert store with this connection
  bear->setCertStore(&certStore);

  client = new PubSubClient(*bear);

  client->setServer(mqtt_server, 8883);
  client->setCallback(callback);
}
void callback(char *topic, byte *payload, unsigned int length)
{

  Serial.print("Mensagem recebida do ");
  Serial.print(topic);
  const char *stringConhecida = "hello world";
  char charArray[length + 1]; // Cria um array de caracteres ( +1 para o caractere nulo de terminação)
  memcpy(charArray, payload, length);
  charArray[length] = '\0';        // Adicionar o caractere nulo de terminação
  String payloadString(charArray); // Cria uma String a partir do array de caracteres
  Serial.println(" => " + payloadString);
  // if (payloadString.equals(stringConhecida))
  // {
  //   Serial.println("O payload é igual à string conhecida.");
  // }
  // else
  // {
  //   Serial.println("O payload é diferente da string conhecida.");
  // }
}

void loop()
{
  static char msg_tds[MSG_BUFFER_SIZE];
  if (!client->connected())
  {
    Mqs.reconnect();
  }
  client->loop();
  unsigned long now = millis();
  if (now - lastMsg > 10000)
  { // atualiza a cada 10s
    lastMsg = now;
    srand(time(NULL));
    // temperature = readTemperature();  //add your temperature sensor and read it

    Tds.setTemperature(temperature); // set the temperature and execute temperature compensation
    Tds.update();                    // sample and calculate
    tdsValue = Tds.getTdsValue() + rand() % 11;    // then get the value
    snprintf(msg_tds, MSG_BUFFER_SIZE, "%4.1f mS/cm", tdsValue);
    client->publish("TdsTopic", msg_tds);
    Serial.println();
  }
}
