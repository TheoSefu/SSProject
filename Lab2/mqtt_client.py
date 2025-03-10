import paho.mqtt.client as mqtt
import base64
from datetime import datetime
import os
from database import save_image_metadata

IMAGE_DIR = "images/"
os.makedirs(IMAGE_DIR, exist_ok=True)

def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker with result code " + str(rc))
    client.subscribe("device/images")

def on_message(client, userdata, msg):
    print("🔔 MQTT message received!")
    payload = msg.payload.decode()
    device_id = msg.topic.split('/')[1]
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")

    filename = f"{device_id}_{timestamp}.jpg"
    filepath = os.path.join(IMAGE_DIR, filename)

    with open(filepath, "wb") as f:
        f.write(base64.b64decode(payload))

    save_image_metadata(device_id, filename, timestamp)

def start_mqtt_client():
    client = mqtt.Client()
    print("🔔 test")
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect("localhost", 1883, 60)
    client.loop_start()
