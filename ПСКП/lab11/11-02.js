const WebSocket = require('ws');
const fs = require('fs');

const wsserver = new WebSocket.Server({ port: 4000, host: 'localhost' })

wsserver.on('connection', (ws) => {
    const duplex = WebSocket.createWebSocketStream(ws, { encoding: 'utf8' });
    let rfile = fs.createReadStream('./MyFile.txt');
    rfile.pipe(duplex);
})

wsserver.on('error', err => console.error('ws server error', err));
console.log(`ws server: host: ${wsserver.options.host}, port: ${wsserver.options.port}`);