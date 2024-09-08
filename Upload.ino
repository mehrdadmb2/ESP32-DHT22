#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DHT.h>
#include <WiFi.h>
#include <time.h>
#include <WebServer.h>

#define DHTPIN 6
#define DHTTYPE DHT22
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define I2C_SDA 5
#define I2C_SCL 4
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

const char* ssidList[] = { "Null", "Null", "Null" };
const char* passwordList[] = { "Null", "Null", "Null" };
const int wifiCount = sizeof(ssidList) / sizeof(ssidList[0]);

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WebServer server(80);

bool isRGBOn = true;  // وضعیت اولیه RGB روشن است
struct tm timeinfo;

void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("OLED allocation failed"));
    while (true);
  }

  displayMessage("Starting...");
  dht.begin();
  connectToWiFi();
  setup_time();

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/info", handleInfo);
  server.on("/toggleRGB", handleToggleRGB);
  server.begin();
}

void loop() {
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 900) {
    lastUpdate = millis();
    DHTData();
    if (getLocalTime(&timeinfo)) {
      displayInfo();
    } else {
      Serial.println(F("Failed to update time"));
    }
    RGBtemp();
  }
  server.handleClient();
}

void DHTData() {
  float h = dht.readHumidity();
  float Temp = dht.readTemperature();

  if (!isnan(h) && !isnan(Temp)) {
    Serial.print(F("Temp: "));
    Serial.println(Temp);
    Serial.print(F("Humidity: "));
    Serial.println(h);
  } else {
    displayMessage("Failed to read DHT sensor!");
  }
}

void handleRoot() {
  String html = "<!DOCTYPE html>"
                "<html lang='en'>"
                "<head>"
                "<meta charset='UTF-8'>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                "<title>ESP32 Weather Station</title>"
                "<style>"
                "body {font-family: Arial, sans-serif; background-color: #f0f0f0; text-align: center; padding: 20px;}"
                "h1 {color: #333;}"
                ".card {background-color: #fff; color: #333; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); display: inline-block;}"
                ".temp, .humidity {font-size: 2em;}"
                ".info {font-size: 1em; margin-top: 20px;}"
                "</style>"
                "</head>"
                "<body>"
                "<h1>Weather Station</h1>"
                "<div class='card'>"
                "<h2 class='temp'>Temperature: <span id='temp'>--</span>&deg;C</h2>"
                "<h2 class='humidity'>Humidity: <span id='humidity'>--</span>%</h2>"
                "<div class='info'>"
                "<p>WiFi SSID: <span id='ssid'>--</span></p>"
                "<p>Time: <span id='time'>--</span></p>"
                "<p>Public IP: <span id='ip'>--</span></p>"
                "<p>Ping: <span id='ping'>--</span> ms</p>"
                "</div>"
                "</div>"
                "<button onclick='toggleRGB()'>Toggle RGB</button>"
                "<script>"
                "function updateBackgroundColor(temp) {"
                "let bgColor;"
                "if (temp <= 23.9) {"
                "bgColor = '#add8e6';"
                "} else if (temp <= 27) {"
                "bgColor = '#90ee90';"
                "} else {"
                "bgColor = '#ff6347';"
                "}"
                "document.body.style.backgroundColor = bgColor;"
                "}"
                "function toggleRGB() {"
                "fetch('/toggleRGB').then(response => response.json()).then(data => {"
                "console.log('RGB Status:', data.rgb_status);"
                "});"
                "}"
                "setInterval(function() {"
                "fetch('/data').then(response => response.json()).then(data => {"
                "document.getElementById('temp').innerText = data.temperature;"
                "document.getElementById('humidity').innerText = data.humidity;"
                "updateBackgroundColor(data.temperature);"
                "});"
                "fetch('/info').then(response => response.json()).then(data => {"
                "document.getElementById('ssid').innerText = data.ssid;"
                "document.getElementById('time').innerText = data.time;"
                "document.getElementById('ip').innerText = data.ip;"
                "document.getElementById('ping').innerText = data.ping;"
                "});"
                "}, 1000);"
                "</script>"
                "</body>"
                "</html>";
  server.send(200, "text/html", html);
}

void handleData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  String json = "{\"temperature\":" + String(temperature) +
                ", \"humidity\":" + String(humidity) + "}";
  server.send(200, "application/json", json);
}

void handleInfo() {
  String ip = WiFi.localIP().toString();
  String ping = getPing();
  String ssid = WiFi.SSID();
  String time = String(timeinfo.tm_year + 1900) + "-" +
                String(timeinfo.tm_mon + 1) + "-" +
                String(timeinfo.tm_mday) + " " +
                String(timeinfo.tm_hour) + ":" +
                String(timeinfo.tm_min) + ":" +
                String(timeinfo.tm_sec);

  String json = "{\"ssid\":\"" + ssid +
                "\", \"time\":\"" + time +
                "\", \"ip\":\"" + ip +
                "\", \"ping\":\"" + ping + "\"}";
  server.send(200, "application/json", json);
}

void handleToggleRGB() {
  isRGBOn = !isRGBOn;
  String json = "{\"rgb_status\":" + String(isRGBOn ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

void setup_time() {
  displayMessage("Setup Time Waiting for NTP time sync...");
  configTime(3 * 3600 + 30 * 60, 0, "pool.ntp.org");
  Serial.println(F("Waiting for NTP time sync..."));

  unsigned long start = millis();
  while (!getLocalTime(&timeinfo) && millis() - start < 10000) {
    delay(100);
  }

  if (!getLocalTime(&timeinfo)) {
    Serial.println(F("Failed to obtain time"));
    displayMessage("Failed to obtain time");
  } else {
    Serial.println(&timeinfo, "Time set to: %A, %B %d %Y %H:%M:%S");
  }
  delay(500);
}

void displayMessage(const char* message) {
  Serial.println(message);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(message);
  display.display();
}

void displayInfo() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("Temp: "));
  display.print(dht.readTemperature());
  display.print(F(" C"));
  display.setCursor(0, 8);
  display.print(F("Humidity: "));
  display.print(dht.readHumidity());
  display.print(F("%"));

  display.setCursor(0, 16);
  display.printf("Date: %02d/%02d/%04d WiFi:", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);

  display.setCursor(0, 24);
  display.printf("Time: %02d:%02d:%02d %s",
                 timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec,
                 (WiFi.status() == WL_CONNECTED) ? WiFi.SSID().c_str() : "Disconnected");
  display.display();
}

void RGBtemp() {
  if (!isRGBOn) return;

  // حالت‌های مختلف دما و رطوبت
  if (dht.readTemperature() <= 10) {
    if (dht.readHumidity() < 30) {
      setRGB(0, 0, RGB_BRIGHTNESS / 2);  // آبی تیره
    } else {
      setRGB(0, 0, RGB_BRIGHTNESS);  // آبی روشن
    }
  } else if (dht.readTemperature() <= 20) {
    setRGB(0, RGB_BRIGHTNESS, RGB_BRIGHTNESS);  // فیروزه‌ای
  } else if (dht.readTemperature() <= 30) {
    if (dht.readHumidity() < 50) {
      setRGB(0, RGB_BRIGHTNESS, 0);  // سبز
    } else {
      setRGB(RGB_BRIGHTNESS, RGB_BRIGHTNESS, 0);  // زرد
    }
  } else if (dht.readTemperature() <= 35) {
    setRGB(RGB_BRIGHTNESS, RGB_BRIGHTNESS / 2, 0);  // نارنجی
  } else {
    setRGB(RGB_BRIGHTNESS, 0, 0);  // قرمز
  }
}

void setRGB(uint8_t r, uint8_t g, uint8_t b) {
#ifdef RGB_BUILTIN
  neopixelWrite(RGB_BUILTIN, r, g, b);
#endif
}

void connectToWiFi() {
  displayMessage("Connecting to WiFi...");
  for (int i = 0; i < wifiCount; i++) {
    Serial.print(F("Connecting to WiFi SSID: "));
    Serial.println(ssidList[i]);
    displayMessage(ssidList[i]);
    WiFi.begin(ssidList[i], passwordList[i]);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
      delay(500);
      Serial.print(F("."));
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.println(F("Connected!"));
      displayMessage("Connected!");
      return;
    }

    Serial.println();
    Serial.println(F("Failed to connect!"));
  }

  displayMessage("No WiFi connection");
  Serial.println(F("No WiFi connection!"));
}

String getPing() {
  WiFiClient client;
  IPAddress remote_ip(8, 8, 8, 8);  // Google DNS server IP
  unsigned long start = millis();

  if (client.connect(remote_ip, 80)) {
    unsigned long duration = millis() - start;
    client.stop();
    return String(duration);
  } else {
    return "Failed";
  }
}
