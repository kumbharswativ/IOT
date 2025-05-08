import time
from paho.mqtt import client as mqtt_client
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

# MQTT configuration
mqtt_broker = "10.0.0.80"
mqtt_port = 1884         # Ensure this matches your Mosquitto configuration port
mqtt_topic = "sensor/air_quality"

# InfluxDB 2 configuration
influxdb_url = "http://localhost:8086"  # Adjust if necessary
influxdb_token = "lJeCu5bYScCcsRiScnu-ii_zw6kC_qjj8CE-HG06n7dQLLg-puP2sy-hVWqfhtf4AxH89TRD_2kc-c7hgl1dQg=="  # Replace with your token
influxdb_org = "rowan"                  # Replace with your organization name
influxdb_bucket = "airquality0"         # This bucket must exist or be created

# Initialize the InfluxDB 2 client and its write API.
influx_client = InfluxDBClient(url=influxdb_url, token=influxdb_token, org=influxdb_org)
write_api = influx_client.write_api(write_options=SYNCHRONOUS)

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT broker!")
        client.subscribe(mqtt_topic)
    else:
        print("MQTT connection failed with code", rc)

def on_message(client, userdata, msg):
    payload = msg.payload.decode()
    print(f"Received: {payload} from topic: {msg.topic}")
    try:
        value = float(payload)
    except ValueError:
        print("Payload is not a valid number.")
        return

    # Create a data point and write it to InfluxDB
    point = Point("sensor_values").field("value", value).time(int(time.time()), WritePrecision.S)
    write_api.write(bucket=influxdb_bucket, record=point)
    print("Data written to InfluxDB")

# Create the MQTT client using the MQTTv311 protocol.
mqtt_client_instance = mqtt_client.Client(client_id="PythonSubscriber", protocol=mqtt_client.MQTTv311)
mqtt_client_instance.on_connect = on_connect
mqtt_client_instance.on_message = on_message

mqtt_client_instance.connect(mqtt_broker, mqtt_port, keepalive=60)
mqtt_client_instance.loop_forever()
