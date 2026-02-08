const http = require('http');
const url = require('url');
const WebSocket = require('ws');
const rpcServer = require('rpc-websockets').Server;
const serverWS = new WebSocket.Server({ port: 4000, host: 'localhost' });
const serverRPC = new rpcServer({ port: 3000, host: 'localhost' });
const server = http.createServer();
const mod = require('./m12')(serverWS);



let http_handler = (req, res) => {
    const regexNumber = new RegExp('^[0-9]+$');
    const regexBackupDate = new RegExp('^/backup/20[0-9]{2}[0-3]{1}[0-9]{1}[0-1]{1}[0-9]{1}$');

    let fullPathName = url.parse(req.url).pathname;
    let pathName = url.parse(req.url).pathname.split('/')[1];
    let method = req.method;
    console.log('full pathName: \t' + fullPathName + '\npathName: \t' + pathName + '\nmethod: \t' + method + '\n');



    if (pathName === '') {
        switch (method) {
            case 'GET': mod.GetAllStudents(req, res); break;
            case 'POST': mod.InsertStudent(req, res); break;
            case 'PUT': mod.UpdateStudent(req, res); break;
            default: mod.ErrorIncorrectMethod(res); break;
        }
    }

    //  pathname:   /n
    else if (regexNumber.test(pathName)) {
        switch (method) {
            case 'GET': mod.GetStudentById(req, res); break;
            case 'DELETE': mod.DeleteStudentById(req, res); break;
            default: mod.ErrorIncorrectMethod(res); break;
        }
    }

    //  pathname:   /backup/YYYYMMDD
    else if (pathName === 'backup' && regexBackupDate.test(fullPathName)) {
        (method === 'DELETE') ? mod.DeleteOldBackups(req, res) : mod.ErrorIncorrectMethod(res);
    }

    //  pathname:   /backup
    else if (fullPathName === '/backup') {
        switch (method) {
            case 'GET': mod.GetAllBackups(req, res); break;
            case 'POST': mod.BackupStudentList(req, res); break;
            default: mod.ErrorIncorrectMethod(res); break;
        }
    }

    else {
        mod.ErrorIncorrectURL(res);
    }
}



server.listen(5000, () => console.log('Server running at localhost:5000/\n'))
    .on('request', http_handler)
    .on('error', error => { console.log('[FATAL] ' + error.message); });