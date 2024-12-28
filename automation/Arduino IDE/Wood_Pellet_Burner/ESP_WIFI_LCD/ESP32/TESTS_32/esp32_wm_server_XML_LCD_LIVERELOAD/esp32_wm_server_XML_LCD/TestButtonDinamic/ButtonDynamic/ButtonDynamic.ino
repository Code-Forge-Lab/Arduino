/// CRASHING BROWSER WHEN SWITHING IP

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

// WiFi credentials
const char* ssid = "network_glass";
const char* password = "migtukas";

// Create an instance of the server
AsyncWebServer server(80);
AsyncWebSocket ws("/ws"); // WebSocket instance

// Button state
bool buttonState = false;

// Function to broadcast button state to all connected clients
void broadcastButtonState() {
  ws.textAll(buttonState ? "ON" : "OFF");
  // ws.textAll("ON");
}

// Handle WebSocket events
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                      AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    // Send the current button state to the new client
    client->text(buttonState ? "ON" : "OFF");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
  } else if (type == WS_EVT_DATA) {
    // Handle incoming WebSocket message
    String message = String((char*)data).substring(0, len);
    Serial.printf("Message received: %s\n", message.c_str());

    if (message == "TOGGLE") {
      // Toggle the button state
      buttonState = !buttonState;

      // Broadcast the new state to all connected clients
      broadcastButtonState();
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Serve a simple web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", R"rawliteral(
      <!DOCTYPE html>
      <html>
      <head>
        <title>ESP WebSocket</title>
        <style>
          #toggleButton {
            padding: 15px 30px;
            font-size: 16px;
            color: white;
            border: none;
            cursor: pointer;
            border-radius: 5px;
          }
          .on {
            background-color: green;
          }
          .off {
            background-color: red;
          }
        </style>
        <script>
          var socket;
          window.onload = function() {
            socket = new WebSocket('ws://' + location.host + '/ws');
            socket.onmessage = function(event) {
              var state = event.data;
              var button = document.getElementById('toggleButton');
              if (state === 'ON') {
                button.className = 'on';
              } else {
                button.className = 'off';
              }
            };
          };

          function toggleButton() {
            socket.send('TOGGLE');
          }
        </script>
      </head>
      <body>
        <h1>ESP WebSocket</h1>
        <button id="toggleButton" class="off" onclick="toggleButton()">Toggle Button</button>
      </body>
      </html>
    )rawliteral");
  });

  // Attach WebSocket event handler
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  // Start the server
  server.begin();
}

void loop() {
  // Simulate a state change in the microcontroller every 10 seconds
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 10000) {
    lastUpdate = millis();
    // buttonState = !buttonState; // Simulate state toggle
    Serial.printf("Microcontroller changed state to: %s\n", buttonState ? "ON" : "OFF");
    broadcastButtonState(); // Notify all clients of the change
  }
}
