#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Arduino.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define API_KEY "AIzaSyCRoYkf6x5XOACULlLusDwVwzLekGCSld8"
#define DATABASE_URL "https://atl-dimapur-default-rtdb.asia-southeast1.firebasedatabase.app"
#define USER_EMAIL "temjen92.jamir@gmail.com"
#define USER_PASSWORD "646h3tfe64rdthesdrfy"

const int Pins[] = {4, 5, 12, 13, 14};

FirebaseData stream;
FirebaseAuth auth;
FirebaseConfig config;

volatile bool dataChanged = false;

void streamCallback(FirebaseStream data)
{
  dataChanged = true;
}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}

void setup()
{
  Serial.begin(115200);
  for (int_fast8_t i = 0; i < sizeof(Pins); i++)
  {
    pinMode(Pins[i], OUTPUT);
  }

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  config.token_status_callback = tokenStatusCallback;
  Firebase.reconnectNetwork(true);

  stream.setBSSLBufferSize(2048, 1024);

  // Connect to Wi-Fi
  WiFi.begin("000", "applemango");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  if (!Firebase.RTDB.beginStream(&stream, "/home/switches"))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
}

void loop()
{
  Firebase.RTDB.runStream();

  if (dataChanged)
  {
    dataChanged = false;
    String data = stream.stringData();
    const char *inputString = data.c_str();

    if (strlen(inputString) == 5)
    {
      for (int i = 0; i < 5; ++i)
      {
        digitalWrite(Pins[i], inputString[i] == '1' ? HIGH : LOW);
        Serial.printf("Pin: %d is %d #  \n", Pins[i], digitalRead(Pins[i]));
      }
    }
    else
    {
      Serial.println("Invalid data length");
    }
    Serial.print("=========== Executed\n");
  }
  delay(50);
}