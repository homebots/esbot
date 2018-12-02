(function () {

  function stringToBuffer(string) {
    const length = string.length;
    const buffer = new Uint8Array(length);

    for (let i = 0; i < length; i++) {
      buffer[i] = string.charCodeAt(i);
    }

    return buffer;
  }

  function addTimestamp(text) {
    if (!text) return '';

    const now = new Date();
    return `${now.getHours()}:${now.getMinutes()} ${text}`;
  }

  const BotInstruction = {
    Noop: 0x01,
    Reset: 0x02,
    Ping: 0x03,
    Write: 0x04,
    Read: 0x05,
    AnalogWrite: 0x06,
    AnalogRead: 0x07
  };


  class BotProtocol {
    constructor(client) {
      this.client = client;
    }

    ping() {
      this.sendBytes([2]);
    }

    reset() {
      this.sendBytes([1]);
    }

    read(pin) {
      pin = Number(pin);
      const buffer = new Uint8Array([BotInstruction.Read, pin]);
      this.client.sendBuffer(buffer);
    }

    write(pin, value) {
      pin = Number(pin);
      value = Number(value);
      const buffer = new Uint8Array([BotInstruction.Read, pin, value ? 1 : 0]);
      this.client.sendBuffer(buffer);
    }
  }

  class BotClient {
    constructor(config) {
      Object.assign(this, config);
      this.connect();
      setInterval(() => this.socket || this.connect(), this.reconnectInterval || 5000);
    }

    connect() {
      const socket = new WebSocket(this.url);
      socket.binaryType = 'arraybuffer';

      socket.onmessage = (event) => this.onMessage(event.data);
      socket.onopen = () => this.onSocketOpen();
      socket.onclose = () => this.onSocketClose();

      this.socket = socket;
    }

    onSocketOpen() {
      this.socket.send('!!web');
    }

    onSocketClose() {
      this.socket = null;
    }

    onMessage(message) {}

    sendString(string) {
      const buffer = stringToBuffer(string);
      this.sendBuffer(buffer);
    }

    sendBuffer(byteArray) {
      this.socket.send(byteArray.buffer);
    }

    sendBytes(bytes) {
      const length = bytes.length;
      const buffer = new Uint8Array(length);

      for (let i = 0; i < length; i++) {
        buffer[i] = bytes[i];
      }

      this.sendBuffer(bytes);
    }
  }

  document.addEventListener('DOMContentLoaded', () => {
    const stdout = document.querySelector('#output');
    const client = new BotClient({ url: 'ws://localhost:81/client'});
    client.onMessage = (message) => stdout.innerText += addTimestamp(message + '\n');
    const botProtocol = window.botProtocol = new BotProtocol(client);

    window.writeToPin = function() {
      const pin = window.pinNumber.value;
      const value = window.pinValue.value;
      botProtocol.write(pin, value);
    };

    window.readPin = function() {
      const pin = window.pinNumber.value;
      botProtocol.read(pin);
    };

  });
})();