const WebSocket = require("ws");
const fs = require("fs");

const ws = new WebSocket('ws://localhost:4000/wsserver');

ws.on("open", () => {
    console.log("Соединение открыто");
    const duplex = WebSocket.createWebSocketStream(ws,{encoding: 'utf8'});
    let rfile = fs.createReadStream(`./MyFile.txt`);
    rfile.pipe(duplex);    
});

ws.on("close", () => {
    console.log("Соединение закрыто");
});
