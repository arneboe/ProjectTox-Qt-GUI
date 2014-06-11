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

TEST_CASE("negative size", "[ringbuffer]")
{
    Ringbuffer<13> buffer;
    buffer.open(QIODevice::ReadWrite);
    char data;
    REQUIRE(-1 == buffer.write(&data, -10));//-1 should be returned in case of error
    REQUIRE(buffer.bytesAvailable() == 0);//nothing should have been written
}

TEST_CASE("zero size", "[ringbuffer]")
{
    Ringbuffer<13> buffer;
    buffer.open(QIODevice::ReadWrite);
    char data;
    REQUIRE(0 == buffer.write(&data, 0));
    REQUIRE(buffer.bytesAvailable() == 0);//nothing should have been written
}


TEST_CASE("complex read write", "[ringbuffer]")
{
    Ringbuffer<5> buffer;
    buffer.open(QIODevice::ReadWrite);

    QByteArray nothing = buffer.read(10);
    REQUIRE(0 == nothing.size());
    char data[5] = {42, 43, 44, 45, 46};
    REQUIRE(5 == buffer.write(&data[0], 5));
    //now the buffer should be full
    REQUIRE(!buffer.isWritable());
    char read[3] = {-1};
    REQUIRE(3 == buffer.read(&read[0], 3));
    REQUIRE(read[0] == data[0]);
    REQUIRE(read[1] == data[1]);
    REQUIRE(read[2] == data[2]);
    REQUIRE(buffer.isWritable());

    REQUIRE(3 == buffer.write(&data[0], 5));
    char read5[5] = {-1};
    REQUIRE(5 == buffer.read(&read5[0], 5));
    REQUIRE(!buffer.isReadable());
    REQUIRE(read5[0] == data[3]);
    REQUIRE(read5[1] == data[4]);
    REQUIRE(read5[2] == data[0]);
    REQUIRE(read5[3] == data[1]);
    REQUIRE(read5[4] == data[2]);


}


/**
 *A simple runnable that writes data to a Ringbuffer
 */
template <int T>
class Writer : public QRunnable
{
public:
    Writer(Ringbuffer<T>& buff) : buff(buff)
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
    Ringbuffer<T>& buff;
};

/**
 *A simple runnable that reads data from a Ringbuffer
 */
template <int T>
class Reader : public QRunnable
{
public:
    Reader(Ringbuffer<T>& buff) : buff(buff)
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
    Ringbuffer<T>& buff;
};

TEST_CASE("threaded read/write", "[ringbuffer]")
{
    static const int SIZE = 10 * sizeof(int);
    //Note: this whole test assumes that the size of the buffer is divisible by sizeof(int)
    Ringbuffer<SIZE> buff;
    buff.open(QIODevice::ReadWrite);
    Reader<SIZE> reader(buff);
    Writer<SIZE> writer(buff);

    QThreadPool *threadPool = QThreadPool::globalInstance();
    threadPool->start(&reader);
    threadPool->start(&writer);
    threadPool->waitForDone();
}





