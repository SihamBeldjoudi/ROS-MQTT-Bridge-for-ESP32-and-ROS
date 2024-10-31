#!/usr/bin/env python
import rospy                          # ROS library for creating and managing nodes
import time                           # Standard Python library for time operations
import paho.mqtt.client as mqtt       # MQTT library for connecting to MQTT brokers
from std_msgs.msg import String       # ROS message type for string data
from std_msgs.msg import Float64MultiArray  # ROS message type for arrays of floats
 

# Callback function for when a message is received on any subscribed MQTT topic
def on_message(client, userdata, message):
    print("message received", str(message.payload.decode("utf-8")))  # Decode and print the message payload
    print("message topic=", message.topic)      # Print the topic of the message
    print("message qos=", message.qos)          # Print the Quality of Service level
    print("message retain flag=", message.retain) # Print the retain flag status


# Callback function triggered when a ROS message is received on the 'messages' topic
def callback(t):
    message = t.data[:4]   # Extract the first four characters of the incoming message
    global x               # Declare `x` as a global variable so it can be accessed outside this function

    # Conditionals to check specific values of `message` and set `x` accordingly
    if message == '0013':
        x = "5501060060"
        send_message(x)    # Send `x` via MQTT and publish it to the ROS 'object' topic

    elif message == '0032':
        x = "0650060060"
        send_message(x)    # Send `x` via MQTT and publish it to the ROS 'object' topic

    else:
        print("Hello!")    # Print "Hello!" if `message` does not match the specified values
        

# Sets up a ROS node and subscriber to listen to 'messages' topic
def callback2():
    rospy.init_node('Sending_node', anonymous=True) # Initialize a new ROS node called 'Sending_node'
    rospy.Subscriber('messages', String, callback)  # Subscribe to the 'messages' topic, using `callback`
    rospy.spin()  # Keeps the node running to process incoming messages


# Publishes a message to both ROS and MQTT
def send_message(msg):
    pub = rospy.Publisher('object', String, queue_size=100) # Create a ROS publisher on the 'object' topic
    pub.publish(msg)      # Publish `msg` to the ROS topic
    client.publish('Detection', msg)  # Publish `msg` to the MQTT topic 'Detection'


# Setting up the MQTT client
broker_address = "10.42.0.1"        # Address of the MQTT broker
client = mqtt.Client("P1")          # Create an MQTT client instance with ID "P1"
client.on_message = on_message      # Link the `on_message` callback to this client

print("connecting...")
client.connect(broker_address)      # Connect to the MQTT broker
client.loop_start()                 # Start the MQTT client loop to process network events
client.subscribe("Detection")       # Subscribe to the MQTT topic 'Detection'
client.loop_stop()                  # Stop the MQTT loop

    
# Main program entry
if __name__ == '__main__':
    callback2()   # Run the `callback2` function to initialize the ROS subscriber
