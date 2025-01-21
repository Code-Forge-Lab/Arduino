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

        .button_small {
            margin: 5px;
            padding: 10px 20px;
            font-size: 1em;
            cursor: pointer;
            border: none;
            border-radius: 2px;
            background-color: #61dafb;
            color: #282c34;
            transition: background-color 0.3s, color 0.3s;
        }
        .button:hover {
            background-color: #21a1f1;
            color: white;
        }

        .button_small:hover {
            background-color: #21a1f1;
            color: white;
        }

        
        
        
        input[type=submit] {
          width: 85%;
          color: white;
          padding: 14px 20px;
          margin: 8px 0;
          border: none;
          border-radius: 4px;
          cursor: pointer;
          background-color: #61dafb;
          color: #282c34;
          transition: background-color 0.3s, color 0.3s;
          }

          input[type=text], select {
          width: 85%;
          padding: 12px 20px;
          margin: 8px 0;
          display: inline-block;
          border: 1px solid #ccc;
          border-radius: 4px;
          box-sizing: border-box;
          }



        /* Common button styles */
        #burnerButton {
            padding: 15px 30px;
            font-size: 18px;
            font-weight: bold;
            border: none;
            border-radius: 8px;
            cursor: pointer;
            transition: background-color 0.3s ease, color 0.3s ease;
        }

        /* Styles for OFF state */
        .off {
/*            background-color: red;*/
            background-color: #61daab;
            color: white;
        }

        /* Styles for ON state */
       /* .on {
            background-color: green;
            color: white;
        }*/



        
    </style>
</head>
<body>
    <script>
        // function checkForRefresh() {
        //     alert('REFRESH PAGE');
        //     fetch('/refresh')
        //       .then(response => response.text())
        //       .then(data => {
        //         if (data === "REFRESH") {
        //           location.reload();
        //         }
        //       })
        //       .catch(err => console.error(err));
        //   }
        //   window.onload = checkForRefresh;
    </script>
    
    <div class="center-content">
        <h1>Pellet Burner</h1>
        
        

        <div class="adc-value">Water Temp Required <span id="adc_TEMP_REQUIRED">0</span>C</div>
        <div class="adc-value">Water Temp. Output: <span id="adc_TEMP_HOT">0</span>C > Return: <span id="adc_TEMP_COLD">0</span></div>
        <div class="adc-value">Fire: <span id="adc_TEMP_HOT">50</span>%  Fuel: <span id="adc_TEMP_COLD">60</span>% FUELD [ <span id="adc_TEMP_COLD">YES</span> ]</div>
        
        <div class="adc-value">Burner: <span id="adc_BURNER_STATE">?</span> Pellet-Push: <span id="adc_PELLET_PUSH"></span>? Fan-Spin <span id="adc_FAN_SPIN">?</span> Burner-Boost <span id="adc_BURNER_BOOST">?</span></div>


        <div class="adc-value">
            <button class="button style= margin-top: 10px;" onclick="toggleRequiredTemp_Plus()">+</button>
            <span   id="adc_TEMP_REQUIRED2">80</span>
            <button class="button style= margin-top: 30px;"  onclick="toggleRequiredTemp_Minus()">-<br></button>
        </div>
        
            <button class="button" class="off" onclick="toggleBurner(this)">Burner OFF</button>
            
        <div>
            <p class="adc-value" >Pellet [<span id="adc_PELLET_ISACTIVE">OFF</span>] Fan [<span id="adc_FAN_ISACTIVE">OFF</span>] Boost [<span id="adc_FAN_BOOST">NO</span>]</p>
            <button class="button_small " onclick="PelletPush()">Pellet Push</button>
            <button class="button_small " onclick="FanSpin()">Fan Spin</button>
            <button class="button_small " onclick="BurnerBoost()">Boost</button>
        </div>
             <form onsubmit="sendCommand(event)">
             <!-- <label for="command"></label><br> -->
             <input class="button_small " type="submit" value="Send">
             <input type="text" id="command" name="command" placeholder="Input Command" required>
            </form>
             <p>Response: <span id="adc_CommandOut" style="color: blue;">??</span></p>
      </div>
    
    <script>



        function toggleBurner(button) {
                  if (button.classList.contains("off")) {
                      button.classList.remove("off");
                      button.classList.add("on");
                      button.innerText = "Burner ONN";
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



         function toggleRequiredTemp_Plus() {
            var xhttp = new XMLHttpRequest();
            // WaterTempRequired = WaterTempRequired + 1; // doesnt work
            xhttp.open("GET", "__requiredTempPlus", true);
            xhttp.send();
        }

         function toggleRequiredTemp_Minus() {
            var xhttp = new XMLHttpRequest();
            // WaterTempRequired = WaterTempRequired - 1; // doesnt work
            xhttp.open("GET", "__requiredTempMinus", true);
            xhttp.send();
        }




/////////////////////
        function PelletPush() {
            var xhttp = new XMLHttpRequest();
            // Here you can add the code to toggle the LED.
            // For demonstration, we just log to the console.
            
            // alert('LED toggled');  // Showing an alert for visual feedback
            xhttp.open("GET", "__btn_PELLET_PUSH", true);
            xhttp.send();
        }
        function FanSpin() {
            var xhttp = new XMLHttpRequest();
            // Here you can add the code to toggle the LED.
            // For demonstration, we just log to the console.
            
            // alert('LED toggled');  // Showing an alert for visual feedback
            xhttp.open("GET", "__btn_FAN_SPIN", true);
            xhttp.send();
        }
    
        function BurnerBoost() {
            var xhttp = new XMLHttpRequest();
            // Here you can add the code to toggle the LED.
            // For demonstration, we just log to the console.
            
            // alert('LED toggled');  // Showing an alert for visual feedback
            xhttp.open("GET", "__btn_BURNER_PUSH", true);
            xhttp.send();
        }
//////////////////    

        function GETDATAT_TEXTFUN(index,elementFunction) {
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", index+elementFunction, true);
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
        
         function GETDATAT_TEXT(index,element) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById(element).innerHTML =
            this.responseText;
          }
        };
        xhttp.open("GET", index+element , true);
        xhttp.send();
      }


        function getadc_TEMP_REQUIRED() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("adc_TEMP_REQUIRED").innerHTML =
            this.responseText;

            document.getElementById("adc_TEMP_REQUIRED2").innerHTML =
            this.responseText;
          }
        };
        xhttp.open("GET", "__adc_TEMP_REQUIRED", true);
        xhttp.send();
      }

      // function getADC_TEMP_HOT() {
      //   var xhttp = new XMLHttpRequest();
      //   xhttp.onreadystatechange = function() {
      //     if (this.readyState == 4 && this.status == 200) {
      //       document.getElementById("adc_TEMP_HOT").innerHTML =
      //       this.responseText;
      //     }
      //   };
      //   xhttp.open("GET", "__adc_TEMP_HOT", true);
      //   xhttp.send();
      // }


      
      // function getADC_TEMP_COLD() {
      //   var xhttp = new XMLHttpRequest();
      //   xhttp.onreadystatechange = function() {
      //     if (this.readyState == 4 && this.status == 200) {
      //       document.getElementById("adc_TEMP_COLD").innerHTML =
      //       this.responseText;
      //     }
      //   };
      //   xhttp.open("GET", "__adc_TEMP_COLD", true);
      //   xhttp.send();
      // }
        
        


     function sendCommand(event) {
            event.preventDefault(); // Prevent form redirection

            const command = document.getElementById("command").value;

            // Send the command via an AJAX POST request
            fetch("/command", {
                method: "POST",
                headers: { "Content-Type": "application/x-www-form-urlencoded" },
                body: `command=${encodeURIComponent(command)}`
            })
            .then(response => response.text())
            .then(data => {
                document.getElementById("adc_CommandOut").innerText = data; // Update the <span> with the response
            })
            .catch(error => {
                console.error("Error:", error);
                document.getElementById("adc_CommandOut").innerText = "Error sending command.";
            });
        }
        
        // Simulate ADC value update
        setInterval(() => {
            // Generate a random ADC value for demonstration
            //const adc_TEMP_REQUIRED = Math.floor(Math.random() * 1024);
            //document.getElementById('adc_TEMP_REQUIRED').innerText = adc_TEMP_REQUIRED;
            // getadc_TEMP_REQUIRED();
            // getADC_TEMP_HOT();
            // getADC_TEMP_COLD();

            // GETDATAT_TEXT("__","adc_TEMP_REQUIRED");
            getadc_TEMP_REQUIRED();
           
           
             // sendBurnerState("ONN"); // error with burnerstate
        }, 1000);  // Update every second
        setInterval(() => {
            

            


            GETDATAT_TEXT("__","adc_BURNER_STATE");
            GETDATAT_TEXT("__","adc_PELLET_PUSH");
            GETDATAT_TEXT("__","adc_FAN_SPIN");
            GETDATAT_TEXT("__","adc_BURNER_BOOST");
            
            GETDATAT_TEXT("__","adc_BURNER_STATE");
            GETDATAT_TEXT("__","adc_TEMP_HOT");
            GETDATAT_TEXT("__","adc_TEMP_COLD");
            GETDATAT_TEXT("__","adc_PELLET_ISACTIVE");
            GETDATAT_TEXT("__","adc_FAN_ISACTIVE");
            GETDATAT_TEXT("__","adc_FAN_BOOST");

        }, 1300);
    </script>
</body>
</html>

)rawliteral";