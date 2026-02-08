const net = require('net');

const PORT = parseInt(process.argv[2], 10);
const X = parseInt(process.argv[3], 10);
const client = new net.Socket();

client.connect(PORT, '127.0.0.1', () => {
    console.log(`Подключено к серверу на порту ${PORT}. Отправляется число: ${X}`);

    let counter = 0;
    const interval = setInterval(() => {
        if (counter >= 20) {
            clearInterval(interval);
            client.end();
            return;
        }

        const buffer = Buffer.alloc(4);
        buffer.writeInt32LE(X);
        console.log(`Отправка: ${X}`);
        client.write(buffer);

        counter++;
    }, 1000);
});

client.on('data', (data) => {
    console.log(`Ответ от сервера: ${data.toString()}`);
});

client.on('close', () => {
    console.log('Отключено от сервера');
});

client.on('error', (err) => {
    console.error('Ошибка:', err);
});
