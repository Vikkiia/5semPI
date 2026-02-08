const http = require('http');

const options = {
    hostname: '127.0.0.1',
    port: 5000,
    path: '/?x=2&y=3',
    method: 'GET',
};

const req = http.request(options, (res) => {
    console.log(`Status Code: ${res.statusCode}`);

    res.on('data', (chunk) => {
        console.log(`Response Body: ${chunk}`);
    });
});

req.on('error', (e) => {
    console.error(`Request Error: ${e.message}`);
});

req.end();