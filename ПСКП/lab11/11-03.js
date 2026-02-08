const WebSocket = require('ws');
let counter = 0;

const wsServer = new WebSocket.Server({ port: 4000, host: 'localhost' });
wsServer.on('connection', ws => {

    setInterval(() => {
        wsServer.clients.forEach((client) => {
            if (client.readyState === ws.OPEN)
                client.send(`11-03-server: ${++counter}`);
        });
    }, 15000);

    setInterval(() => {
        console.log(`\nAviable: ${wsServer.clients.size} client(s)`);
        ws.ping(`Server ping: ${wsServer.clients.size} client(s)`);
    }, 5000);

    ws.on('pong', data => {
        console.log(`on pong: ${data.toString()}`);
    });

    // ws.on('message', data => {
    //     console.log('on message: ', data.toString());
    //     ws.send(data);
    // })
});
wsServer.on('error', e => { console.log('[ERROR] ', e.message) });