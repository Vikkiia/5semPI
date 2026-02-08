const http = require('http');
const url = require('url');

const server = http.createServer((req, res) => {
    if (req.method === 'GET') {
        const queryObject = url.parse(req.url, true).query;
        const x = parseFloat(queryObject.x);
        const y = parseFloat(queryObject.y);
        if (isNaN(x) || isNaN(y)) {
            res.writeHead(400, { 'Content-Type': 'text/plain' });
            res.end('Invalid parameters');
        }
        else {
            const result = x * y;
            res.writeHead(200, { 'Content-Type': 'text/plain' });
            res.end(`${x} * ${y} = ${result}`);
        }
    }
    else {
        res.writeHead(405, { 'Content-Type': 'text/plain' });
        res.end('Method not allowed');
    }
});

server.listen(5000, () => {
    console.log('Server is running on port 5000');
});