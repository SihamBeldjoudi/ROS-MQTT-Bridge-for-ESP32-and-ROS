This project establishes a bridge between ROS (Robot Operating System) and MQTT (Message Queuing Telemetry Transport) protocol, allowing ESP32 devices to communicate with ROS nodes through an MQTT broker. It enables message exchange between the ESP32 microcontroller and ROS, facilitating control and monitoring of devices connected to ROS from MQTT-enabled devices.

## Project Structure

This repository contains:
- **ESP32 Code** (written in C++): Connects the ESP32 to a WiFi network and MQTT server. It subscribes to topics, receives messages from the ROS system, and processes specific actions based on message contents.
- **Python Code**: Sets up a ROS node that subscribes to the `messages` ROS topic and publishes to an MQTT broker, forwarding messages to the ESP32 over the `Detection` MQTT topic. This code also listens for incoming MQTT messages and relays them to ROS.

## Features

- **Two-Way Communication**: Allows ESP32 to receive ROS messages over MQTT and take actions based on message content.
- **Customizable Message Handling**: Configurable message handling in Python to allow specific responses based on message content.
- **MQTT Broker Connection**: The ESP32 and Python ROS node both connect to a common MQTT broker, enabling smooth communication.
- **ROS Integration**: Uses `std_msgs` for message exchange in ROS, making it easy to integrate with existing ROS projects.

## Prerequisites

- **ESP32 microcontroller** with WiFi capability.
- **MQTT Broker**: Ensure an MQTT broker (such as Mosquitto) is set up and accessible to the ESP32 and ROS machine.
- **ROS Installed** on the machine where the Python script is running.
- **Libraries**:
  - ESP32 requires the `WiFi.h` and `PubSubClient.h` libraries.
  - Python script requires `paho-mqtt` for MQTT communication and `rospy` for ROS integration.

## Setup

### 1. ESP32 Setup

1. Flash the ESP32 code onto your ESP32 microcontroller.
2. Ensure you have WiFi credentials and the MQTT broker IP in the code:
   ```cpp
   const char* ssid = "your_network_ssid";
   const char* password = "your_network_password";
   const char* mqtt_server = "mqtt_broker_ip";
   ```
3. Upload the code to ESP32 via the Arduino IDE or another compatible uploader.

### 2. Python ROS Node Setup

1. Install the required Python libraries:
   ```bash
   pip install paho-mqtt rospy
   ```
2. Update the `broker_address` in the Python script to match the MQTT broker IP.
3. Run the Python ROS node script:
   ```bash
   python ros_mqtt_bridge.py
   ```

### 3. Running ROS Nodes

Start the ROS environment and make sure your topics are correctly set up. Run the ROS node by executing the Python script (`ros_mqtt_bridge.py`), which will begin subscribing to and publishing messages.

## Code Overview

### ESP32 Code (C++)

- **WiFi and MQTT Setup**: Connects ESP32 to WiFi and subscribes to the `Detection` MQTT topic.
- **Callback Processing**: When a message is received on the `Detection` topic, the ESP32 processes it to determine specific actions.
- **Reconnection Logic**: Ensures the ESP32 reconnects to the MQTT server if the connection is lost.

### Python ROS Node Code

- **on_message() Function**: Callback to display details of any received MQTT message, used for logging/debugging.
- **callback() Function**: Processes messages received on the ROS `messages` topic. If the message matches specified values, it publishes a predefined response to both ROS and MQTT topics.
- **send_message() Function**: Publishes messages to both the MQTT `Detection` topic and the ROS `object` topic.
- **Main Execution**: Sets up the ROS node and subscriber, allowing the script to continuously listen and respond.

## Usage Example

1. **Send Messages from ROS to ESP32**: Publish a message on the `messages` topic in ROS (for example, `0013`), and the ESP32 will receive and process it.
2. **Receive MQTT Messages in ROS**: The ESP32 can also send messages to ROS via the MQTT broker, which are then processed by the ROS node.

### Sample Commands

- **Publishing in ROS**:
   ```bash
   rostopic pub /messages std_msgs/String "0013"
   ```
   - This command will trigger a response in the Python script and publish a message back to `Detection` MQTT topic, which the ESP32 will then receive and process.

## Troubleshooting

- **MQTT Connection Issues**: Verify the broker IP, ensure both ESP32 and ROS machine are connected to the network, and check for firewall or router issues.
- **Message Not Received**: Confirm correct topic subscription, both in MQTT and ROS, and check for any network delays or errors in the broker logs.

