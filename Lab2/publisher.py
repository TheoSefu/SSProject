import paho.mqtt.client as mqtt

with open("image.jpg", "rb") as f:
    image_bytes = f.read()

client = mqtt.Client()
client.connect("localhost", 1883, 60)
client.publish("device/images", image_bytes)
client.disconnect()