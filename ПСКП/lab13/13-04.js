const net = require('net');

const PORT = 3000;
const client = new net.Socket();

client.connect(PORT, '127.0.0.1', () => {
    console.log('Подключено к серверу');

    let counter = 0;
    const interval = setInterval(() => {
        if (counter >= 20) {
            clearInterval(interval);
            client.end();
            return;
        }

        const num = Math.floor(Math.random() * 100);
        console.log(`Отправка: ${num}`);
        
        const buffer = Buffer.alloc(4);
        buffer.writeInt32LE(num);
        client.write(buffer);

        counter++;
    }, 1000);
});

client.on('data', (data) => {
    const sum = data.readInt32LE();
    console.log(`Получена промежуточная сумма: ${sum}`);
});

client.on('close', () => {
    console.log('Отключено от сервера');
});

client.on('error', (err) => {
    console.error('Ошибка:', err);
});
