const http = require('http');
const fs = require('fs');
const path = require('path');

const filePath = path.join(__dirname, 'MyFile.txt');

if (!fs.existsSync(filePath)) {
    fs.writeFileSync(filePath, 'тест');
}

const server = http.createServer((req, res) => {
    if (req.method === 'GET' && req.url === '/download') {
        console.log('Получен запрос на загрузку файла.');
        res.writeHead(200, {
            'Content-Type': 'text/plain',
            'Content-Disposition': 'attachment; filename="MyFile.txt"',
        });
        const fileStream = fs.createReadStream(filePath);
        fileStream.pipe(res);
        fileStream.on('end', () => {
            console.log('Файл успешно отправлен клиенту.');
        });
        fileStream.on('error', (err) => {
            console.error(`Ошибка при отправке файла: ${err.message}`);
            res.writeHead(500, { 'Content-Type': 'text/plain' });
            res.end('Ошибка при отправке файла');
        });
    }
    else {
        res.writeHead(404, { 'Content-Type': 'text/plain' });
        res.end('Эндпоинт не найден');
    }
});

server.listen(5000, () => {
    console.log('Сервер запущен на 5000');
});