const http=require('http');
const url = require('url');

function factorial(k) {
    if (k == 0)
        return 1;
    else
        return k * factorial(k - 1);
}

http.createServer(function (req, res) {
    var url_path = url.parse(req.url, true);      
    var query = url_path.query;                            
    var k = query.k;                                 
    var fact_value = factorial(k);
    res.writeHead(200, {'Content-Type': 'application/json'});
    res.end(JSON.stringify({k: k, fact: fact_value}));
}).listen(5000);

console.log('Start server at http://localhost:5000/fact?k=3');