const http = require('http');
const querystring = require('querystring');

const server = http.createServer((req, res) => {
    if (req.method === 'POST') {
        let body = '';
        req.on('data', (chunk) => {
            body += chunk;
        });
        req.on('end', () => {
            const parsedData = querystring.parse(body);
            const { x, y, s } = parsedData;//диструктуризация
            console.log(`Получены данные: x=${x}, y=${y}, s=${s}`);
            res.writeHead(200, { 'Content-Type': 'text/plain' });
            res.end(`Обработаны данные: x=${x}, y=${y}, s=${s}`);
        });
    }
    else {
        res.writeHead(405, { 'Content-Type': 'text/plain' });
        res.end('Только POST-запросы поддерживаются');
    }
});

server.listen(5000, () => {
    console.log('Server is running on port 5000.');
});