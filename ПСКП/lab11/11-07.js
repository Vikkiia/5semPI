const rpcServer = require('rpc-websockets').Server;
const server = new rpcServer({ port: 4000, host: 'localhost' });

server.register('A', params => { console.log('A УВЕДОМЛЕНИЕ'); }).public();
server.register('B', params => { console.log('B УВЕДОМЛЕНИЕ'); }).public();
server.register('C', params => { console.log('C УВЕДОМЛЕНИЕ'); }).public();