const http = require('http');
const fs = require('fs');
const WebSocket = require('ws');

const clients = {};

const server = http.createServer(function(request, response) {
  if (request.url === '/') {
    fs.createReadStream('./server.html').pipe(response);
    return;
  }

  if (fs.existsSync('.' + request.url)) {
    fs.createReadStream('.' + request.url).pipe(response);
    return;
  }

  response.writeHead(404);
  response.end('Not found');
});


const webSocket = new WebSocket.Server({
  port: 80,
  server
});

server.listen(8080);
console.log('Server running at http://localhost:8080/')
console.log('WebSockets running at http://localhost:80/')

webSocket.on('connection', function(connection) {
  connection.on('message', function(data) {
    if (data === 'bot') {
      console.log('bot connected');
      clients.bot = connection;

      if (clients.web) {
        clients.web.send('Bot connected');
      }

      return;
    }

    // console.log(data);

    try {
      if (data === 'C! web') {
        console.log('web connected');
        clients.web = connection;
        return;
      }

      // web > bot
      if (data.startsWith('C!')) {
        const command = data.slice(3).trim();
        console.log(`send to bot: "${command}"`);

        if (clients.bot) {
          clients.bot.send(command);
        }
        return;
      }

      // bot > web
      if (clients.web) {
        console.log(`send to web: "${data}"`);
        clients.web.send(data);
      }
    } catch (e) {
      console.error(e.message);
    }
  });
});