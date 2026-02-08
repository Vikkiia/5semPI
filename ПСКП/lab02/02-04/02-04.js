const http = require("http");
const fs = require("fs");


http.createServer(function (req, res){
    if(req.url === "/xmlhttprequest"){
        fs.readFile("xmlhttprequest.html", function (err, data){
            if(err)
            {
                res.writeHead(500, { "Content-Type": "text/plain" });
                res.end("Server Error");
            }
            else
            {
                res.writeHead(200, { "Content-Type": "text/html" });
                res.end(data);
            }

        })
    }

   else if (req.url === "/api/name" && req.method === "GET") {
        res.writeHead(200, { "Content-Type": "text/plain" });
        res.end("Borisova Victoria Vasil'evna");
    }

    else {
        res.writeHead(404, { "Content-Type": "text/plain" });
        res.end("Page not found");
    }
}).listen(5000);

console.log('Server running at http://localhost:5000/xmlhttprequest');