const http = require('http');

const jsonData = JSON.stringify({
    _comment: "lab09",
    x: 1,
    y: 2,
    s: "message",
    m: ["a", "b", "c"],
    o: { surname: "Borisova", name: "Victoria" }
});

const options = {
    hostname: '127.0.0.1',
    port: 5000,
    path: '/',
    method: 'POST',
    headers: {
        'Content-Type': 'application/json',
        'Content-Length': Buffer.byteLength(jsonData)
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
        console.log('Ответ сервера:');
        console.log(JSON.parse(responseBody));
    });
});

req.on('error', (e) => {
    console.error(`Ошибка запроса: ${e.message}`);
});

req.write(jsonData);
req.end();

/*

{
    "_comment": "lab09",
    "x": 1,
    "y": 2,
    "s": "message",
    "m": ["a", "b", "c"],
    "o": { "surname": "Borisova", "name": "Victoria" }
}

*/