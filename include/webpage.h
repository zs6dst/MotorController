const char WEBPAGE[] PROGMEM = R"=====(
    <html>

        <head>
            <title>Reloader Motor</title>

            <meta name='viewport' content='width=device-width, initial-scale=1.0' />
            <meta charset='UTF-8' />

            <script>
                var socket;

				function setValue(item) {
					let element = document.getElementById(item.id);
					if (element) {
                        if (item.id == 'led') {
                            element.innerHTML = item.value;
                        } else {
						    element.value = item.value;
                        }
					}
				}

                function messageHandler(event) {
                   console.log("Received from websocket: " + event.data);

                    let data = JSON.parse(event.data);
                    if (!data) {
                        console.log('No data received from websocket');
                        return;
                    }
					
					data.forEach(setValue);
                }

                function init() {
                    let url = 'ws://' + window.location.hostname + ':81/';
                    socket = new WebSocket(url);
                    socket.onmessage = (event) => { messageHandler(event); };
                }

                function toggleLed() { //Toggle LED value; ie. no value specified, only ID
                    socket.send(JSON.stringify({ id: "led" }));
                }
				
				function sendRequest(id) {
					let req = { 
						id: id, 
						value: document.getElementById(id).value 
					};
                    socket.send(JSON.stringify(req));
				}
				
				function setRPM() {
					sendRequest('rpm');
				}

                window.onload = (event) => init();
            </script>
        </head>

        <body>
            <h1>AutoReloader</h1>
            <p>
                LED <button id="led" onClick=toggleLed()>?</button>
            </p>

            <p>
                Speed (RPM):
                <input id="rpm"/>
                <button onClick=setRPM()>Submit</button>
            </p>
        </body>
    </html>
    )=====";
