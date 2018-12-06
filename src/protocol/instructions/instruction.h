#ifndef _BOT_PROTOCOL_H_
#define _BOT_PROTOCOL_H_

typedef void (*Callback)(unsigned char output);

class Instruction {
  public:
    unsigned char id;
    virtual void run() {}
};

class NoopInstruction: public Instruction {
  public:
    void run(Callback callback) {
      callback(1);
    }
};

#endif