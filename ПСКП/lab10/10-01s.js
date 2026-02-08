const http = require('http');
const WebSocket = require('ws');

const httpServer = http.createServer((req, res) => {
    if (req.method === 'GET' && req.url === '/start') {
        res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
        res.end(`
            <!DOCTYPE html>
            <html>
            <head>
                <title>10-01</title>
            </head>
            <body>
                <h1>10-01</h1>
                <button id="startWS">startWS</button>
                <div id="messages"></div>
                <script>
                    let ws;
                    let messageCounter = 0;
                    document.getElementById('startWS').addEventListener('click', () => {
                        ws = new WebSocket('ws://localhost:4000');
                        const messagesDiv = document.getElementById('messages');
                        ws.onopen = () => {
                            const intervalId = setInterval(() => {
                                messageCounter++;
                                const message = \`10-01-client: \${messageCounter}\`;
                                ws.send(message);
                                console.log(\`Отправлено: \${message}\`);
                            }, 3000);

                            setTimeout(() => {
                                clearInterval(intervalId);
                                ws.close();
                            }, 25000);
                        };
                        ws.onmessage = (event) => {
                            const message = document.createElement('div');
                            message.textContent = \`Получено: \${event.data}\`;
                            messagesDiv.appendChild(message);
                        };
                        ws.onclose = () => {
                            console.log('WebSocket соединение закрыто');
                        };
                    });
                </script>
            </body>
            </html>
        `);
    }
    else {
        res.writeHead(400, { 'Content-Type': 'text/plain' });
        res.end('Bad Request');
    }
});

httpServer.listen(3000, () => {
    console.log('HTTP сервер запущен на http://localhost:3000/start');
});

const wss = new WebSocket.Server({ port: 4000 });

let serverMessageCounter = 0;

wss.on('connection', (socket) => {
    console.log('Новое WebSocket-соединение установлено.');

    socket.on('message', (message) => {
        const messageStr = message.toString();
        console.log(`Получено сообщение от клиента: ${messageStr}`);
        const match = messageStr.match(/^(10-\d{2})-client: (\d+)$/);
        if (!match) {
            console.error('Неверный формат сообщения от клиента.');
            return;
        }
        const clientName = match[1];
        const clientMessageCounter = parseInt(match[2], 10);
        serverMessageCounter++;
        const response = `${clientName}-server: ${clientMessageCounter}->${serverMessageCounter}`;
        console.log(`Отправлено сообщение клиенту: ${response}`);
        socket.send(response);
    });
    socket.on('close', () => {
        console.log('WebSocket-соединение закрыто.');
    });
});

console.log('WebSocket сервер запущен на ws://localhost:4000');