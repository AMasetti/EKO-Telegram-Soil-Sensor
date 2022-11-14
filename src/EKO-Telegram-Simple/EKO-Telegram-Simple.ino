#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Configuraciones de WiFi
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_SSID_PASS"
// Configuraciones de Telegram BOT
#define BOT_TOKEN "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
#define CHAT_ID "123456789"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Nos conectamos a la red Wifi
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setTrustAnchors(&cert);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);

  int humedad = analogRead(A0);

  Serial.println(humedad);

  bot.sendMessage(CHAT_ID, "EKO Bot arrancó con exito 🪴", "");

  bot.sendMessage(CHAT_ID, "Viendo que tal esta el suelo 👀", "");
  
}

int alerta_enviada = 0;
int startup = 1;

void loop() {

  // Leemos la humedad de suelo desde el pin analogico (rango de 0 a 1024 siendo <600 sumergido en agua)
  int humedad = analogRead(A0);
  Serial.println(humedad);

  // Regamos con mucha agua
  if ( humedad < 600 &&  (!alerta_enviada || startup)) {
    alerta_enviada = 1 ;
    startup = 0;
    bot.sendMessage(CHAT_ID, "Me regaste de más, me voy a morir 🥺", "");
  }

  // La tierra esta seca
  if ( humedad > 1000 && (!alerta_enviada || startup)) {
    alerta_enviada = 1 ;
    startup = 0;
    bot.sendMessage(CHAT_ID, "Regame por favor 🙏", "");
  }
  

  // Rango normal de humedad, solo alertamos si previamente no lo hicimos
  if ( humedad > 700 && humedad < 900 && (alerta_enviada || startup)) {
    alerta_enviada = 0 ;
    startup = 0;
    bot.sendMessage(CHAT_ID, "Todo piola, gracias por cuidarme 🙌", "");
  }

  delay(1000);
}
