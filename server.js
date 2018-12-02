const http = require('http');
const fs = require('fs');
const WebSocket = require('ws');

const clients = {};

const httpServer = http.createServer(function(request, response) {
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

httpServer.listen(8080);

const botSocket = new WebSocket.Server({
  port: 80,
  httpServer
});

const clientSocket = new WebSocket.Server({
  port: 81,
  httpServer,
  path: '/client'
});

console.log('Server running at http://localhost:8080/')
console.log('WebSockets running at http://localhost:80/')

botSocket.on('connection', function(connection) {
  connection.on('message', function(data) {
    if (data === 'bot') {
      console.log('bot connected');
      clients.bot = connection;

      if (clients.web) {
        clients.web.send('Bot connected');
      }

      return;
    }

    if (clients.web) {
      console.log('send to web', data);
      clients.web.send(data);
    }
  });

  connection.on('close', () => clients.bot = null);
});

clientSocket.on('connection', function (connection) {
  connection.on('message', function(data) {
    try {
      if (data === '!!web') {
        console.log('web connected');
        clients.web = connection;
        return;
      }

      if (!clients.bot) {
        console.log(`no bots available"`);
        return;
      }

      console.log('send to bot', data);
      clients.bot.send(data);
    } catch (e) {
      console.error(e.message);
    }
  });

  connection.on('close', () => clients.web = null);
});