import { User, DB, database } from "./DB.js";
import { createServer } from 'http';
import * as url from "url";
import * as fs from 'fs';
import { EventEmitter } from "events";

class MyEmitter extends EventEmitter { };
const myEmitter = new MyEmitter();

let collectingStats = false;
let intervalRequestCount = 0;
let intervalCommitCount = 0;
let statistics = {
    start: '',
    finish: ''
};

let statTimeout = null;
let commitInterval = null;
let shutdownTimeout = null;

const server = createServer(async function (req, res) {
    const reqUrl = url.parse(req.url, true);
    if (req.url.startsWith("/api/db")) {
        if (collectingStats) intervalRequestCount++;
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
    } else if (req.url === "/api/ss") {
        // globalRequestCount++;
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({
            start: statistics.start || '',
            finish: statTimeout ? '' : statistics.finish || '',
            request: intervalRequestCount,
            commit: intervalCommitCount
        }));
    } else if (req.url === "/") {
        fs.readFile("index.html", function (err, data) {
            if (err) {
                res.writeHead(500, { "Content-Type": "text/plain" });
                res.end("Server error!");
            } else {
                res.writeHead(200, { "Content-Type": "text/html" });
                res.end(data);
            }
        });
    } else {
        res.writeHead(404);
        res.end('Page not found');
    }
});

myEmitter.on("GET", async (req, res) => {
    const data = await DB.select(database);
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify(data));
});

myEmitter.on("POST", async (req, res) => {
    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });
    req.on('end', async () => {
        const newRow = await DB.insert(JSON.parse(body));
        // globalCommitCount++;
        // intervalCommitCount++;
        res.writeHead(201, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(newRow));
    });
});

myEmitter.on("PUT", async (req, res) => {
    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });
    req.on('end', async () => {
        const newData = JSON.parse(body);
        const updatedRow = await DB.update(newData);
        if (updatedRow) {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify(updatedRow));
        } else {
            res.writeHead(404, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ message: 'Row not found' }));
        }
    });
});

myEmitter.on("DELETE", async (req, res) => {
    const reqUrl = url.parse(req.url, true);
    const idD = reqUrl.query.id;

    if (idD) {
        const deletedRow = await DB.delete(idD);
        if (deletedRow) {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify(deletedRow));
        } else {
            res.writeHead(404, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ message: 'Row not found' }));
        }
    } else {
        res.writeHead(400, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ message: 'Bad Request: ID is required' }));
    }
});

myEmitter.on("COMMIT", async () => {
    await DB.commit();
    if (collectingStats) intervalCommitCount++;
});


process.stdin.on('data', (input) => {
    const command = input.toString().trim().split(" ");
    const cmd = command[0];
    const arg = command[1] ? parseInt(command[1]) : null;

    switch (cmd) {
        case 'sd':
            if (shutdownTimeout)
                clearTimeout(shutdownTimeout);
            if (arg) {
                shutdownTimeout = setTimeout(() => {
                    console.log("Server shut down.");
                    server.close();
                }, arg * 1000);
                console.log(`Shutdown scheduled in ${arg} seconds.`);
            } else {
                shutdownTimeout = null;
                console.log("Shutdown cancelled.");
            }
            break;

        // case 'sc':
        //     if (commitInterval) clearInterval(commitInterval);
        //     if (arg) {
        //         commitInterval = setInterval(async () => {
        //             await DB.commit();
        //             globalCommitCount++;
        //             intervalCommitCount++;
        //         }, arg * 1000);
        //         commitInterval.unref();
        //         console.log(`Commit function started with interval ${arg} seconds.`);
        //     } else {
        //         clearInterval(commitInterval);
        //         commitInterval = null;
        //         console.log("Commit function stopped.");
        //     }
        //     break;

        case 'sc':
            if (commitInterval) clearInterval(commitInterval);
            if (arg) {
                commitInterval = setInterval(async () => {
                    myEmitter.emit("COMMIT");
                }, arg * 1000);
                commitInterval.unref();
                console.log(`Commit function started with interval ${arg} seconds.`);
            } else {
                clearInterval(commitInterval);
                commitInterval = null;
                console.log("Commit function stopped.");
            }
            break;

        case 'ss':
            if (statTimeout)
                clearTimeout(statTimeout);
            if (arg) {
                statistics.start = new Date().toISOString();
                statistics.finish = '';
                intervalRequestCount = 0;
                intervalCommitCount = 0;
                collectingStats = true;

                console.log(`Statistics collection started for ${arg} seconds.`);

                statTimeout = setTimeout(() => {
                    statistics.finish = new Date().toISOString();
                    collectingStats = false;
                    statTimeout = null;
                    console.log("Statistics collection finished.");
                    console.log(`Requests: ${intervalRequestCount}, Commits: ${intervalCommitCount}`);

                }, arg * 1000);
                statTimeout.unref();
            } else {

                if (collectingStats) {
                    statistics.finish = new Date().toISOString();
                }
                collectingStats = false;
                statTimeout = null;
                console.log("Statistics collection stopped.");
            }
            break;

        default:
            console.log("Unknown command.");
    }
});

server.listen(5000);
console.log('Server running at http://localhost:5000/db');