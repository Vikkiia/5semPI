const http = require('http');

const options = {
    hostname: '127.0.0.1',
    port: 5000,
    path: '/',
    method: 'GET',
};

const req = http.request(options, (res) => {
    console.log(`Status Code: ${res.statusCode}`);
    console.log(`Status Message: ${res.statusMessage}`);
    console.log(`Remote Address: ${options.hostname}`);
    console.log(`Remote Port: ${options.port}`);
    
    res.on('data', (chunk) => {
        console.log(`Response Body: ${chunk}`);
    });
});

req.on('error', (e) => {
    console.error(`Request Error: ${e.message}`);
});

req.end();