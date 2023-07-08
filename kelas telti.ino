#include "PubSubClient.h"
#include "ESP8266WiFi.h"
#include "DHT.h"


const char* ssid = "didin";
const char* password = "12345satu";
#define TOKEN "jAvF0VVPZBPpfU9zEsKe" //Ambil dari device thingsboard di atas
char ThingsboardHost[] = "demo.thingsboard.io";

// #define sensor 14 
#define sensor_hujan 13  //Deklarasi Sensor hujan pada pin D5 NodeMcu
#define DHTPIN 14
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

WiFiClient wifiClient;
PubSubClient client(wifiClient);
int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup() 
{
  lastSend = 0;
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("connected to");
  Serial.println(ssid);
  client.setServer( ThingsboardHost, 1883 );

  
  Serial.println("Sensor Suhu dan Kelembaban Udara DHT11");
  delay(1000);

  pinMode (sensor_hujan, INPUT); //Deklarasi sensor hujan sebagai input
 
}
 
void loop()
{
if ( !client.connected() ) 
{
    reconnect();
}

if ( millis() - lastSend > 5000 ) { // kirim data suhu tiap 1 detik
    getAndSendTemperatureAndHumidityData();
    lastSend = millis();
  }
  
}

void getAndSendTemperatureAndHumidityData()
{
  Serial.println("Collecting temperature data.");

  // baca suhu dan kelembaban
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println("Sending data to ThingsBoard:");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C ");
 
  // Prepare a JSON payload string
  String payload = "{";
 payload += "\"humidity\":";
 payload += String(humidity); 
 payload += ",";
 payload += "\"temperature\":";
 payload += String(temperature); 
  payload += "}";

  char attributes[1000];
  payload.toCharArray( attributes, 1000 );
  client.publish( "v1/devices/me/telemetry",attributes);
  Serial.println( attributes );
   
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("Esp8266", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.println( " : retrying in 5 seconds]" );
      delay( 500 );
    }
  }
}