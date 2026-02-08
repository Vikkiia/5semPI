const WebSocket = require('ws');

const socket = new WebSocket('ws://localhost:4000');

socket.onopen = () => {
    console.log('Connected to broadcast WebSocket server.');
    let counter = 0;
    const interval = setInterval(() => {
        counter++;
        socket.send(`10-03a-client: ${counter}`);
        console.log(`Sent: 10-03a-client: ${counter}`);
        if (counter === 8) {
            clearInterval(interval);
            socket.close();
        }
    }, 3000);
};

socket.onmessage = (event) => {
    console.log(`Message from server: ${event.data}`);
};

socket.onclose = () => {
    console.log('Disconnected from WebSocket server.');
};