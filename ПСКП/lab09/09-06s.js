const http = require('http');
const fs = require('fs');
const path = require('path');

const uploadDir = path.join(__dirname, 'upload');

if (!fs.existsSync(uploadDir)) {
    fs.mkdirSync(uploadDir);
}

const server = http.createServer((req, res) => {
    if (req.method === 'POST' && req.url === '/upload') {
        const filePath = path.join(uploadDir, 'UpMyFile.txt');
        const fileStream = fs.createWriteStream(filePath);
        req.pipe(fileStream);
        req.on('end', () => {
            console.log(`Файл успешно сохранен: ${filePath}`);
            res.writeHead(200, { 'Content-Type': 'text/plain' });
            res.end('Файл успешно загружен');
        });
        req.on('error', (err) => {
            console.error(`Ошибка приема файла: ${err.message}`);
            res.writeHead(500, { 'Content-Type': 'text/plain' });
            res.end('Ошибка загрузки файла');
        });
    }
    else {
        res.writeHead(404, { 'Content-Type': 'text/plain' });
        res.end('не найден');
    }
});

server.listen(5000, () => {
    console.log('Сервер запущен на 5000');
});