#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Include necessary helper files for Firebase
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "HiBro"
#define WIFI_PASSWORD "66666666"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCJJLR1U0pmmYySJ4GmWqJ2FqR5r3elyqg"

// Insert RTDB URL
#define DATABASE_URL "https://footmap-fbee6-default-rtdb.firebaseio.com/"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup(){
  // Initialize ADC-capable pins
  pinMode(13, INPUT);
  pinMode(12, INPUT);
  pinMode(14, INPUT);
  pinMode(32, INPUT);
  pinMode(35, INPUT);
  pinMode(34, INPUT);

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Assign the API key and database URL
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Sign up for Firebase
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Signed up successfully");
    signupOK = true;
  }
  else{
    Serial.printf("Sign-up error: %s\n", config.signer.signupError.message.c_str());
  }

  // Set token status callback function
  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  // Read analog values from ADC-capable pins
  int A = analogRead(13);
  int B = analogRead(12);
  int C = analogRead(14);
  int D = analogRead(32);
  int E = analogRead(35);
  int F = analogRead(34);

  // Check if Firebase is ready and if signup was successful
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)){
    // Update data every second
    sendDataPrevMillis = millis();

    // Send analog values to Firebase
    if (Firebase.RTDB.setInt(&fbdo, "piezo/1", A)){
      Serial.printf("Analog value from pin 13: %d\n", A);
    }
    else {
      Serial.println("Failed to read from pin 13");
      Serial.println("Reason: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setInt(&fbdo, "piezo/2", B)){
      Serial.printf("Analog value from pin 12: %d\n", B);
    }
    else {
      Serial.println("Failed to read from pin 12");
      Serial.println("Reason: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setInt(&fbdo, "piezo/3", C)){
      Serial.printf("Analog value from pin 14: %d\n", C);
    }
    else {
      Serial.println("Failed to read from pin 14");
      Serial.println("Reason: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setInt(&fbdo, "piezo/4", D)){
      Serial.printf("Analog value from pin 32: %d\n", D);
    }
    else {
      Serial.println("Failed to read from pin 32");
      Serial.println("Reason: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setInt(&fbdo, "piezo/5", E)){
      Serial.printf("Analog value from pin 35: %d\n", E);
    }
    else {
      Serial.println("Failed to read from pin 35");
      Serial.println("Reason: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setInt(&fbdo, "piezo/6", F)){
      Serial.printf("Analog value from pin 34: %d\n", F);
    }
    else {
      Serial.println("Failed to read from pin 34");
      Serial.println("Reason: " + fbdo.errorReason());
    }
  }
}
