#include "audiooutput.hpp"

AudioOutput::AudioOutput(const QAudioFormat format, QObject *parent) :
    QAudioOutput(format, parent)
{
}

QIODevice* AudioOutput::start()
{
    device = QAudioOutput::start();
    return device;
}

void AudioOutput::output(Ringbuffer* buffer)
{
    Ringbuffer::chunk_t chunk;
    if(buffer->pop(chunk)) {
        device->write(chunk.data, chunk.size);
    }
}
