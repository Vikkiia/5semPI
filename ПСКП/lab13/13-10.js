const dgram = require("dgram");
const client = dgram.createSocket("udp4");

const SERVER_IP = "127.0.0.1";
const PORT = 3000;
const MESSAGE = "VILKA!";

client.send(MESSAGE, PORT, SERVER_IP, (err) => {
    if (err) {
        console.error("Ошибка отправки:", err);
        client.close();
    } else {
        console.log(`Сообщение отправлено: ${MESSAGE}`);
    }
});

client.on("message", (msg) => {
    console.log(`Ответ сервера: ${msg}`);
    client.close();
});
