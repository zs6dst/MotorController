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
				
				function setSpeed() {
					sendRequest('speed');
				}

				function setAcceleration() {
					sendRequest('accel');
				}
				
				function setAmps() {
					sendRequest('amps');
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
                Speed (Hz):
                <input id="speed"/>
                <button onClick=setSpeed()>Submit</button>
            </p>

            <p>
                Acceleration:
                <input id="accel"/>
                <button onClick=setAcceleration()>Submit</button>
            </p>

            <p>
                Current (Amps):
                <input id="amps"/>
                <button onClick=setAmps()>Submit</button>
            </p>
			
			<hr/>
			<p/>
			<table border>
				<tr>
					<th>Measure</th>
					<th>Req</th>
					<th>Act</th>
					<th>Max</th>
					<th>Avg</th>
				</tr>
				<tr>
					<td>mm/s</td>
					<td></td>
					<td></td>
					<td></td>
					<td></td>
				</tr>
				<tr>
					<td>steps/s</td>
					<td></td>
					<td></td>
					<td></td>
					<td></td>				
				</tr>
				<tr>
					<td>RPM</td>
					<td></td>
					<td></td>
					<td></td>
					<td></td>
				</tr>
				<tr>
					<td>Accel</td>
					<td></td>
					<td></td>
					<td></td>
					<td></td>				
				</tr>
			</table>
			
			<p/>
			<strong>Current (mA):</strong>
			<br/>Act: <input id="actAmp"/>
			<br/>Max: <input id="maxAmp"/>
			
			<p/>
			<strong>Step position:</strong>
			<br/>Act: <input id="actStepPos"/>
			<br/>Max: <input id="maxStepPos"/>
			
			<p/>
			<strong>Stallguard:</strong>
			<br/><input id="stallguard"/>
			
			<p/>
			<strong>TStep:</strong>
			<br/><input id="tstep"/>	
			
			<p/>
			<strong>Last movement duration (s):</strong>
			<br/><input id="lastDuration"/>
			
			<p/>
			<strong>Microsteps:</strong>
			<br/><input id="microsteps"/>        </body>

    </html>
    )=====";
