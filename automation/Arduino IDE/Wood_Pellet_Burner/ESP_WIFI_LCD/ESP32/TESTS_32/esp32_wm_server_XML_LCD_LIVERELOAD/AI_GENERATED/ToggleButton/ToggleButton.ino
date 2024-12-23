#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "network_glass";
const char* password = "migtukas";

AsyncWebServer server(80);

// Variable to store the burner state
String burnerState = "OFF";

void setup() {
  // Start Serial Monitor
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", R"rawliteral(
      <!DOCTYPE html>
      <html>
      <head>
          <title>Burner Control</title>
          <style>
              #burnerButton {
                  padding: 15px 30px;
                  font-size: 18px;
                  font-weight: bold;
                  border: none;
                  border-radius: 8px;
                  cursor: pointer;
                  transition: background-color 0.3s ease, color 0.3s ease;
              }

              .off {
                  background-color: red;
                  color: white;
              }

              .on {
                  background-color: green;
                  color: white;
              }
          </style>
          <script>
              function toggleBurner(button) {
                  if (button.classList.contains("off")) {
                      button.classList.remove("off");
                      button.classList.add("on");
                      button.innerText = "Burner ON";
                      sendBurnerState("ON");
                  } else {
                      button.classList.remove("on");
                      button.classList.add("off");
                      button.innerText = "Burner OFF";
                      sendBurnerState("OFF");
                  }
              }

              function sendBurnerState(state) {
                  fetch(`/burner?state=${state}`)
                      .then(response => response.text())
                      .then(data => console.log("ESP32 Response:", data))
                      .catch(error => console.error("Error:", error));
              }
          </script>
      </head>
      <body>
          <h1>Burner Control</h1>
          <button id="burnerButton" class="off" onclick="toggleBurner(this)">Burner OFF</button>
      </body>
      </html>
    )rawliteral");
  });

  // Handle the burner state update
  server.on("/burner", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("state")) {
      burnerState = request->getParam("state")->value();
      Serial.println("Burner State: " + burnerState);
      request->send(200, "text/plain", "Burner State Updated to " + burnerState);
    } else {
      request->send(400, "text/plain", "Missing state parameter");
    }
  });

  // Start server
  server.begin();
}

void loop() {
  // No loop processing needed
}
