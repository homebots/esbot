#include <Arduino.h>
#include <string.h>
#include <WebSocketsClient.h>

void pingBack(WebSocketsClient *webSocket, byte* payload) {
  webSocket->sendTXT("pong");
}

/*ledWrite(WebSocketsClient *webSocket, byte payload) {
  if (payload[0] === "1") {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

/*if (strcmp(command, "DRD")) {
        webSocket.sendTXT("Read digital");
        webSocket.sendTXT((char *) digitalRead(0));
        return;
      }

      if (strcmp(command, "DWR")) {
        webSocket.sendTXT("Write digital");
        digitalWrite(0, argument);
        return;
      }

      if (strcmp(command, "ARD")) {
        webSocket.sendTXT("Read analog");
        webSocket.sendTXT((char *) analogRead(0));
        return;
      }

      if (strcmp(command, "AWR")) {
        webSocket.sendTXT("Write digital");
        analogWrite(0, argument);
        return;
      }

      if (strcmp(command, "P0R")) {
        webSocket.sendTXT("Pin 0 read");
        pinMode(0, INPUT);
        return;
      }

      if (strcmp(command, "P0W")) {
        webSocket.sendTXT("Pin 0 write");
        pinMode(0, OUTPUT);
        return;
      }

      if (strcmp(command, "P2R")) {
        webSocket.sendTXT("Pin 2 read");
        pinMode(2, INPUT);
        return;
      }

      if (strcmp(command, "P2W")) {
        webSocket.sendTXT("Pin 2 write");
        pinMode(2, OUTPUT);
        return;
      }

      webSocket.sendTXT("INVALID COMMAND");
      webSocket.sendTXT(command); */
