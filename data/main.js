async function updateValue(id, value) {
    await fetch(`/setvalue?${id}=${value}`);
}

function restartESP32() {
    updateValue('esp32', 'restart');
}

function toggleLed() {
    updateValue('led', 'toggle');
}

function setRPM() {
    updateValue('rpm', document.getElementById('rpmIn').value);
}

function setMicrosteps() {
    updateValue('microsteps', document.getElementById('microstepsIn').value);
}

if (!!window.EventSource) {
    let source = new EventSource('/events');

    source.addEventListener('open', (e) => {
        console.log('Events connected');
    }, false);

    source.addEventListener('message', (e) => {
        console.log(e.data);
    }, false);

    source.addEventListener('error', (e) => {
        if (e.target.readyState != EventSource.OPEN) {
            console.log('Events disconnected');
        }    
    }, false);    

    let ids = ['led', 'stealthchop', 'rpm', 'microsteps', 'speed'];
    ids.forEach((id) => {
        source.addEventListener(id, (e) => {
            let element = document.getElementById(id);
            if (element) element.innerHTML = e.data;
        });
    }, false);
}