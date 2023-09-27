import json
import paho.mqtt.client as mqtt

config_file = "config.json"


def load_config():
    try:
        with open(config_file, 'r') as file:
            return json.load(file)
    except FileNotFoundError:
        return {}


def save_config(config):
    with open(config_file, 'w') as file:
        json.dump(config, file)


config = load_config()
session_number = config.get("session_number", 0)
session_enabled = False


def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    client.subscribe("gps/metric/enable")
    client.subscribe("gps/metric/disable")


def on_message(client, userdata, msg):
    topic = msg.topic
    message = msg.payload.decode("utf-8")
    handle_message(topic, message)


def handle_message(topic, message):
    global session_number
    global session_enabled

    if topic == "gps/metric/enable":
        if not session_enabled:
            session_number += 1
            session_enabled = True
            config["session_number"] = session_number
            save_config(config)

           
            payload = session_number
            payload_json = json.dumps(payload)
            mqtt_client.publish("gps/metric/enable", payload)

    elif topic == "gps/metric/disable":
        if session_enabled:
            session_enabled = False
            save_config(config)


mqtt_client = mqtt.Client()

mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message

mqtt_client.connect("192.168.0.123", 1883)
mqtt_client.loop_forever()
