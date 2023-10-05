import time
import paho.mqtt.client as paho
import random
import serial

broker = "broker.emqx.io"
client = paho.Client("client-245")


def on_message(client, userdata, message):
    time.sleep(1)
    data = str(message.payload.decode("utf-8"))
    print("received message =", data)
    if data:
        if data in ["u", "d"]:
            resp = send_command(data, responses[data])
            time.sleep(1)
    match data:
        case "u":
            mes = "led on"
        case "d":
            mes = "led off"
        case _:
            mes = "Неверно"
    client.publish("house/dasha", mes)
    time.sleep(1)


def send_command(message: str, resp_length: int) -> str:
    connection.write(message.encode())
    if resp_length > 0:
        response = connection.read(resp_length)
        return response.decode("utf-8")
    return ""


responses = {'u': 6,
             'd': 7}
port = "COM7"
connection = serial.Serial(port, 9600, timeout=1)

client.on_message = on_message
print("Connecting to broker", broker)
client.connect(broker)
client.loop_start()
print("Subcribing")
client.subscribe("house/tata")
time.sleep(30)
client.disconnect()
client.loop_stop()
