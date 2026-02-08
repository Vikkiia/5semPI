
const http = require('http');
const fs = require('fs');
const url = require('url');
const { parse } = require('querystring');
const nodemailer = require('nodemailer');

const transporter = nodemailer.createTransport({
    host: 'smtp.mail.ru',
    port: 587,
    secure: false,
    auth: {
        user: 'vichka_16@vk.com',
        pass: 'vbgbgbgghhghhhh'
    }
});

//vichka_kazakova_16 @vk.com


http.createServer((req, resp) => {
    const pathname = url.parse(req.url).pathname;
    resp.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });

    if (pathname === '/' && req.method === 'GET') {
        fs.readFile('./06-02.html', (err, data) => {
            if (err) {
                console.error('Ошибка загрузки формы:', err);
                resp.end('<h1>Ошибка загрузки формы</h1>');
            } else {
                resp.end(data);
            }
        });
    } else if (pathname === '/' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => body += chunk.toString());
        req.on('end', () => {
            const { from, to, message } = parse(body);

            console.log(body);

            if (!from || !to || !message) {
                resp.end('<h1>Ошибка: Все поля должны быть заполнены!</h1>');
                return;
            }

            transporter.sendMail({
                from: 'vichka_16@vk.com',
                to: to,
                subject: 'Сообщение из приложения',
                text: message
            }, (smtpErr, info) => {
                if (smtpErr) {
                    console.error('Ошибка отправки :', smtpErr);
                    resp.end(`<h1>Ошибка отправки: ${smtpErr.message}</h1>`);
                } else {
                    console.log('Сообщение успешно отправлено:', info.response);
                    resp.end('<h1>Сообщение успешно отправлено!</h1>');
                }
            });
        });
    } else {
        resp.end('<h1>Not supported</h1>');
    }
}).listen(3000, () => console.log('Server running at http://localhost:3000/'));