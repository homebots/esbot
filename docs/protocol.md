# Binary protocol

## Design goals

- Remote control over pins
- Read/Write arbitrary values in digital/analog mode

## Encoding

Each message sent to esbot has the shape of:

| Instruction Code | ...arguments |
|--|--|
| 0x00..0xff | ... 0x00..0xff |

## Data types

| type | description |
|--|--|
| byte    | 0..255 |
| bool    | 0 or 1 |
| string     | byte[] |
| number    | 0..65535 |

## Instructions

| Code | Command | Arguments | Description |
|--|--|--|--|
| 0x01 | NOOP           | NULL | no-op signal to test connection |
| 0x02 | RESET          | NULL | reset board |
| 0x03 | PING           | NULL | ping the ESP and asks for a pong |
| 0x04 | WRITE          | `byte` pin, `bool` on | flip a pin to on or off |
| 0x05 | READ           | `byte` pin, `bool` analog | read digital value of pin |
| 0x06 | ANALOG_WRITE   | `byte` pin, `number` value | write analog value to pin |
| 0x07 | READ_STREAM    | `byte` pin, `byte` interval, `number` times, `number` bufferSize | read `samples` samples from `pin`, every `interval` ms, sending a new message every `bufferSize` samples |
| 0x08 | WRITE_STREAM   | `byte` pin, `byte` interval, `number` length, `string` buffer | read `samples` samples from `pin`, every `interval` ms, sending a new message every `bufferSize` samples |
| 0x09 | STOP_STREAM    | `byte` pin | stop streaming from `pin` |
| 0x0a | DEBUG          | `bool` on | start/stop Serial debug messages |

# Responses

| Instruction | Code | Arguments | Description |
|--|--|--|--|
| PING | 0x03 | string[18] | Board's MAC Address |


