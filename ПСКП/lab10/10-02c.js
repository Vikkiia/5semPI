const WebSocket = require('ws');

const ws = new WebSocket('ws://localhost:4000');

let messageCounter = 0;

ws.on('open', () => {
    console.log('WebSocket-соединение установлено.');
    const intervalId = setInterval(() => {
        messageCounter++;
        const message = `10-02-client: ${messageCounter}`;
        ws.send(message);
        console.log(`Отправлено: ${message}`);
    }, 3000);
    setTimeout(() => {
        clearInterval(intervalId);
        ws.close();
    }, 25000);
});

ws.on('message', (data) => {
    console.log(`Получено от сервера: ${data}`);
});

ws.on('close', () => {
    console.log('WebSocket-соединение закрыто.');
});