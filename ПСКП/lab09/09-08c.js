const http = require('http');
const fs = require('fs');
const path = require('path');

const options = {
    hostname: '127.0.0.1',
    port: 5000,
    path: '/download',
    method: 'GET',
};

const req = http.request(options, (res) => {
    console.log(`Статус ответа: ${res.statusCode}`);
    console.log(`Сообщение к статусу: ${res.statusMessage}`);
    if (res.statusCode === 200) {
        const filePath = path.join(__dirname, 'VilkinFile.txt');
        const fileStream = fs.createWriteStream(filePath);
        res.pipe(fileStream);
        fileStream.on('finish', () => {
            console.log(`Файл успешно получен и сохранен: ${filePath}`);
        });
    }
    else {
        let responseBody = '';
        res.on('data', (chunk) => {
            responseBody += chunk;
        });
        // res.on('end', () => {
        //     console.log('Ответ сервера:', responseBody);
        // });
    }
});

req.on('error', (e) => {
    console.error(`Ошибка запроса: ${e.message}`);
});

req.end();