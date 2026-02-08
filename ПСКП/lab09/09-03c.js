const http = require('http');
const querystring = require('querystring');

const postData = querystring.stringify({ x: 5, y: 10, s: "Vilkin" });

const options = {
    hostname: '127.0.0.1',
    port: 5000,
    path: '/',
    method: 'POST',
    headers: {
        'Content-Type': 'application/x-www-form-urlencoded',
        'Content-Length': Buffer.byteLength(postData)
    }
};

const req = http.request(options, (res) => {
    console.log(`Статус ответа: ${res.statusCode}`);
    console.log(`Сообщение к статусу: ${res.statusMessage}`);
    console.log(`IP-адрес удаленного сервера: ${res.socket.remoteAddress}`);
    console.log(`Порт удаленного сервера: ${res.socket.remotePort}`);
    res.setEncoding('utf8');
    res.on('data', (chunk) => {
        console.log(`Данные ответа: ${chunk}`);
    });
});

req.on('error', (e) => {
    console.error(`Ошибка запроса: ${e.message}`);
});

req.write(postData);
req.end();

//x=5&y=10&s=Vilkin