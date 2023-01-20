const char WEBPAGE[] PROGMEM = R"=====(
    <html>

        <head>
            <script>
                var xhr = new XMLHttpRequest();
                var item;

                xhr.onreadystatechange = () => {
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
            Hello World!
            <p>
                LED <button id="led" onclick="switchLED()">OFF</button>
            </p>

        </body>

    </html>
    )=====";
