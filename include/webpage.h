const char WEBPAGE[] PROGMEM = R"=====(
    <html>

        <head>
            <title>Reloader Motor</title>

            <meta name='viewport' content='width=device-width, initial-scale=1.0' />
            <meta charset='UTF-8' />

            <script>
                var xhr = new XMLHttpRequest();
                var item;

                xhr.onreadystatechange = function() {
                    console.log("Response: " + this.responseText);
                    console.log("readyState: " + this.readyState);
                    console.log("status: " + this.status);
                    if (this.readyState == 4 && this.status == 200) {
                        let result = JSON.parse(this.responseText);
                        switch (item) {
                            case "led":
                                document.getElementById(item).innerHTML = result.led ? "ON" : "OFF";
                                break;
                        }
                    }
                };

                function switchLED() {
                    item = "led";
                    xhr.open("GET", "/led", true);
                    xhr.send();
                }
            </script>
        </head>

        <body>
            <h1>Reloader Motor</h1>

            <p>
                LED <button id="led" onclick="switchLED()">OFF</button>
            </p>

        </body>

    </html>
    )=====";
