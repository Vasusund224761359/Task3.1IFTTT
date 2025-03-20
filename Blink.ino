#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

const char* ssid = "DODO-166E";//my wifi name 
const char* password = "EJTZG2WUNN";// mywifi passcode

const char* host = "maker.ifttt.com";
String eventName = "light_detector"; //the name of my event in IFTTT
String key = "pBmZvrQk_m3Evri4ug-bL";

int lightSensorPin = A0;  
int threshold = 420;  //As soon as light crosses this value, the niotifiction is sent 
bool sunlightOn = false;

void ensureWiFiConnected() {// this funtion is made to ensure the wifi connection.
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("connecting to Wi-Fi...");
        WiFi.disconnect();
        WiFi.begin(ssid, password);
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 10) {
            delay(1000);
            Serial.print(".");
            attempts++;
        }
        if (WiFi.status() == WL_CONNECTED) {//if the wifi is connected print out cnnected 
            Serial.println("Connected!");
        } else {
            Serial.println("Failed to reconnect."); //if wifi fails to connect
        }
    }
}

void sendIFTTTNotification(String event) { // function that is used for triggering the IFTTT
    ensureWiFiConnected(); // first wifi will connect, 
= 
    WiFiClient wifi;
    HttpClient client = HttpClient(wifi, host, 80);

    String url = "/trigger/" + event + "/with/key/" + key;// this is the full webhook url from IFTTT
    Serial.println("Sending request to: " + url);

    client.get(url);
    int statusCode = client.responseStatusCode();
    //200 means the successfull sending of trigger (CNC) 
    Serial.print("Response Code: ");
    Serial.println(statusCode);

    client.stop();
}

void setup() {
    Serial.begin(9600);
    ensureWiFiConnected();
}

void loop() {
    int lightValue = analogRead(lightSensorPin); // for reading the value of the light from the ligt sensor
    Serial.print("Light Sensor Value: ");
    Serial.println(lightValue);

    if (lightValue > threshold && !sunlightOn) {
        sendIFTTTNotification(eventName); //this function is run to send a trigger and I receive a notification.
        sunlightOn = true;
    } 
    else if (lightValue <= threshold && sunlightOn) {
        sendIFTTTNotification("sunlight_stopped");
        sunlightOn = false;
    }

    delay(10000);  // Reduce to 10 seconds for testing
}
