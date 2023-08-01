# ESP-Smart-Meter

This project reads electricity and voltage data from an energy monitor sensor and sends it to Google Sheets through HTTP and to Node-RED through MQTT. We proposed adding an AI component to detect electricity theft.

## Prerequisites

- NodeMCU board
- AC Current sensor
- AC Voltage sensor
- Arduino IDE
- Adafruit MQTT library
- ESP8266WiFi library
- A Google account
- Jupyter Notebook with TensorFlow installed


![bd drawio (13)](https://user-images.githubusercontent.com/86083079/229686175-e321b83f-72dc-4abb-8789-0545452ef467.png)


## Prototype Setup

1. Wire the voltage and current sensors to the NodeMCU board.
2. Connect the NodeMCU board to your computer via USB.
3. Install the Adafruit MQTT, WiFiManager, and ESP8266HTTPClient libraries by going to `Sketch > Include Library > Manage Libraries` in the Arduino IDE, and searching for each library.
4. Create a Google Sheets document and obtain a Google Sheets API key and spreadsheet ID. 
   - Follow the instructions on this [Google Sheets API documentation](https://developers.google.com/sheets/api/quickstart/python) to create a Google Sheets API key.
   - Create a new Google Sheets document and copy the document ID from the URL. (e.g. https://docs.google.com/spreadsheets/d/DOCUMENT_ID/edit)
   - Paste the document ID into the `GoogleSheets.h` file where indicated.
5. Connect the NodeMCU board to your WiFi network.
   - The NodeMCU board will create an access point (AP) named "NodeMCU Energy Monitor" when it cannot find the saved WiFi credentials.
   - Connect to the "NodeMCU Energy Monitor" AP, then enter your WiFi credentials.
6. Start Node-RED on your computer and create a new MQTT input node. 
   - Set the MQTT input node topic to "energy-monitor/data".
   - Configure the MQTT input node to output to a chart or dashboard.

## Jupyter Notebook setup

1. Open Jupyter Notebook on your computer.
2. Create a new Python 3 notebook.
3. Install the necessary libraries


## Upload the code

1. Open the `energy_monitor.ino` file in the Arduino IDE.
2. Edit the code to include your WiFi network name and password, as well as your MQTT broker IP address and credentials.
3. Click the `Upload` button in the top-left corner of the IDE.

## Testing

1. The NodeMCU board will connect to your WiFi network and the MQTT broker.
2. The energy monitor sensor will start measuring electricity and voltage.
3. The NodeMCU board will send data to Google Sheets and Node-RED every minute.
   - The Google Sheets data will be stored in the sheet named "Energy Monitor Data".
   - The Node-RED data will be sent to the MQTT input node and displayed on your chart or dashboard.

## Troubleshooting

If you see an error message while uploading the code, make sure that the correct board and port are selected in the Arduino IDE. If you're still having trouble, check the [NodeMCU forum](https://www.esp8266.com/viewforum.php?f=13) for help.
