const net = require('net');

const PORTS = [40000, 50000];

const createServer = (port) => {
    const server = net.createServer((socket) => {
        console.log(`Клиент подключился на порт ${port}`);

        socket.on('data', (data) => {
            const num = data.readInt32LE();
            console.log(`Получено на порту ${port}: ${num}`);

            const response = `ECHO: ${num}`;
            socket.write(response);
        });

        socket.on('end', () => {
            console.log(`Клиент отключился от порта ${port}`);
        });

        socket.on('error', (err) => {
            console.error(`Ошибка на порту ${port}:`, err);
        });
    });

    server.listen(port, () => {
        console.log(`Сервер слушает порт ${port}`);
    });
};

PORTS.forEach(createServer);
