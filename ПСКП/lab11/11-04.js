const WebSocket = require('ws');
let k = 0;


const wss = new WebSocket.Server({ port: 4000, host: 'localhost' });
wss.on('connection', ws => {

    ws.on('message', mess => {
        console.log('\non message: ', mess.toString());
        ws.send(JSON.stringify({ server: ++k, client: JSON.parse(mess).client, timestamp: new Date().toISOString() }, null, 2))
    })

});
wss.on('error', error => {
    console.log('[ERROR] WebSocket: ', error.message);
});