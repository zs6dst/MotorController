const char WEBPAGE[] PROGMEM = R"=====(
        <html>

        <head>
            <title>Reloader Motor</title>

            <meta name='viewport' content='width=device-width, initial-scale=1.0' />
            <meta charset='UTF-8' />
			
			<style>
				.output {
					border-width:1px;
					border-style:solid;
					border-color:black;
				}
			</style>

            <script>
                var socket;

				function setValue(item) {
					let element = document.getElementById(item.id);
					if (element) {
                        element.innerHTML = item.value;
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
				
				function sendRequest(id, value) {
                    socket.send(JSON.stringify({ id: id, value: value }));
				}

                function toggleLed() { //Toggle LED value; ie. no value specified, only ID
					sendRequest('led', '');
                }
				
				function setRPM() {
					sendRequest('rpm', document.getElementById('rpmIn').value);
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
                <input id="rpmIn"/>
                <button onClick=setRPM()>Submit</button>
				<hr/>
				<p>
					RPM: <span id="rpm" class="output">000</span>
				</p>
				<p>
					Speed (microsteps/s): <span id="speed" class="output">000</span>
				</p>
            </p>
        </body>
    </html>
    )=====";
