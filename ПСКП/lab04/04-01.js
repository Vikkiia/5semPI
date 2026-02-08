import { User, DB, database } from "./DB.js";
import { createServer } from 'http'
import * as url from "url";
import { EventEmitter } from "events";

class MyEmitter extends EventEmitter {}
const myEmitter = new MyEmitter();

myEmitter.on("GET", async (req, res) => {
    const data = await DB.select(database);
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify(data));
});

myEmitter.on("POST", async (req, res)=>{
    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });
    req.on('end', async() => {
        const newRow = await DB.insert(JSON.parse(body));
        res.writeHead(201, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(newRow));
    });
})

myEmitter.on("PUT", async (req, res)=>{
    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });
    req.on('end', async () => {
        const newData = JSON.parse(body);
        const updatedRow = await DB.update(newData);
        if (updatedRow) {
            res.writeHead(200, {'Content-Type': 'application/json'});
            res.end(JSON.stringify(updatedRow));
        } else {
            res.writeHead(404, {'Content-Type': 'application/json'});
            res.end(JSON.stringify({message: 'Row not found'}));
        }
    });
})

myEmitter.on("DELETE", async (req, res) => {
    const reqUrl = url.parse(req.url, true);
    const idD = reqUrl.query.id;
    if (idD) {
        const deletedRow = await DB.delete(idD);
        if (deletedRow) {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify(deletedRow));
        } else {
            res.writeHead(404);
            res.end('Row not found');
        }
    } else {
        res.writeHead(400);
        res.end('Bad Request: ID is required');
    }
});

createServer(async function (req, res) {
    if (req.url.startsWith("/api/db")) {
        switch (req.method) {
            case "GET": {
                myEmitter.emit("GET", req, res);
                break;
            }
            case "PUT": {
                myEmitter.emit("PUT", req, res);
                break;
            }
            case "POST": {
                myEmitter.emit("POST", req, res);
                break;
            }
            case "DELETE": {
                myEmitter.emit("DELETE", req, res);
                break;
            }
        }
    }

    else {
        res.writeHead(404);
        res.end('Page not found');
    }
}).listen(5000);

console.log('http://localhost:5000/api/db');