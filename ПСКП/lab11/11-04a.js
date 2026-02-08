const WebSocket = require('ws');
let parm = process.argv[2];
let clientName = typeof parm == 'undefined' ? 'defaultName' : parm;


setTimeout(() => {
    const wsClient = new WebSocket('ws://localhost:4000/');

    wsClient.on('open', () => {
        wsClient.on('message', (mess) => {
            console.log('\non message: ', mess.toString());
        });

        setInterval(() => {
            wsClient.send(JSON.stringify({ client: clientName, timestamp: new Date().toISOString() }, null, 2))
        }, 5000);

    });

    wsClient.on('error', error => {
        console.log('[ERROR] WebSocket: ', error.message);
    });
}, 500);

