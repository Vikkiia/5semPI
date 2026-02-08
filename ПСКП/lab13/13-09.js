const dgram = require("dgram");
const server = dgram.createSocket("udp4");

const PORT = 3000;

server.on("message", (msg, rinfo) => {
    console.log(`Получено сообщение: ${msg}`);

    const response = `ECHO: ${msg}`;
    server.send(response, rinfo.port, rinfo.address, (err) => {
        if (err) {
            console.error("Ошибка отправки ответа:", err);
        }
    });
});

server.on("listening", () => {
    const address = server.address();
    console.log(`UDP сервер слушает ${address.address}:${address.port}`);
});

server.bind(PORT);
