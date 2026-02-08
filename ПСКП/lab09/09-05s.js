const http = require('http');
const { parseStringPromise, Builder } = require('xml2js');

const server = http.createServer((req, res) => {
    if (req.method === 'POST' && req.headers['content-type'] === 'application/xml') {
        let body = '';
        req.on('data', (chunk) => {
            body += chunk;
        });
        req.on('end', async () => {
            try {
                const parsedXml = await parseStringPromise(body);
                console.log('Полученные данные:', parsedXml);
                const x = parseInt(parsedXml.request.x[0], 10);
                const x2 = parseInt(parsedXml.request.x[1], 10);
                const sum = x + x2;
                const items = parsedXml.request.m[0].item;
                const concat = items.join('');
                const responseXml = new Builder().buildObject({
                    response: {
                        $: { id: '33', request: '28' },
                        sum: { $: { element: 'x', result: sum.toString() } },
                        concat: { $: { element: 'm', result: concat } }
                    }
                });
                res.writeHead(200, { 'Content-Type': 'application/xml' });
                res.end(responseXml);
            } catch (err) {
                res.writeHead(400, { 'Content-Type': 'text/plain' });
                res.end('Ошибка обработки XML');
            }
        });
    }
    else {
        res.writeHead(400, { 'Content-Type': 'text/plain' });
        res.end('Только POST-запросы с XML поддерживаются');
    }
});

server.listen(5000, () => {
    console.log('Сервер запущен на 5000');
});