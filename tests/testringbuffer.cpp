#include "catch.hpp"
#include "../src/av/ringbuffer.hpp"
#include <QByteArray>
#include <QRunnable>
#include <QThreadPool>

TEST_CASE( "empty buffer should not be readable", "[ringbuffer]" )
{
    Ringbuffer<42> buffer;
    REQUIRE_FALSE(buffer.isReadable());
}

TEST_CASE( "empty buffer should be at end", "[ringbuffer]" )
{
    Ringbuffer<42> buffer;
    REQUIRE(buffer.atEnd());
}

TEST_CASE( "simple write and read", "[ringbuffer]" )
{
    Ringbuffer<5> buffer;
    buffer.open(QIODevice::ReadWrite);
    char data[5] = {0, 1, -1, -128, 127};
    REQUIRE(5 == buffer.write(&data[0], 5));

    char readData[5] = {0};
    REQUIRE(5 == buffer.read(&readData[0], 5));
    for(int i = 0; i < 5; ++i)
    {
        REQUIRE(data[i] == readData[i]);
    }

    //same test but with readAll()
    REQUIRE(5 == buffer.write(&data[0], 5));
    QByteArray readArr = buffer.readAll();
    REQUIRE(readArr.size() == 5);
    for(int i = 0; i < 5; ++i)
    {
        REQUIRE(data[i] == readArr[i]);
    }
}

TEST_CASE( "atEnd", "[ringbuffer]" )
{
    Ringbuffer<1> buffer;
    buffer.open(QIODevice::WriteOnly);
    char data = 42;
    REQUIRE(1 == buffer.write(&data, 1));
    REQUIRE(!buffer.atEnd());
}

TEST_CASE( "write/read more than size", "[ringbuffer]" )
{
    Ringbuffer<1> buffer;
    buffer.open(QIODevice::ReadWrite);
    char data[2] = {42, 43};
    REQUIRE(1 == buffer.write(&data[0], 2));
    char readData = 0;
    REQUIRE(1 == buffer.read(&readData, 2));
    REQUIRE(readData == data[0]);
}


TEST_CASE("threaded read/write", "[ringbuffer]")
{
    class Writer : public QRunnable
    {
    public:
        Writer(Ringbuffer<40>& buff) : buff(buff)
        {
            setAutoDelete(false);
        }
        void run()
        {
            for(int i = 0; i < 10000; ++i)
            {
                int v = 42;
                int result = buff.write((char*)&v, sizeof(int));
            }
        }
        Ringbuffer<40>& buff;
    };

    class Reader : public QRunnable
    {
    public:
        Reader(Ringbuffer<40>& buff) : buff(buff)
        {
            setAutoDelete(false);
        }
        void run()
        {
            for(int i = 0; i < 10000; ++i)
            {
                int result;
                int size = buff.read((char*)&result, sizeof(int));
                //whole ints are written => we should always be able to read whole ints or nothing
                if(size == sizeof(int))
                {
                    REQUIRE(result == 42);
                }
                else
                {
                    REQUIRE(size == 0);
                }
            }
        }
        Ringbuffer<40>& buff;
    };
    //Note: this whole test assumes that the size of the buffer is divisible by 4
    Ringbuffer<40> buff;
    buff.open(QIODevice::ReadWrite);
    Reader reader(buff);
    Writer writer(buff);

    QThreadPool *threadPool = QThreadPool::globalInstance();
    threadPool->start(&reader);
    threadPool->start(&writer);
    threadPool->waitForDone();
}





