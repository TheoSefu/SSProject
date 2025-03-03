#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "esp_camera.h"

// Configurația WiFi
const char* ssid = "Numele_Retelei";
const char* password = "Parola_Retelei";

// Configurația MQTT
const char* mqtt_server = "broker.hivemq.com";  // Schimbă cu adresa brokerului tău
const int mqtt_port = 8883;
const char* mqtt_topic = "esp32/camera";

// Certificate pentru mTLS
const char* ca_cert = "-----BEGIN CERTIFICATE-----\n...\n-----END CERTIFICATE-----";
const char* client_cert = "-----BEGIN CERTIFICATE-----\n...\n-----END CERTIFICATE-----";
const char* client_key = "-----BEGIN PRIVATE KEY-----\n...\n-----END PRIVATE KEY-----";

WiFiClientSecure espClient;
PubSubClient client(espClient);

#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    21
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27

#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      19
#define Y4_GPIO_NUM      18
#define Y3_GPIO_NUM      5
#define Y2_GPIO_NUM      4
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

void initCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    
    if(psramFound()) {
        config.frame_size = FRAMESIZE_UXGA;
        config.jpeg_quality = 10;
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }
    
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Eroare inițializare cameră: 0x%x", err);
        return;
    }
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConectat la WiFi");
}

void connectToMQTT() {
    espClient.setCACert(ca_cert);
    espClient.setCertificate(client_cert);
    espClient.setPrivateKey(client_key);
    
    client.setServer(mqtt_server, mqtt_port);
    while (!client.connected()) {
        Serial.print("Conectare la MQTT...");
        if (client.connect("ESP32_Camera")) {
            Serial.println("Conectat!");
        } else {
            Serial.print("Eșec, rc=");
            Serial.print(client.state());
            Serial.println(" Se reîncearcă în 5 secunde...");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    initCamera();
    connectToWiFi();
    connectToMQTT();
    client.publish(mqtt_topic, "ESP32 Camera conectată!");
}

void loop() {
    client.loop();
}
