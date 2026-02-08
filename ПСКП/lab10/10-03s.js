const WebSocket = require('ws');

const wsServer = new WebSocket.Server({ port: 4000 });

let messageCounter = 0;

wsServer.on('connection', (socket) => {
    console.log('WebSocket client connected.');
    socket.on('message', (message) => {
        console.log(`Received: ${message}`);
        wsServer.clients.forEach((client) => {
            if (client.readyState === WebSocket.OPEN) {
                messageCounter++;
                client.send(`Broadcast message ${messageCounter}: ${message}`);
            }
        });
    });
    socket.on('close', () => {
        console.log('WebSocket client disconnected.');
    });
});

console.log('Broadcast WebSocket Server is running on ws://localhost:4000');