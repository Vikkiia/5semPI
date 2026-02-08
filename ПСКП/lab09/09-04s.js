const http = require('http');

const server = http.createServer((req, res) => {
    if (req.method === 'POST' && req.headers['content-type'] === 'application/json') {
        let body = '';
        req.on('data', (chunk) => {
            body += chunk;
        });
        req.on('end', () => {
            const parsedData = JSON.parse(body);
            console.log('Получены данные:', parsedData);
            const responseData = {
                "__comment": "Ответ: " + parsedData._comment,
                "x_plus_y": parsedData.x + parsedData.y,
                "Concatination_s_o": parsedData.s + ": " + parsedData.o.surname + " " + parsedData.o.name,
                "Length_m": parsedData.m.length
            };
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify(responseData));
        });
    }
    else {
        res.writeHead(400, { 'Content-Type': 'text/plain' });
        res.end('Только POST-запросы с JSON поддерживаются');
    }
});

server.listen(5000, () => {
    console.log('Сервер запущен на 5000');
});