const http = require('http');

const xmlData = `
<request>
    
    <x>4</x>
    <x>2</x>
    
    <m>
        <item>a</item>
        <item>b</item>
        <item>c</item>
        <item>d</item>
    </m>
   
</request>
`;

const options = {
    hostname: '127.0.0.1',
    port: 5000,
    path: '/',
    method: 'POST',
    headers: {
        'Content-Type': 'application/xml',
        'Content-Length': Buffer.byteLength(xmlData)
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
        console.log(responseBody);
    });
});

req.on('error', (e) => {
    console.error(`Ошибка запроса: ${e.message}`);
});

req.write(xmlData);
req.end();

