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
        .button {
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
        .button:hover {
            background-color: #21a1f1;
            color: white;
        }
    </style>
</head>
<body>
    <div class="center-content">
        <h1>Pellet Burner</h1>
        <div class="adc-value">Water Temp Required <span id="adc_TEMP_REQUIRED">0</span>C</div>
        <div class="adc-value">Water Temp. Output: <span id="adc_TEMP_HOT">0</span>C > Return: <span id="adc_TEMP_COLD">0</span>C </div>
        <button class="button" onclick="toggleRequiredTemp_Plus()">+</button>
        <button class="button" onclick="toggleRequiredTemp_Minus()">-<br></button>
        <button class="button" onclick="toggleLED()">LED Switch</button>
    </div>

    <script>
        

         function toggleRequiredTemp_Plus() {
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "__requiredTempPlus", true);
            xhttp.send();
        }

         function toggleRequiredTemp_Minus() {
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "__requiredTempMinus", true);
            xhttp.send();
        }


        function toggleLED() {
            var xhttp = new XMLHttpRequest();
            // Here you can add the code to toggle the LED.
            // For demonstration, we just log to the console.
            
            // alert('LED toggled');  // Showing an alert for visual feedback
            xhttp.open("GET", "led_toggle", true);
            xhttp.send();
        }
        
        function getadc_TEMP_REQUIRED() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("adc_TEMP_REQUIRED").innerHTML =
            this.responseText;
          }
        };
        xhttp.open("GET", "__adc_TEMP_REQUIRED", true);
        xhttp.send();
      }

      function getADC_TEMP_HOT() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("adc_TEMP_HOT").innerHTML =
            this.responseText;
          }
        };
        xhttp.open("GET", "__adc_TEMP_HOT", true);
        xhttp.send();
      }


      
      function getADC_TEMP_COLD() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("adc_TEMP_COLD").innerHTML =
            this.responseText;
          }
        };
        xhttp.open("GET", "__adc_TEMP_COLD", true);
        xhttp.send();
      }
        
        // Simulate ADC value update
        setInterval(() => {
            // Generate a random ADC value for demonstration
            //const adc_TEMP_REQUIRED = Math.floor(Math.random() * 1024);
            //document.getElementById('adc_TEMP_REQUIRED').innerText = adc_TEMP_REQUIRED;
            getadc_TEMP_REQUIRED();
            getADC_TEMP_HOT();
            getADC_TEMP_COLD();
        }, 1000);  // Update every second
    </script>
</body>
</html>

)rawliteral";