const http = require("http");
const fs = require("fs");

const server = http.createServer(function(request,response){
    if(request.url === '/png'){
        fs.readFile('./pic.png', function(err, data){
            if(err){
                response.writeHead(404, {'Content-Type': 'image/png'});
                response.end('image not found');
            }
            else{
                response.writeHead(200, {'Content-Type': 'image/png'});
                response.end(data);
            }
        });
    }
    else{
        response.writeHead(404, {'Content-Type': 'image/png'});
        response.end('image not found');
    }
});

server.listen(5000, function(){
    console.log('Server running at http://localhost:5000/png');
})
