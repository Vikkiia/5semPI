const url = require('url');
const fs = require('fs');
const { parse } = require('querystring');
const xml2js = require('xml2js');
const multiParty = require('multiparty');
const path = require('path');

class Service {
    constructor(server, sockets) {
        this.server = server;
        this.sockets = sockets;
    }

    handleConnection = (req, res) => {
        let setParameter = url.parse(req.url, true).query.set;
        res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
        if (!setParameter) {
            res.end(`<h1>KeepAliveTimeout: ${this.server.keepAliveTimeout}</h1>`);
        }
        else if (Number(setParameter)) {
            console.log('KeepAliveTimeout: ' + setParameter);
            this.server.keepAliveTimeout = Number(setParameter);
            res.end(`<h1>New KeepAliveTimeout = ${this.server.keepAliveTimeout}</h1>`);
        }
        else {
            res.end(`<h1>Enter correct keepAliveTimeout. </br>Your value: ${setParameter}</h1>`);
        }
    };

    handleHeaders = (req, res) => {
        let i = 0;
        let result = '<h1>Заголовки запроса:</h1>';
        for (let key in req.headers) {
            result += `${++i}. ${key}: ${req.headers[key]}<br/>`;
        }
        i = 0;
        result += '<br/><h1>Заголовки ответа:</h1>';
        res.setHeader('Connection', 'keep-alive');
        res.setHeader('Content-Type', 'text/html; charset=utf-8');
        res.setHeader('X-Lab-Student', 'Vilka');
        let resHeaders = res.getHeaders();
        console.log('response headers: ', resHeaders);
        for (let key in resHeaders) {
            result += `${++i}. ${key}: ${resHeaders[key]}<br/>`;
        }
        res.writeHead(200);
        res.end(result);
    }

    handleClose = (req, res) => {
        let pathname = url.parse(req.url, true).pathname;
        if (pathname === '/close') {
            res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
            res.end('<h1>Сервер будет остановлен через 10 секунд...</h1>');
            console.log('Сервер остановится через 10 секунд...');
            setTimeout(() => {
                console.log('Остановка сервера.');
                this.server.close(() => {
                    console.log('Сервер остановлен.');
                });
            }, 10000);
        }
        else {
            res.writeHead(404, { 'Content-Type': 'text/html; charset=utf-8' });
            res.end('<h1>Страница не найдена</h1>');
        }
    };

    handleReqData = (req, res) => {
        const query = url.parse(req.url, true).query;
        const message = query.message || '';
        const chunkSize = 10;
        if (!message) {
            res.writeHead(400, { 'Content-Type': 'text/plain; charset=utf-8' });
            res.end('Ошибка: параметр message не передан');
            return;
        }
        res.writeHead(200, { 'Content-Type': 'text/plain; charset=utf-8' });
        let currentIndex = 0;
        function sendChunk() {
            if (currentIndex < message.length) {
                const chunk = message.slice(currentIndex, currentIndex + chunkSize);
                res.write(`Получена порция данных: "${chunk}"\n`);
                console.log(`Получена порция данных: "${chunk}"`);
                currentIndex += chunkSize;
                // 500мл
                setTimeout(sendChunk, 500);
            }
            else {
                res.write('Все данные обработаны.\n');
                console.log('Все данные обработаны.');
                res.end();
            }
        }
        sendChunk();
    }
    //AAAAAAAAAADDDDDDDDDDFFFFFFFFFFGGGGGGGGGGHHHHHHHHHHYYYY
    handleParameters = (req, res) => {
        let xQuery = url.parse(req.url, true).query.x;
        let yQuery = url.parse(req.url, true).query.y;
        let xRoute = url.parse(req.url).pathname.split('/')[2];
        let yRoute = url.parse(req.url).pathname.split('/')[3];
        res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
        if (Number(xQuery) && Number(yQuery)) {
            res.end(`
                    <h1>
                        x = ${xQuery}; y = ${yQuery} <br/>
                        x + y = ${+xQuery + +yQuery} <br/>
                        x - y = ${xQuery - yQuery} <br/>
                        x * y = ${xQuery * yQuery} <br/>
                        x / y = ${xQuery / yQuery}
                    </h1>
            `);
        }
        else if (xRoute !== undefined && yRoute !== undefined) {
            if (Number(xRoute) && Number(yRoute)) {
                res.end(`<h1>
                        x = ${xRoute}; y = ${yRoute} <br/>
                        x + y = ${+xRoute + +yRoute} <br/>
                        x - y = ${xRoute - yRoute} <br/>
                        x * y = ${xRoute * yRoute} <br/>
                        x / y = ${xRoute / yRoute}
                    </h1>`);
            }
            else {
                res.end(`<h1>${req.url}</h1>`);
            }
        }
        else {
            res.end(`<h1>Enter correct x and y parameters in query. </h1>`);
        }
    }

    handleSockets = (req, res) => {
        const clientAddress = req.socket.remoteAddress;
        const clientPort = req.socket.remotePort;
        const serverAddress = req.socket.localAddress;
        const serverPort = req.socket.localPort;
        res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
        res.end(`
            <h1>Информация о подключении:</h1>
            <p>IP-адрес клиента: ${clientAddress}</p>
            <p>Порт клиента: ${clientPort}</p>
            <p>IP-адрес сервера: ${serverAddress}</p>
            <p>Порт сервера: ${serverPort}</p>
        `);
        console.log('Обработан запрос на /socket');
        console.log(`IP-адрес клиента: ${clientAddress}, Порт клиента: ${clientPort}`);
        console.log(`IP-адрес сервера: ${serverAddress}, Порт сервера: ${serverPort}`);
    }

    handleResponseStatus = (req, res) => {
        let code = url.parse(req.url, true).query.code;
        let message = url.parse(req.url, true).query.mess;
        console.log('code = ' + code);
        console.log('mess = ' + message);
        if (code === undefined || message === undefined) {
            res.writeHead(405, 'Incorrect parameters', { 'Content-Type': 'text/html; charset=utf-8' });
            res.end('<h1>Enter parameters in URI: code = int, mess = string.</h1>');
        }
        else if (Number(code)) {
            if (+code >= 200 && +code < 600) {
                res.writeHead(+code, message, { 'Content-Type': 'text/html; charset=utf-8' });
                res.end(`<h1>Responsed with Code = ${code} and Message = ${message}</h1>`);
            }
            else {
                res.writeHead(406, 'Invalid StatusCode', { 'Content-Type': 'text/html; charset=utf-8' });
                res.end('<h1>Enter valid StatusCode (200-599).</h1>')
            }
        }
        else {
            res.writeHead(407, 'Incorrect StatusCode', { 'Content-Type': 'text/html; charset=utf-8' });
            res.end('<h1>Enter correct StatusCode (200-599).</h1>')
        }
    }

    handleFormParameter = (req, res) => {
        res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
        if (req.method === 'GET') {
            res.end(fs.readFileSync('./form.html'));
        }
        else if (req.method === 'POST') {
            let body = '';
            let parm = '';
            req.on('data', chunk => {
                body += chunk.toString();
            });
            req.on('end', () => {
                parm = parse(body);
                console.log('parm = ', parm);
                res.end(`<h2>
                            Text: ${parm.inpText} <br/>
                            Number: ${parm.inpNumber} <br/>
                            Date: ${parm.inpDate} <br/>
                            Checked checkbox(s): ${parm.inpCheck} <br/>
                            Selected radiobutton: ${parm.inpRadio} <br/>
                            Textarea: ${parm.inpTextArea} <br/>
                            Submit: ${parm.inpSubmitForm} <br/>
                        </h2>`);
            });
        }
        else {
            res.writeHead(408, 'Incorrect method', { 'Content-Type': 'text/html; charset=utf-8' });
            res.end('<h1>Incorrect method</h1>');
        }


    }

    handleJSON = (req, res) => {
        let data = '';
        req.on('data', chunk => {
            data += chunk;
        });
        req.on('end', () => {
            try {
                const requestData = JSON.parse(data);
                const comment = requestData._comment;
                const x = requestData.x;
                const y = requestData.y;
                const s = requestData.s;
                const o = requestData.o;
                const m = requestData.m;
                const response = {
                    "__comment": "Ответ: " + comment,
                    "x_plus_y": x + y,
                    "Concatination_s_o": s + ": " + o.surname + " " + o.name,
                    "Length_m": m.length
                };
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify(response));
            }
            catch (error) {
                res.writeHead(400, { 'Content-Type': 'text/plain' });
                res.end('Bad Request');
            }
            /*
            *   {
                "_comment": "lab08",
                "x": 1,
                "y": 2,
                "s": "message",
                "m":["a", "b", "c"],
                "o": {"surname": "Borisova", "name": "Victoria" }
                }
            * */
        });
    }

    handleXML = (req, res) => {
        let data = '';
        req.on('data', chunk => {
            data += chunk;
        });
        req.on('end', () => {
            xml2js.parseString(data, (err, result) => {
                if (err) {
                    res.writeHead(400, { 'Content-Type': 'text/plain' });
                    res.end('Bad Request');
                    return;
                }
                const request = result.request;
                const id = request.$.id;
                const xs = request.x.map(x => +x.$.value || 0);
                const ms = request.m.map(m => m.$.value);
                const sumX = xs.reduce((acc, curr) => acc + curr, 0);
                const concatM = ms.join('');
                const response = {
                    response: {
                        $: { id: id, request: id },
                        sum: { $: { element: 'x', result: sumX.toString() } },
                        concat: { $: { element: 'm', result: concatM } }
                    }
                };
                const builder = new xml2js.Builder();
                const xml = builder.buildObject(response);
                res.writeHead(200, { 'Content-Type': 'application/xml' });
                res.end(xml);
            });
        });

        /*
        *   <request id="28">
                <x value = "1"/>
                <x value = "2"/>
                <m value = "a"/>
                <m value = "b"/>
                 <m value = "c"/>
            </request>
        * */
    }

    handleFiles = (req, res) => {
        let filename = url.parse(req.url).pathname.split('/')[2];
        if (filename === undefined) {
            fs.readdir('./static', (err, files) => {
                if (err) {
                    res.end('<h1>Unable to find ./static directory<h1>');
                    return;
                }
                res.setHeader('X-static-files-count', `${files.length}`);
                res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
                res.end(`<h1>Count of files in ./static directory: ${files.length}.</h1>`);
            });
        }
        else {
            try {
                res.setHeader('Content-Type', 'application/octet-stream');
                res.end(fs.readFileSync(`static/${filename}`));

            }
            catch (err) {
                res.writeHead(404, 'Resource not found', { 'Content-Type': 'text/html; charset=utf-8' });
                res.end('<h1>Error 404: Resource not found.</h1>')
            }
        }
    }

    handleUpload = (req, res) => {
        res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
        if (req.method === 'GET') {
            res.end(fs.readFileSync('./upload.html'));
        }
        else if (req.method === 'POST') {
            let form = new multiParty.Form({ uploadDir: './static' });
            form.on('file', (name, file) => {
                const originalFilename = file.originalFilename;             // Исходное имя файла
                const tempPath = file.path;                                 // Временный путь
                const targetPath = path.join('./static', originalFilename); // Целевой путь

                fs.rename(tempPath, targetPath, (err) => {
                    if (err) {
                        console.error('Error moving file:', err);
                        res.end('<h1>Error moving file.</h1>');
                        return;
                    }
                    console.log(`File saved as: ${targetPath}`);
                });
            });
            form.on('error', err => { res.end(`<h1>Form returned error: ${err}</h1>`); });
            form.on('close', () => {
                res.end('<h1>File successfully uploaded.</h1>');
            });
            form.parse(req);
        }
        else {
            res.writeHead(408, 'Incorrect method', { 'Content-Type': 'text/html; charset=utf-8' });
            res.end('<h1>408: Incorrect method</h1>');
        }
    }
}

exports.Service = Service;