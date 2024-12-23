const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Pellet Burner</title>
    <style>
        body {
            display: flex;
            justify-content: center;
            align-items: flex-start;
            height: 100vh;
            margin: 0;
            background-color: #282c34;
            color: white;
            font-family: Arial, sans-serif;
        }
        .center-content {
            text-align: center;
            margin-top: 50px;
        }
        h1 {
            font-size: 4em;
            margin-bottom: 20px;
        }
        .adc-value {
            margin: 2px 0;
            font-size: 1em;
            font-weight: bold;
            color: #61dafb;
        }
        .button, .switch {
            margin: 10px;
            padding: 20px 40px;
            font-size: 2em;
            cursor: pointer;
            border: none;
            border-radius: 5px;
            background-color: #61dafb;
            color: #282c34;
            transition: background-color 0.3s, color 0.3s;
        }
        .button:hover, .switch input:checked + .slider {
            background-color: #21a1f1;
            color: white;
        }
        .switch {
            position: relative;
            display: inline-block;
            width: 60px;
            height: 34px;
        }
        .switch input {
            opacity: 0;
            width: 0;
            height: 0;
        }
        .slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #ccc;
            transition: 0.4s;
            border-radius: 34px;
        }
        .slider:before {
            position: absolute;
            content: "";
            height: 26px;
            width: 26px;
            border-radius: 50%;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: 0.4s;
        }
        input:checked + .slider {
            background-color: #61dafb;
        }
        input:checked + .slider:before {
            transform: translateX(26px);
        }
    </style>
</head>
<body>
    <div class="center-content">
        <h1>Pellet Burner</h1>
        <div class="adc-value">Water Temp Required <span id="adc_TEMP_REQUIRED">0</span>°C</div>
        <div class="adc-value">Water Temp. Output: <span id="adc_TEMP_HOT">0</span>°C > Return: <span id="adc_TEMP_COLD">0</span>°C </div>
        
        <!-- Temperature Control Slider -->
        <input type="range" min="0" max="100" value="0" id="temp-slider" oninput="updateTemperature(this.value)">
        <span id="slider-value">0</span>°C
        
        <!-- LED Toggle Switch -->
        <label class="switch">
            <input type="checkbox" onchange="toggleLED(this.checked)">
            <span class="slider round"></span>
        </label>
        
        <!-- Action Buttons -->
        <button class="button" onclick="toggleRequiredTemp_Plus()">+</button>
        <button class="button" onclick="toggleRequiredTemp_Minus()">-</button>
    </div>

    <script>
        // Update temperature when the slider is changed
        function updateTemperature(value) {
            document.getElementById('slider-value').innerText = value;
            const xhttp = new XMLHttpRequest();
            xhttp.open("GET", `__setTemp?value=${value}`, true);
            xhttp.send();
        }

        function toggleRequiredTemp_Plus() {
            showLoading(this);
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "__requiredTempPlus", true);
            xhttp.onload = function() {
                resetLoading(this);
                getTemperatureValues();  // Update the values after change
            };
            xhttp.send();
        }

        function toggleRequiredTemp_Minus() {
            showLoading(this);
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "__requiredTempMinus", true);
            xhttp.onload = function() {
                resetLoading(this);
                getTemperatureValues();  // Update the values after change
            };
            xhttp.send();
        }

        function toggleLED(isOn) {
            const xhttp = new XMLHttpRequest();
            xhttp.open("GET", `led_toggle?state=${isOn ? 'on' : 'off'}`, true);
            xhttp.send();
        }

        function showLoading(button) {
            button.disabled = true;
            button.innerText = 'Processing...';
        }

        function resetLoading(button) {
            button.disabled = false;
            button.innerText = button.innerText === '+' ? '+' : '-';
        }

        function getTemperatureValues() {
            getadc_TEMP_REQUIRED();
            getADC_TEMP_HOT();
            getADC_TEMP_COLD();
        }

        function getadc_TEMP_REQUIRED() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("adc_TEMP_REQUIRED").innerHTML = this.responseText;
                }
            };
            xhttp.open("GET", "__adc_TEMP_REQUIRED", true);
            xhttp.send();
        }

        function getADC_TEMP_HOT() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("adc_TEMP_HOT").innerHTML = this.responseText;
                }
            };
            xhttp.open("GET", "__adc_TEMP_HOT", true);
            xhttp.send();
        }

        function getADC_TEMP_COLD() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("adc_TEMP_COLD").innerHTML = this.responseText;
                }
            };
            xhttp.open("GET", "__adc_TEMP_COLD", true);
            xhttp.send();
        }

        // Use setInterval to get the latest sensor data every 2 seconds
        setInterval(getTemperatureValues, 2000);
    </script>
</body>
</html>
)rawliteral";
