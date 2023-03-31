#include <ESP8266WiFi.h>
#include "EmonLib.h"  // Include Emon Library
EnergyMonitor emon1;  // Create an instance

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <PubSubClient.h>



//----------------------------------------------------------------------------------------------
const char* mqtt_server = "192.168.137.227";


// ----------------------------------------------------------------------------------------------
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ADHAM";
char pass[] = "adham123";
String GAS_ID = "AKfycbxrwXo9Y1ob0XrHmuY4CoEnKXriHKltzP8tQAHALs6KyZQHsC9K4XUp1TL2crg0fvJJpg"; //--> spreadsheet script ID

//Your Domain name with URL path or IP address with path
const char* host = "script.google.com"; // only google.com not https://google.com

// ----------------------Google Sheets Timer Variable Specification Variables----------------------------------------

#define UPDATE_INTERVAL_HOUR  (0)
#define UPDATE_INTERVAL_MIN   (0)
#define UPDATE_INTERVAL_SEC   (5)

#define UPDATE_INTERVAL_MS    ( ((UPDATE_INTERVAL_HOUR*60*60) + (UPDATE_INTERVAL_MIN * 60) + UPDATE_INTERVAL_SEC ) * 1000 )

// ----------------------------------------------------------------------------------------------

float Volt;
float power;
float Bill;
double Irms;

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in the system

//Creates a client that can connect to to a specified internet IP address and port as defined in client.connect().
WiFiClient espClient;
//MQTT client
PubSubClient client(espClient);



// ----------------------------Function to update google sheets with values------------------------------
void update_google_sheet()
{
    Serial.print("connecting to ");
    Serial.println(host);
  
    // Initializes the https client. 
    //Creates a client that can connect to to a specified internet IP address and port as defined in client.connect().
    WiFiClientSecure client;
    
    // 80 is for HTTP / 443 is for HTTPS!
    const int httpPort = 443; 
    
    // this is disaples the certificate check needed stage in the https handshaking framework 
    client.setInsecure(); 
    
    if (!client.connect(host, httpPort)) { 
      Serial.println("connection failed");
      return;
    }
       
    //-----Processing data and sending data--------
    String url = "/macros/s/" + GAS_ID + "/exec?power=";
   
    url += String(power);
    
    url += "&bill=";
    url += String(Bill);
    
    Serial.print("Requesting URL: ");
    Serial.println(url);
  
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
  
    Serial.println();
    Serial.println("closing connection");  
}

//--------------------------------------------------------------------------------

void callback(char* topic, byte* payload, unsigned int length) {

}



//----------------------------to ensure connectivity to the broker------------------------------
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//------------------------------------------------------------------------





// ----------------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);

  Volt = 220.0;
  

  //
  emon1.current(A0, 111.1);  // Current: input pin, calibration.
  

  

//----------------------------------------Wait for connection
  Serial.print("Connecting");
  WiFi.begin(ssid, pass); //--> Connect to your WiFi router
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.println("Alert Wifi Not Connected");

    delay(2000);
  }

   ////mqtt broker adress and port
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// ----------------------------------------------------------------------------------------------
unsigned long time_ms;
unsigned long time_1000_ms_buf;
unsigned long time_sheet_update_buf;
unsigned long time_dif;
unsigned long time_dif1;

void loop()
{

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  
  StaticJsonDocument<64> doc;
  char output[55];


  // Update data to google sheet in specific period
  time_ms = millis();
  time_dif = time_ms - time_sheet_update_buf;  
  if ( time_dif >= UPDATE_INTERVAL_MS ) // Specific period
  {
    
    time_sheet_update_buf = time_ms;

  Irms = 0;
  Irms += emon1.calcIrms(1480);  // Calculate Irms current only

  power = (Irms * 220)/1000;


  /////billing condional system based on which segment your power consumption is
  if (power < 50){
    Bill = power * 0.48 ;
    }
    else if (power > 50 && power < 100){
      Bill = power * 0.58;
      }
      else if ( power > 100 && power < 200) {
        Bill = power * 0.77;
        }
    

  Serial.print("Current: ");
  Serial.print(Irms);
  Serial.println(" A");

  Serial.print("Power: ");
  Serial.print(power);
  Serial.println(" Kwatt");

  Serial.print("Billing Price: ");
  Serial.print(Bill);
  Serial.println(" EGP");



    update_google_sheet();
    
  }




  time_ms = millis();
  time_dif1 = time_ms - time_1000_ms_buf;

  // Read and print serial data every 1 sec
  if ( time_dif1 >= 1000 ) // 1sec 
  {
    
    //specifying keys and value in the json document
    doc["I"] = Irms;
    doc["V"] = Volt;
    doc["P"] = power;
    
    Serial.println("Read");

    /////function to serialize json document
    serializeJson(doc, output);
    Serial.println(output);
    client.publish("/home/Power", output);
    Serial.println("Sent");
  }

  

  
 
}///--------The End of The Void Loop-------///
