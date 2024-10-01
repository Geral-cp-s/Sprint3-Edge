<p align='center' >
  <img width="250px" loading="lazy" src = "https://github.com/Geral-cp-s/Sprint-Edge/assets/110639916/aa204473-bba7-4dc2-8db1-ea5744b8e9bc"/>
</p>
<h1 align="Center">🏎️ Race Lap Time Capture System with ESP32 and PIR Sensor</h1>

# Table of Contents
* [1 - Project Description](#descricao)
* [2 - Basic Architecture for IoT Projects](#arquitetura)
* [3 - Architecture and Components](#componentes)
  * [3.1 - Phisycal Components](#componentes1)
  * [3.2 - Comunication](#componentes2)
  * [3.3 - Software](#componentes3)
* [4 - Functionality](#funcionalidades)
  * [4.1 - Lap Capture](#funcionalidades1)
  * [4.2 - Storage and Calculation](#funcionalidades2)
  * [4.3 - Communication and Feedback](#funcionalidades3)
  * [4.4 - Race Completion](#funcionalidades4)
* [5 - System Configuration](#config)
  * [5.1 - Network Settings](#config1)
  * [5.2 - HTTP Endpoints](#config2)
  * [5.3 - Hardware Specifications](#config3)
* [6 - Available Statistics](#estatisticas)
* [7 - Implementation Steps](#passo-passo)
  * [7.1 - Implementation Steps](#passo-passo1)
  * [7.2 - Implementation Steps](#passo-passo2)
* [8 - Possible Improvements and Expansions](#melhorias)
* [9 - License](#licenca)
* [10 - Video](#video)
* [11 - Code](#codigo)
* [12 - Access to the Simulator](#simulador)
* [13- Authors](#autores)


<h2 id="Descricao">Project Description</h2>
<p>This project implements a lap time capture system for racing, using an ESP32, a PIR sensor for motion detection, and MQTT for data communication. It allows for the registration, storage, and real-time visualization of detailed lap statistics.</p>

<h2 id="arquitetura">Basic Architecture for IoT Projects</h2>
<p>IoT projects typically involve several key layers:</p>
<ol>
  <li><strong>Devices Layer:</strong>Physical devices like sensors and actuators collect data or perform actions.</li>
    <ul>
      <li>In this project: An ESP32 is used with a PIR sensor to detect laps and a buzzer for feedback.</li>
    </ul>
  <li><strong>Connectivity Layer:</strong> Devices communicate via protocols like Wi-Fi and MQTT to transmit data.</li>
    <ul>
      <li>In this project: The ESP32 connects to the internet via Wi-Fi, and MQTT is used to send lap times to a broker.</li>
    </ul>
  <li><strong>Data Processing Layer:</strong> Data can be processed locally or in the cloud.</li>
    <ul>
      <li>In this project: Lap times are calculated on the ESP32 and also published to the MQTT broker.</li>
    </ul>
  <li><strong>Application Layer:</strong> Users interact with the system through dashboards or apps.</li>
    <ul>
      <li>In this project: A dashboard displays real-time race statistics.</li>
    </ul>
    <img width="600px" loading="lazy" src = "https://github.com/user-attachments/assets/0007ff04-e78f-44a3-896f-95444fbd1508"/>


</ol>

<h2 id="Componentes">🛠️ Architecture and Components</h2> 
<h3 id="componentes1">Physical Components</h3> 
<ul> 
  <li>ESP32: Microcontroller that manages the sensors and communication with the MQTT server and the display.</li> 
  <li>PIR Sensor: Detects the passage of competitors (simulating the finish line).</li> 
  <li>LCD I2C Display (16x2): Displays information about the race, such as the average lap times.</li> 
  <li>Buzzer: Provides an audible indication of a new lap record.</li> 
  <li>RTC DS3231: Maintains the record of date and time.</li> 
</ul> 
<h3 id="componentes2">Communication</h3> 
<ul> <li>PIR Sensor: Detects movement as competitors pass the finish line, simulating the recording of a lap.</li> 
  <li>MQTT (Message Queuing Telemetry Transport): A lightweight communication protocol that allows lap times to be published to an MQTT broker.</li> 
  <li>Web Server (HTTP): An embedded HTTP server in the ESP32 allows querying of lap times in JSON format.</li>
</ul> 
<h3 id="componentes3">Software</h3> 
<ul> 
  <li>C++/Arduino Framework: The system logic is implemented using the C++ language in the Arduino framework, with specific libraries for ESP32, MQTT, and hardware control.</li> 
</ul>

<h2 id="funcionalidades">⚙️ System Operation</h2>
<h3 id="funcionalidades1">1. Lap Capture</h3>

- `Feature 1` <strong>PIR Sensor:</strong> Detects movement as competitors pass the finish line, simulating the recording of a lap.

- `Feature 2` <strong>State Logic:</strong> The current and previous states of the PIR sensor are compared to prevent multiple consecutive recordings of the same lap.

<h3 id="funcionalidades2">2. Storage and Calculation</h3>

- `Feature 1` <strong>Time Recording:</strong> Lap times are stored in an array, limited to 10 laps per race.

- `Feature 2` <strong>Real-Time Calculations:</strong> The average lap time is continuously recalculated and displayed on the LCD. The total race time is also computed.

- `Feature 3` <strong>Statistics:</strong>

<ul> 
  <li><strong>Best Lap:</strong> The shortest recorded time.</li>
  <li><strong>Worst Lap:</strong> The longest recorded time.</li>
  <li><strong>Average Laps:</strong> The sum of times divided by the number of completed laps.</li> 
</ul> 
<h3 id="funcionalidades3">3. Communication and Feedback</h3>

- `Feature 1`<strong>MQTT Publication:</strong> Each completed lap's time is published to the MQTT topic /race/laps.

- `Feature 2` <strong>Audible Feedback:</strong> A buzzer emits a sound for each recorded lap.

- `Feature 3` <strong>HTTP Server:</strong> Accessible locally via the ESP32's IP, the server provides the race status in JSON format.

<h3 id="funcionalidades4">4. Race Completion</h3>

- `Feature 1` <strong>Race Finished:</strong> After recording 10 laps, the system automatically ends the race, stopping the capture of new laps and resetting the counter.

   
<h2 id="config">📡 System Configuration</h2> 
<h3 id="config1">Network Settings</h3> 
<p>The connection to the Wi-Fi network and the MQTT broker is configured through the constants in the code:</p>
<img width="600px" loading="lazy" src = "https://github.com/user-attachments/assets/161b1ecc-85fd-4ce7-a4ee-323e4076fc8d"/>

<h3 id="config2">HTTP Endpoints</h3> 
<p>The system provides an HTTP endpoint that returns the race data in JSON format:</p> 
<ul> 
  <li><strong>Endpoint:</strong>/race_status</li> 
  <li><strong>Example Response:</strong></li>
  <img width="600px" loading="lazy" src = "https://github.com/user-attachments/assets/61773e65-bc80-465c-afc8-8f541cb2fca9"/>
</ul>

<h3 id="config3">Hardware Specifications</h3> 
<ul> 
  <li><strong>ESP32:</strong> Dual-core processor with built-in Wi-Fi.</li>
  <li><strong>PIR Sensor:</strong> Detects movement based on changes in infrared radiation.</li>
  <li><strong>LCD I2C:</strong> Simplified communication interface for data display.</li>
  <li><strong>BUZZER:</strong> Emits a sound signal when a lap is recorded.</li> 
</ul>

<h2 id="estatisticas">📊 Available Statistics</h2> 
<ul> 
  <li><strong>Lap Times:</strong> Each individual recorded time in the format seconds.milliseconds.</li> 
  <li><strong>Total Race Time:</strong> Sum of all lap times.</li>
  <li><strong>Average Lap Time:</strong> Average of the times for each lap.</li>
  <li><strong>Best Lap:</strong> The shortest time recorded during the race.</li>
  <li><strong>Worst Lap:</strong> The longest time recorded during the race.</li> 
</ul>

<h2 id="passo-passo">🚀 Implementation Steps</h2>
<h3 id="passo-passo1">Software Requirements</h3> 
<ul> 
  <li><strong>Arduino IDE:</strong> To program and upload the code to the ESP32.</li>
  <li><strong>Arduino Libraries:</strong></li> <ul> <li>WiFi.h: For Wi-Fi connection.</li>
    <li>PubSubClient.h: For MQTT communication.</li> 
    <li>LiquidCrystal_I2C.h: For controlling the LCD display.</li> 
    <li>WebServer.h: For the HTTP server.</li>
  </ul>
</ul> 
<h3 id="passo-passo2">Configuration Steps</h3> 
<ol> 
  <li><strong>Hardware:</strong> Connect the ESP32, PIR sensor, LCD display, and buzzer according to the wiring diagram.</li> 
  <li><strong>Configure Wi-Fi and MQTT:</strong> Enter the SSID, Wi-Fi password, MQTT broker address, and port in the code.</li>
  <li><strong>Upload the Code:</strong> Compile and upload the code to the ESP32 using the Arduino IDE.</li> 
  <li><strong>Serial Monitor:</strong> Check the output in the serial monitor to confirm the connection to Wi-Fi and the MQTT broker.</li>
  <li><strong>Access Web Server:</strong> Use the IP of the ESP32 to access the /race_status endpoint and view the data in JSON format.</li> 
  <li><strong>Dashboard Integration:</strong> Ensure your dashboard is configured to connect to the same MQTT broker and subscribe to the /race/laps topic to receive real-time data.</li>
</ol>

<h2 id="melhorias">🔧 Possible Improvements and Expansions</h2> 
<ul>
  <li><strong>Dashboard Integration:</strong> Expand the dashboard functionality to include graphs, lap history, and real-time visualization of race data.</li> 
  <li><strong>Sensor Accuracy Improvements:</strong> Implement more precise or additional sensors, such as phototransistors or digital timers.</li>
  <li><strong>Permanent Storage:</strong> Implement a solution for permanent data storage (e.g., remote database storage).</li> 
  <li><strong>Support for Multiple Competitors:</strong> Adapt the code to support more than one competitor simultaneously.</li> 
</ul> 

<h2 id="licenca">📄 License</h2> 
<p>This project is available under the MIT License. Feel free to use, modify, and distribute it as needed.</p>

<h2 id="video">Video</h2>
https://youtu.be/WkrpSNXZ43Y

<h2 id="codigo">Code</h2> 
<p>Upload the code to your microcontroller. The code includes the initialization of the sensors, data reading, and display on the LCD, as well as logic to trigger alerts based on the detected amount of rain and to measure lap times.</p>
https://github.com/Geral-cp-s/Sprint3-Edge/blob/main/code.ino

<h2 id="Simulador">Access to the Simulator</h2>
https://wokwi.com/projects/410188165632536577

<h2 id="Autores">Authors</h2>

<div align="center">
  
| [<img loading="lazy" src="https://github.com/gvqsilva/CP2-Edge/assets/110639916/d022ed18-0057-4944-9e00-db796c6d2e45" width=115><br><sub>Gabriel Vasquez</sub>](https://github.com/gvqsilva)  |  [<img loading="lazy" src="https://github.com/gvqsilva/CP2-Web/assets/110639916/1eb7df1a-c0e8-4170-aabf-444cfb3c64f9" width=115><br><sub>Guilherme Araujo</sub>](https://github.com/guilhermearaujodec)  |  [<img loading="lazy" src="https://github.com/gvqsilva/CP2-Edge/assets/110639916/86514492-2b1e-4422-bdc0-0ec3c8be3dcc" width=115><br><sub>Augusto Douglas</sub>](https://github.com/gutomend)  |  [<img loading="lazy" src="https://github.com/gvqsilva/CP2-Edge/assets/110639916/4bb3084d-d1ff-4b49-ba37-96c8046f6e14" width=115><br><sub>Gustavo Oliveira</sub>](https://github.com/Gusta346) |
| :---: | :---: | :---: | :---: |

<ul>
  <li>Gabriel Vasquez - RM: 557056</li>
  <li>Guilherme Araujo - RM: 558926</li>
  <li>Gustavo Oliveira - RM: 559163</li>
  <li>Augusto Mendonça - RM: 558371</li>
</ul><br>

<p>Happy racing and good luck with your implementation! 🚗💨</p>
</div>

