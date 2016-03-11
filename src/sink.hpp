#ifndef UUID_8EC8FF70_7F93_4281_9370_FF756B846775
#define UUID_8EC8FF70_7F93_4281_9370_FF756B846775
#pragma once

#include "fs.hpp"
#include "utils.hpp"
#include <boost/endian/arithmetic.hpp>
#include <cstring>

class Sink
{
public:
    virtual ~Sink() = default;
    static std::unique_ptr<Sink> ToFile(
        fs::path fname, FilePosition size, bool try_mmap = true);

    FilePosition Tell() const noexcept { return offset + buf_put; }

    template <typename T>
    void Write(const T& x)
    { Write(reinterpret_cast<const Byte*>(&x), sizeof(T)); }
    void Write(const Byte* data, FileMemSize len)
    {
        auto cp = std::min(len, buf_size - buf_put);
        memcpy(buf+buf_put, data, cp);
        data += cp;
        buf_put += cp;
        len -= cp;

        if (len) Write_(data, len);
    }

    void Pad(FileMemSize len)
    {
        auto cp = std::min(len, buf_size - buf_put);
        memset(buf+buf_put, 0, cp);
        buf_put += cp;
        len -= cp;

        if (len) Pad_(len);
    }

    virtual void Flush() {}

    void WriteLittleUint8 (boost::endian::little_uint8_t  i) { Write(i); }
    void WriteLittleUint16(boost::endian::little_uint16_t i) { Write(i); }
    void WriteLittleUint32(boost::endian::little_uint32_t i) { Write(i); }

protected:
    Sink(FileMemSize size) : size{size} {}

    Byte* buf = nullptr;
    FilePosition offset = 0, size;
    FileMemSize buf_put = 0, buf_size;

private:
    virtual void Write_(const Byte* buf, FileMemSize len) = 0;
    virtual void Pad_(FileMemSize len) = 0;
};

#endif