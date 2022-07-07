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
* Node.JS RED
	
## Setup
* MQTT broker, private or public
* ESP8266 Module
*GNSS module
* ESP

## Backend
* connect to wi-fi - done 
* connect to broker -done 
* connect to GNSS module -done
* subscribe to topic -done but not in use atm 
* publish info about GNSS module:
          * Latitude - done 
          * Longitude
          * Time 
          * Speed
          * Number of sattelites
* use 2 modules at once           
## Frontend 
* select technology - done NODERED
* subscribe to MQTT topics about GNSS info -done for 1 topic atm 
* write dynamicly charts about data -done for 1 topic atm 
* use auth 
* save data about sent data to database to store them or show 


## Tests 
* tests will be done in city and nearby forests
* they will show the accurancy of gnss modules 
* will work out about improving accuracy by using 2 modules at once 
