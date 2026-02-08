const sendMail = require('sendmail')();
const nodemailer = require('nodemailer');

const TO_EMAIL = 'vichka_16@vk.com';

const transporter = nodemailer.createTransport({
    host: 'smtp.mail.ru',
    port: 587,
    secure: false,
    auth: {
        user: 'vichka_16@vk.com',
        pass: 'nbvjhvjhb'
    }
});

function send(message, resp) {
    const mailOptions = {
        from: 'vichka_16@vk.com',
        to: 'vichka_16@vk.com',
        subject: 'Сообщение из модуля m0603',
        text: message,
    };

    transporter.sendMail(mailOptions, (err, info) => {
        if (err) {
            console.error('Ошибка отправки через nodemailer:', err);
            resp.end('<h1>Ошибка при отправке письма!</h1>');
        } else {
            console.log('Сообщение успешно отправлено через sendMail :', info.response);
            resp.end('<h1>Сообщение успешно отправлено! m0603</h1>');
        }
    });
}

module.exports = { send };






