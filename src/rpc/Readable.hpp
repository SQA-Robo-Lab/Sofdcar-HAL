#ifndef READBLE_HPP
#define READBLE_HPP

#include "Arduino.h"

class Readable
{
public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
};

class ReadableStream : public Readable
{
protected:
    Stream &stream;

public:
    ReadableStream(Stream &streamBackend) : stream(streamBackend) {}
    int available() { return this->stream.available(); };
    int read() { return this->stream.read(); };
    int peek() { return this->stream.peek(); };
};

class ReadableString : public Readable
{
protected:
    const char *string;
    uint16_t index = 0;
    uint16_t len = 0;

public:
    ReadableString(const char *text, uint16_t strLen) : string(text), len(strLen)
    {
        this->len = min(min(strLen, strlen(text)), 65535);
    }

    int available() { return this->index < this->len; };
    int read() { return this->string[this->index++]; };
    int peek() { return this->string[this->index]; };
};

#endif