const net = require('net');

const PORT = 3000;
const clients = new Map(); 

const server = net.createServer((socket) => {
    const clientId = `${socket.remoteAddress}:${socket.remotePort}`;
    clients.set(socket, 0); 

    console.log(`Клиент ${clientId} подключился`);

    socket.on('data', (data) => {
        const num = data.readInt32LE();
        console.log(`Клиент ${clientId} отправил: ${num}`);
        
        const newSum = clients.get(socket) + num;
        clients.set(socket, newSum);
    });

    socket.on('end', () => {
        console.log(`Клиент ${clientId} отключился`);
        clients.delete(socket);
    });

    socket.on('error', (err) => {
        console.error(`Ошибка у клиента ${clientId}:`, err);
        clients.delete(socket);
    });
});

setInterval(() => {
    console.log("=== Отправка промежуточных сумм ===");
    clients.forEach((sum, client) => {
        const buffer = Buffer.alloc(4);
        buffer.writeInt32LE(sum);
        client.write(buffer);
        console.log(`Отправлено клиенту ${client.remoteAddress}:${client.remotePort} -> ${sum}`);
    });
}, 5000);

server.listen(PORT, () => {
    console.log(`Сервер слушает порт ${PORT}`);
});
