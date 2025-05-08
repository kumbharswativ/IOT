# Air Quality Monitoring System

## Introduction

This project monitors air quality in real time by integrating sensor data with InfluxDB and Grafana. Data is continuously stored, aggregated via Flux queries, and visualized through interactive dashboards that reveal trends and threshold breaches. Proactive alerts send notifications when sensor readings exceed safe limits, providing timely insights for smart decision-making.

---

## Data Dictionary

The dataset collected from InfluxDB contains the following fields:

1. **_start (dtype: dateTime:RFC3339):**  
   The starting timestamp of the data range (configured to record the last 7 days of data).

2. **_stop (dtype: dateTime:RFC3339):**  
   The ending timestamp based on the InfluxDB settings.

3. **_time (dtype: dateTime:RFC3339):**  
   The actual time when the data was recorded or fetched by InfluxDB via the MQTT server.

4. **_value (dtype: double):**  
   The ADC (Analog-to-Digital Converter) reading value.

---

## Process Flow of the Project

1. **Configure the Arduino UNO R4 WIFI:**  
   Set up the board for establishing a WiFi connection and configure the MQTT broker settings for Mosquitto. We used an insecure connection because the UNO R4 doesn’t support the secure MQTT port 8883, which is the only option provided by the HiveMQ Cloud MQTT broker. To resolve this, we opted for the Mosquitto broker and used port 1884 instead, prioritizing project functionality over security.The sensor data is published with a delay of 5 seconds.

2. **Initiate the Mosquitto Broker:**  
   Launch the broker via the terminal by running:  
   ```
   mosquitto.exe -c "C:\Program Files\mosquitto\mosquitto.conf" -v
   ```

3. **Create the MQTT Subscriber Python Script:**  
   Develop a Python script (named `sub1.py`) using VS Code to subscribe to the MQTT topics and capture the data published by the UNO R4.

4. **Set Up the InfluxDB Container:**  
   - Create a container for InfluxDB named "airquality".  
   - Inside InfluxDB, create a bucket named "airquality0".  
   - Paste the API token of this bucket into your subscriber Python script.

5. **Establish the Connection Between InfluxDB and Grafana:**  
   Use the following command in the command prompt to verify connectivity:  
   ```
   wget -qO- http://host.docker.internal:8086/health
   ```

6. **Configure the Grafana Container:**  
   - Create a container for Grafana with the port mapping `3001:3000`.  
   - Configure the Grafana data source by providing the InfluxDB URL, the API token, and selecting the "airquality0" bucket.

7. **Set Up Alerts in Grafana:**  
   After creating basic visualizations, configure two alerts ("Sensor Disconnected" and "Poor Air Quality") using a Discord server webhook.

---

## Future Improvements
Currently, the sensor provides ADC readings rather than actual AQI values or precise VOC (Volatile Organic Compound) concentrations in ppm. For future improvements, functions that convert ADC readings to VOC/AQI values can be developed by referencing various websites and online conversion tools/formulae to produce more accurate and understandable readings from the sensor.
