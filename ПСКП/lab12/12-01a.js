const WebSocket = require('ws');


setTimeout(() => {
    const ws = new WebSocket('ws://localhost:4000/');

    ws.on('open', () => {
        console.log('\nSocket opened.');

        ws.on('message', message => {
            console.log('[INFO]', message.toString());
        })
    })

    ws.on('close', () => { console.log('Socket closed.'); });

    ws.on('error', error => { console.log('[ERROR]: ', error.message); });

}, 100);