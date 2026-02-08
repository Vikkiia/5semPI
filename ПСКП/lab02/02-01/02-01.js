const http = require('http');
const fs = require('fs');

const server = http.createServer(function (request, response) {
    if (request.url === '/html') {
        fs.readFile('./index.html', function(err, data) {
            if (err) {
                response.writeHead(404, {'Content-Type': 'text/plain'});
                response.end('file not found'); 
            } else {
                response.writeHead(200, {'Content-Type': 'text/html'});
                response.end(data); 
            }
        });
    } else {
        response.writeHead(404, {'Content-Type': 'text/plain'});
        response.end('file not found'); 
    }
});

server.listen(5000, function(){
    console.log('Server running at http://localhost:5000/html');
})