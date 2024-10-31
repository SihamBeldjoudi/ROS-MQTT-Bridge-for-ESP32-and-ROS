#include <WiFi.h>                 // Library for WiFi connection
#include <PubSubClient.h>         // Library for MQTT protocol

// WiFi and MQTT server credentials
const char* ssid = "ubuntu";       // Name of the WiFi network to connect to
const char* password =  "znvtGU7r"; // WiFi password
const char* mqtt_server = "10.42.0.1"; // IP address of the MQTT server (ROS server)
const char* name_device = "ESP32"; // Device name, used as client ID in MQTT

WiFiClient wifiClient;             // WiFi client to manage the connection
PubSubClient client(wifiClient);   // MQTT client based on the WiFi connection
long lastMsg = 0;                  // Variable to store the last message time
char msg[50];                      // Buffer for MQTT messages (not used in this code)

// ESP32 initialization
void setup() {
 
  WiFi.mode(WIFI_STA);             // Sets ESP32 to station (client) mode for WiFi
 
  Serial.begin(115200);            // Initializes serial communication at 115200 baud
 
  WiFi.begin(ssid, password);      // Connects ESP32 to WiFi using SSID and password

  // Waits in a loop until ESP32 is connected to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);                    // Pauses 500ms between each check
  }
 
  Serial.println("Connected to the WiFi network"); // Confirmation of WiFi connection

  // Set up the MQTT server and the callback function
  client.setServer(mqtt_server, 1883); // Sets the MQTT server address and port (default: 1883)
  client.setCallback(callback);        // Links the callback function to handle incoming MQTT messages
}
 
// Callback function executed each time an MQTT message is received
void callback(char* topic, byte* message, unsigned int length) {
 
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);                 // Prints the topic of the received message
  Serial.print(" Message from a ROS Node: ");
  String messageTemp;                  // Variable to store the received message content

  // Loop to read each character of the message and store it in `messageTemp`
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);    // Prints the character to the console
    messageTemp += (char)message[i];   // Adds the character to `messageTemp`
  }
  Serial.println();

  // Processes the received message if the topic is "Detection"
  if (String(topic) == "Detection") {
    int h = (messageTemp[0] - '0') + (messageTemp[1] - '0'); // Adds first two characters as integers
    int r = (messageTemp[1] - '0') + (messageTemp[2] - '0'); // Adds characters in position 1 and 2 as integers

    // If `h` is 10, prints "H" to the serial port
    if (h == 10) {
       Serial.println("H");
    }
    // If `r` is 8, prints "R" to the serial port
    else if (r == 8) { 
       Serial.println("R");
    }
  }
}

// Function to handle MQTT reconnection
void reconnect() {
  // Loops until connection is re-established
  while (!client.connected()) {
    Serial.print("Attempting connection...");
    if (client.connect(name_device)) {      // Attempts connection with `name_device` as the client ID
       Serial.println("connected to a ROS Node");
       client.subscribe("Detection");       // Subscribes to the "Detection" topic to receive messages
    } else {
      Serial.print(client.state());         // Prints the current state if connection fails
      delay(5000);                          // Wait 5 seconds before trying to reconnect
    }
  }
}

// ESP32 main loop
void loop() {

  // Checks if MQTT client connection is active
  if (!client.connected()) {
    reconnect();                            // Reconnects if the connection is lost
  }
  client.loop();                            // Runs the MQTT loop to handle receiving and sending messages

  long now = millis();                      // Gets the current time in milliseconds
  if (now - lastMsg > 5000) {               // Checks if 5 seconds have passed since the last message
    lastMsg = now;                          // Updates `lastMsg` with the current time
  }
}
