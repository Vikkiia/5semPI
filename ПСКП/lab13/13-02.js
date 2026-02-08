const net = require('net');

const client = new net.Socket();

client.connect(3000, '127.0.0.1', () => {
    console.log('Подключено к серверу');
    client.write('HIII!');
});

client.on('data', (data) => {
    console.log(`Ответ от сервера: ${data}`);
    client.destroy();
});

client.on('close', () => {
    console.log('Соединение закрыто');
});
