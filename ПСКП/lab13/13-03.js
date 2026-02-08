const net = require('net');

const PORT = 3000;
let sum = 0;
const clients = new Set();

const server = net.createServer((socket) => {
    console.log('Клиент подключился');
    clients.add(socket);

    socket.on('data', (data) => {
        const num = data.readInt32LE();
        console.log(`Получено число: ${num}`);
        sum += num;
    });

    socket.on('end', () => {
        console.log('Клиент отключился');
        clients.delete(socket);
    });

    socket.on('error', (err) => {
        console.error('Ошибка сокета:', err);
        clients.delete(socket);
    });
});

setInterval(() => {
    console.log(`Отправка промежуточной суммы: ${sum}`);
    for (const client of clients) {
        const buffer = Buffer.alloc(4);
        buffer.writeInt32LE(sum);
        client.write(buffer);
    }
}, 5000);

server.listen(PORT, () => {
    console.log(`Сервер слушает порт ${PORT}`);
});
