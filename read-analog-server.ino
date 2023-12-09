#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Bil"; 
const char* password = "123456789"; 
const int analogPin = A0; 

AsyncWebServer server(80); 
int sensorValue = 0; 
float voltage = 0.0; 

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    String html = "<html><body>";
    html += "<h1>Analog Value and Voltage</h1>";
    html += "<table border='1'><tr><th>Analog Value</th><th>Voltage</th></tr>";
    html += "<tbody id='dataRows'></tbody>"; // Body for data rows
    html += "</table>";
    html += "<script>setInterval(function() {fetch('/readSensor').then(response => response.text()).then(data => {const [sensorVal, voltageVal] = data.split(','); appendData(sensorVal, voltageVal)})}, 2000);";
    html += "function appendData(sensorVal, voltageVal) {";
    html += "const tableRef = document.getElementById('dataRows');";
    html += "const newRow = tableRef.insertRow();";
    html += "const cell1 = newRow.insertCell(0);";
    html += "const cell2 = newRow.insertCell(1);";
    html += "cell1.innerText = sensorVal;";
    html += "cell2.innerText = voltageVal + 'V';";
    html += "}</script>";
    html += "</body></html>";
    request->send(200, "text/html", html);
  });



  server.on("/readSensor", HTTP_GET, [](AsyncWebServerRequest * request) {
    String values = String(sensorValue) + "," + String(voltage, 2);
    request->send(200, "text/plain", values);
  });

  server.begin();
}

void loop() {
  sensorValue = analogRead(analogPin); 
  voltage = sensorValue * (3.3 / 1023.0); 

  Serial.print("Analog Value: ");
  Serial.print(sensorValue);
  Serial.print(", Voltage: ");
  Serial.print(voltage);
  Serial.println("V");

  delay(2000); 
}
