# Testing environment for GNSS modules

## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Requirements](#setup)
* [Backend To-Do list](#backend)
* [Frontend To-Do list](#frontend)
* [Testing](#tests)
## General info
* Project for Engineer Degreee on Zielona Góra University Information Technology
* Under supervisement of D.Eng Michał Doligalski 
	
## Technologies
Project is created with:
* Arduino IDE
* Mosquitto
* MQTT
* Node.JS RED
* Grafana
* InfluxDB
	
## Hardware Requirements
* MQTT broker, private or public
* ESP8266 Module
* GNSS module
## Software Requirements
* Latest docker version
* ability to open ports on your network where containters are running. This step is neccesary when you want to work outside your local network

## Arduino program abilities
* Connects to selected Wi-Fi
* Connects to selected MQTT broker (TLS 1.2 is enabled, no certs required)
* Connects to 1 GNSS module 
* When connected to broker, sends topic with ESP module MAC Address to topic "esp/macaddress/+" where "+" is MAC address of device
* Subscibes to "esp/gps/start" - topic that sends payload 0 or 1 depends if user wants to turn off or on sending gps data
* Sends data from GNSS module 
          * Latitude 
          * Longitude
          * Time 
	  * Altitude
          * Speed
          * Number of sattelites         
## Frontend 
* WebService based on Node-RED 
* Grafana plugin that can visualize data from unlimited numbers of modules
* InfluxDB database to store and work on data for further tests

## Docker Compose File

User can pull and run both preconfigured containers using 
```
docker compose up
```
Docker will automatically pull neccesary containers and creates new network 

If u re ecountrering error that says 

```
failed to create network gnsstest_test: Error response from daemon: Pool overlaps with other one on this address space
```

use command 

```
docker network prune

```
## Tests 
* tests will be done in city and nearby forests
* they will show the accurancy of gnss modules 
* will work out about improving accuracy by using 2 modules at once 
