const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Pellet Burner Control</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            padding: 20px;
            background-color: #f7f7f7;
        }
        .card {
            background: #fff;
            border: 1px solid #ddd;
            border-radius: 10px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
            margin: 10px auto;
            padding: 20px;
            width: 300px;
        }
        .card h1 {
            font-size: 24px;
            margin: 10px 0;
        }
        .info {
            font-size: 18px;
            margin: 10px 0;
        }
        button {
            background: #007bff;
            border: none;
            border-radius: 5px;
            color: #fff;
            font-size: 16px;
            margin: 5px;
            padding: 10px 20px;
            cursor: pointer;
        }
        button:disabled {
            background: #ccc;
            cursor: not-allowed;
        }
        .inline-buttons button {
            margin: 5px 10px;
        }
        .custom-command {
            margin: 15px 0;
        }
        .custom-command input {
            padding: 10px;
            width: 70%;
            margin-right: 10px;
            border: 1px solid #ccc;
            border-radius: 5px;
        }
    </style>
    <script>
        // Update data from ESP32
        function updateData() {
            fetch("/status")
                .then(response => response.json())
                .then(data => {
                    document.getElementById("currentTemp").innerText = `${data.temperature} C`;
                    document.getElementById("burnerStatus").innerText = data.burnerStatus ? "ON" : "OFF";
                    document.getElementById("fireStrength").innerText = `${data.fireStrength}`;
                    document.getElementById("fuelLeft").innerText = `${data.fuelLeft}%`;
                });
        }

        // Send commands to ESP32
        function sendCommand(command) {
            fetch(`/command?cmd=${command}`)
                .then(response => {
                    if (response.ok) updateData();
                });
        }

        // Send custom command
        function sendCustomCommand() {
            const command = document.getElementById("customCommandInput").value.trim();
            if (command) {
                sendCommand(command);
                document.getElementById("customCommandInput").value = ""; // Clear input after sending
            } else {
                alert("Please enter a command.");
            }
        }

        // Initialize the interface
        document.addEventListener("DOMContentLoaded", () => {
            updateData();
        });
    </script>
</head>
<body>
    <div class="card">
        <h1>Pellet Burner Control</h1>
        <div class="info">Current Temperature: <span id="currentTemp">-- C</span></div>
        <div class="info">Burner Status: <span id="burnerStatus">--</span></div>
        <div class="info">Fire Strength: <span id="fireStrength">--</span></div>
        <div class="info">Fuel Left: <span id="fuelLeft">--%</span></div>

        <button onclick="sendCommand('toggleBurner')">Start/Stop Burner</button>
        <button onclick="sendCommand('boost')">Boost Burner</button>
        <div class="inline-buttons">
            <button onclick="sendCommand('heatUp')">+</button>
            <button onclick="sendCommand('heatDown')">-</button>
        </div>

        <div class="custom-command">
            <input id="customCommandInput" type="text" placeholder="Enter custom command" />
            <button onclick="sendCustomCommand()">Send</button>
        </div>
    </div>
</body>
</html>
)rawliteral";