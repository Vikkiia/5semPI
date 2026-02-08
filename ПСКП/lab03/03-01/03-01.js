const http = require('http');
const fs = require('fs');
const readline = require('readline');


const states = ['norm', 'stop', 'test', 'idle'];
let currentState = 'norm';


const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});


function handleInput(input) {
    input = input.trim();
    
    if (states.includes(input)) {
        const prevState = currentState;
        currentState = input;
        console.log(`reg = ${prevState}--> ${currentState}`);
        rl.setPrompt(`${currentState}->`); 
        rl.prompt(); 
    } else if (input === 'exit') { 
        console.log('Завершение');
        rl.close(); 
        process.exit(0); 
    } else {
         if (input.length > 0) {
            console.log(input); 
        }
        rl.prompt();   
    }

}


rl.on('line', (input) => {
    handleInput(input);
});

const server = http.createServer((req, res) => {
    fs.readFile('index.html', 'utf8', (err, data) => {
        if (err) {
            console.error('Ошибка при чтении файла:', err);
            res.writeHead(500, { 'Content-Type': 'text/plain; charset=utf-8' });
            res.end('Ошибка при чтении файла.');
            return;
        }

        const responseHtml = data.replace(/{{state}}/g, currentState);
        
        res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
        res.end(responseHtml);
    });
});

server.listen(5000, () => {
    console.log('Сервер запущен по адресу: http://localhost:5000');
    rl.setPrompt(`${currentState}->`);
    rl.prompt(); 
});