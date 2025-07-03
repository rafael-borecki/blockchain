#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT22.h>

#include <WiFi.h>
#include <PubSubClient.h>

// WiFi Config
const char* ssid = "";      // Coloque aqui o seu wifi
const char* password = "";  // Coloque aqui a sua senha do wifi

// MQTT Config
const char* mqtt_server = "blockchain.borecki.xyz"; // Coloque seu servidor aqui
const int mqtt_port = 1883;         
const char* mqtt_topic = "sensor/1337/data";
const int sensor_id = 1337;     // !! Altere o ID do seu dispositivo !!

// DHT Sensor Config
#define DHTPIN 4        // Pino digital do DHT
DHT22 dht(DHTPIN); 

// OLED Display Config
#define SCREEN_WIDTH 128 // OLED display width
#define SCREEN_HEIGHT 64 // OLED display height
#define OLED_RESET    -1 // Reset pin #
#define SCREEN_ADDRESS 0x3C // Direto do datasheet 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //

WiFiClient espClient;
PubSubClient client(mqtt_server, mqtt_port, espClient);

unsigned long lastMsg = 0;
#define MSG_PUBLISH_INTERVAL 5000 // Intervalo entre cada publish

long timezone = -3;
byte daysavetime = 0;

void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32ClientTEST";

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  // Inicializar display OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  Serial.println(F("OLED display initialized."));

  display.clearDisplay(); 
  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0,0); 
  display.println("Connecting WiFi..."); 
  display.display(); 

  setup_wifi();

  display.clearDisplay();
  display.setCursor(0,0);
  display.println("WiFi Connected!");
  display.print("IP: ");
  display.println(WiFi.localIP());
  display.display();
  delay(1000);

  configTime(timezone * 3600, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");

  struct tm timeinfo;
  Serial.print("Waiting for time synchronization");
  while (!getLocalTime(&timeinfo, 5000)) {
    Serial.print(".");
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\nWiFi disconnected during time sync. Restarting...");
      delay(1000);
      ESP.restart();
    }
  }
  Serial.println("\nTime synchronized.");

  char timeStr[64];
  strftime(timeStr, sizeof(timeStr), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  Serial.println(timeStr);
}

void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }

  client.loop(); 

  unsigned long now = millis();
  if (now - lastMsg > MSG_PUBLISH_INTERVAL) {

    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm tmstruct;
    localtime_r(&tv.tv_sec, &tmstruct);

    char datetime_part[20];
    strftime(datetime_part, sizeof(datetime_part), "%Y-%m-%dT%H:%M:%S", &tmstruct);

    char timestamp[30];
    sprintf(timestamp, "%s.%06ld", datetime_part, tv.tv_usec);

    char current_time[20];
    strftime(current_time, sizeof(current_time), "%H:%M:%S", &tmstruct);

    Serial.println(timestamp);

    delay(1000);

    lastMsg = now;

    float t = dht.getTemperature(); 
    float h = dht.getHumidity();    

    if (isnan(h) || isnan(t)) { 
      Serial.println(F("Failed to read from DHT sensor!")); 
      display.clearDisplay(); 
      display.setCursor(0,0); 
      display.println("DHT Read Error"); 
      display.display(); 
      return; 
    }

    Serial.print(F("Humidity: ")); 
    Serial.print(h); 
    Serial.print(F("%  Temperature: ")); 
    Serial.print(t);
    Serial.println(F("°C")); 

    //Display on OLED
    display.clearDisplay(); 
    display.setCursor(0,0); 

    display.setTextSize(2); 
    display.print(t, 1);    
    display.print(" ");     
    display.setTextSize(1); 
    display.cp437(true);    
    display.write(248);     
    display.setTextSize(2); 
    display.print("C");     

    display.setCursor(0, 20); 
    display.setTextSize(1);   
    display.print("Humidity: ");
    display.print(h, 1);      
    display.println("%");     

    display.setCursor(0, 40);
    display.print("MQTT: ");
    display.println(client.connected() ? "OK" : "FAIL");

    display.setCursor(60,40);
    display.print(current_time);

    display.display();

    String payload = "{";
    payload += "\"sensor_id\": " + String(sensor_id) + ","; 
    payload += "\"temperature_c\": " + String(t, 2) + ","; 
    payload += "\"humidity\": " + String(h, 2) + ",";
    payload += "\"latitude\": " + String("22") + ",";
    payload += "\"longitude\": " + String("35") + ",";
    payload += "\"timestamp\": \"" + String(timestamp) + "\"";
    payload += "}";

    Serial.print("Publishing message: ");
    Serial.println(payload);

    if (client.publish(mqtt_topic, payload.c_str())) {
      Serial.println("Message published successfully");
    } else {
      Serial.println("Failed to publish message");
    }
  }
}
