$mqttHost=$args[0]
$mqttPort=$args[1]
$mqttUser=$args[2]
$mqttPassword=$args[3]
$wifiSsid=$args[4]
$wifiPassword=$args[5]
$unichar = $([char]0x0022)
 (Get-Content -path ./.env -Raw) -replace '(\bMQTT_HOST\b)(.*)',"MQTT_HOST=$mqttHost" |  Set-Content ./.env
 (Get-Content -path ./.env -Raw) -replace '(\bMQTT_PORT\b)(.*)',"MQTT_PORT=$mqttPort" |  Set-Content ./.env
 (Get-Content -path ./.env -Raw) -replace '(\bMQTT_USER\b)(.*)',"MQTT_USER=$mqttUser" |  Set-Content ./.env
 (Get-Content -path ./.env -Raw) -replace '(\bMQTT_PWD\b)(.*)',"MQTT_PWD=$mqttPassword" |  Set-Content ./.env

 (Get-Content -path ./gps_TestEnv/gps_TestEnv.ino -Raw) -replace '(\bmqtt_broker\b = )(.*)',"mqtt_broker = $unichar$mqttHost$unichar;" |  Set-Content ./gps_TestEnv/gps_TestEnv.ino 
 (Get-Content -path ./gps_TestEnv/gps_TestEnv.ino -Raw) -replace '(\bmqtt_port\b = )(.*)',"mqtt_port = $mqttPort;" |  Set-Content ./gps_TestEnv/gps_TestEnv.ino 
 (Get-Content -path ./gps_TestEnv/gps_TestEnv.ino -Raw) -replace '(\bmqtt_uname\b = )(.*)',"mqtt_uname = $unichar$mqttUser$unichar;" |  Set-Content ./gps_TestEnv/gps_TestEnv.ino 
 (Get-Content -path ./gps_TestEnv/gps_TestEnv.ino -Raw) -replace '(\bmqtt_passwd\b =)(.*)',"mqtt_passwd = $unichar$mqttPassword$unichar;" |  Set-Content ./gps_TestEnv/gps_TestEnv.ino 
 (Get-Content -path ./gps_TestEnv/gps_TestEnv.ino -Raw) -replace '(\bssid\b )(.*)',"ssid $unichar$wifiSsid$unichar" |  Set-Content ./gps_TestEnv/gps_TestEnv.ino 
 (Get-Content -path ./gps_TestEnv/gps_TestEnv.ino -Raw) -replace '(\bpassword\b )(.*)',"password $unichar$wifiPassword$unichar" |  Set-Content ./gps_TestEnv/gps_TestEnv.ino 