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
* MQTT
* ASP .NET
* Node.JS RED
	
## Setup
* MQTT broker, private or public
* ESP8266 Module
*GNSS module
* ESP

## Backend
* connect to wi-fi - done 
* connect to broker
* connect to GNSS module
* subscribe to topic
* publish info about GNSS module:
          * Latitude
          * Longitude
          * Time 
          * Speed
          * Number of sattelites
* use 2 modules at once           
## Frontend 
* select technology (ASP or nodered)
* subscribe to MQTT topics about GNSS info
* write dynamicly charts about data

## Tests 
* tests will be done in city and nearby forests
* they will show the accurancy of gnss modules 
* will work out about improving accuracy by using 2 modules at once 
