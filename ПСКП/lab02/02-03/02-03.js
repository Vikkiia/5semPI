var http = require("http");


http.createServer(function (req, res){
    if (req.url === "/api/name" && req.method === "GET") {
        res.writeHead(200, { "Content-Type": "text/plain" });
        res.end("Borisova Victoria Vasil'evna");
    }
    else
    {
        res.writeHead(404, { "Content-Type": "text/plain" });
        res.end("Page not found");
    }

}).listen(5000);

console.log('Server running at http://localhost:5000/api/name');