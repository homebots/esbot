(function () {
  const esbot = window.esbot = {
    socket: null,
    stdout: null,

    init() {
      esbot.stdout = document.querySelector('#output');
    },

    output(message) {
      if (!esbot.stdout) return;
      esbot.stdout.innerText += addTimestamp(message + '\n');
    },

    send(message) {
      if (!esbot.socket) return;
      esbot.socket.send(`C! ${message}`);
    },

    ping() {
      esbot.send('ping');
    },

    doSubmit (event) {
      event.preventDefault();

      const command = window.commandInput.value.trim();

      if (command) {
        esbot.send(command);
        window.commandInput.value = '';
      }
    },

    doToggle(pin) {
      esbot.send(`${pin}`);
    },

    connect() {
      const socket = new WebSocket('ws://localhost:80');
      esbot.socket = socket;

      socket.onmessage = (event) => {
        const message = event.data;
        if (message === 'bot ping') {
          esbot.ping();
          esbot.output('pong');
          return;
        }

        esbot.output(message);
      }

      socket.onopen = () => {
        esbot.output('Connected');
        esbot.send('web');
      };

      socket.onclose = () => {
        esbot.output('Disconnected');
        esbot.socket = null;
      };
    }
  };

  function addTimestamp(text) {
    if (!text) return '';

    const now = new Date();
    return `${now.getHours()}:${now.getMinutes()} ${text}`;
  }

  document.addEventListener('DOMContentLoaded', () => {
    esbot.init();
    esbot.connect();

    setInterval(() => esbot.socket || esbot.connect(), 5000);
  });
})();