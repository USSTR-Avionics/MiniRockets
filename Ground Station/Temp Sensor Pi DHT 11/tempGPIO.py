from datetime import datetime
import Adafruit_DHT
import time
import json
import csv

DHT_SENSOR = Adafruit_DHT.DHT11
DHT_PIN = 4
TIMEOUT = 3

try:
    with open("settings.json","r") as fobj:
        settings = json.load(fobj)
        fobj.close()

    DHT_PIN = settings["PIN"]
    TIMEOUT = settings["TIMEOUT"]

except FileNotFoundError:
    print("reverting to default settings")

except KeyError:
    print("reverting to default settings")

while True:

    humidity, temperature = Adafruit_DHT.read(DHT_SENSOR, DHT_PIN)
    now = datetime.now()
    dateStr = now.strftime("%d/%m/%Y, %H:%M:%S")
    strFMT = dateStr + " " + f"temp : {temperature}C humd : {humidity}%"
    csvFMT = [dateStr,temperature,humidity]

    if humidity is not None and temperature is not None:
        print(strFMT)
    else:
        print("Sensor failure. Check wiring.")

    with open("data_temp_humd.csv","a") as fobj:
        writer = csv.writer(fobj)
        writer.writerow(csvFMT)
        fobj.close()

    time.sleep(TIMEOUT)
