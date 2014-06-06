#include "audioinput.hpp"

AudioInput::AudioInput(const QAudioFormat& format, QObject *parent)
: QAudioInput(format, parent), buffer(256)
{
}

QIODevice* AudioInput::start()
{
    device = QAudioInput::start();
    connect(device, &QIODevice::readyRead, this, &AudioInput::readChunk);
    return device;
}


void AudioInput::readChunk()
{
    const size_t n = bytesReady();
    // for some reason the number of bytes that are ready
    // to ready is 0
    if (n > 0) {
        Ringbuffer::chunk_t chunk;
        chunk.data = (char*) malloc(n * sizeof(char));
        chunk.size = device->read(chunk.data, n);
        if (buffer.push(chunk)) {
            emit inputRecorded(&buffer);
        } else {
            free(chunk.data);
        }
    }
}
