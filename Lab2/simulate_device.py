import paho.mqtt.client as mqtt
import base64

with open("test_image.jpg", "rb") as image_file:
    encoded = base64.b64encode(image_file.read()).decode()

client = mqtt.Client()
client.connect("localhost", 1883, 60)
client.publish("device/images", encoded)
client.disconnect()
