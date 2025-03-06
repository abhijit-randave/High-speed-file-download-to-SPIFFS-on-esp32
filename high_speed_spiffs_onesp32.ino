#include <WiFi.h>
#include <HTTPClient.h>
#include <SPIFFS.h>
const char* WIFI_ID = "Abhi";  //wifi name
const char* WIFI_PASS = "123456789"; //wifi pass
const char* FILE_URL = "https://www.dropbox.com/s/l9osup9mpsp1id2/SAVE_20220227_141157.jpg?st=a39vtr4b&dl=0"; //link which file is downlad

void setup() {
    Serial.begin(115200);
    delay(400);  
    Serial.println("Initialize SPIFFS...");
    //for spiffs
    if (!SPIFFS.begin(true)) {
        Serial.println("Error: SPIFFS  Failed");
        return;
    }

    Serial.print("Connecting to WiFi: ");
    //wifi initilize
    WiFi.begin(WIFI_ID, WIFI_PASS);

    int connectionAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && connectionAttempts < 15) {
        delay(400);
        Serial.print(".");
        connectionAttempts++;
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nWiFi Connection Failed! Check again");
        return;
    }
    Serial.println("\nWiFi Connected Successfully");
    Serial.println("Starting File Download...");
    HTTPClient http;
    //htpp initilize
    http.begin(FILE_URL);
    
    int httpResponseCode = http.GET();
    if (httpResponseCode == HTTP_CODE_OK) {  
      

        File file = SPIFFS.open("/downloaded_file.bin", FILE_WRITE);
        if (!file) {
            Serial.println("Error: Failed to create file!");
            return;
        }

        WiFiClient* stream = http.getStreamPtr();
        uint8_t buffer[4096]; 
        size_t totalBytesDownloaded = 0;
        unsigned long startTime = millis();

        while (http.connected()) {
            int bytesReceived = stream->read(buffer, sizeof(buffer));  
            if (bytesReceived > 0) {
                file.write(buffer, bytesReceived);
                totalBytesDownloaded += bytesReceived;

                if (totalBytesDownloaded % 8192 == 0) {
                    Serial.print("*");
                }
            } else {
                break;  
            }
        }

        file.close();
        Serial.println("\nDownload Completed!");

        unsigned long totalTime = millis() - startTime;
        float downloadSpeed = (totalBytesDownloaded / 1024.0) / (totalTime / 1000.0);
        
        Serial.print("Average Speed: ");
        Serial.print(downloadSpeed);
        Serial.println(" KB/s");

    } else {
        Serial.print("HTTP Error: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}

void loop() {
    delay(1200);//simple delay for wait
     }
