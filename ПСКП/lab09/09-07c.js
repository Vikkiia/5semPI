const http = require('http');
const fs = require('fs');
const path = require('path');

const filePath = path.join(__dirname, 'MyFile.png');
const fileStats = fs.statSync(filePath);

const options = {
    hostname: '127.0.0.1',
    port: 5000,
    path: '/upload',
    method: 'POST',
    headers: {
        'Content-Type': 'image/png',
        'Content-Length': fileStats.size
    }
};

const req = http.request(options, (res) => {
    console.log(`Статус ответа: ${res.statusCode}`);
    console.log(`Сообщение к статусу: ${res.statusMessage}`);
    let responseBody = '';
    res.on('data', (chunk) => {
        responseBody += chunk;
    });
    res.on('end', () => {
        console.log('Ответ сервера:', responseBody);
    });
});

req.on('error', (e) => {
    console.error(`Ошибка запроса: ${e.message}`);
});

const fileStream = fs.createReadStream(filePath);
fileStream.pipe(req);

fileStream.on('error', (err) => {
    console.error(`Ошибка чтения файла: ${err.message}`);
});