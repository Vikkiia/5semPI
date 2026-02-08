const net = require('net');

const server = net.createServer((socket) => {
    console.log('Клиент подключился');

    socket.on('data', (data) => {
        console.log(`Получено: ${data}`);
        socket.write(`ECHO: ${data}`);
    });

    socket.on('end', () => {
        console.log('Клиент отключился');
    });
});

server.listen(3000, () => {
    console.log('TCP-сервер запущен на порту 3000');
});
