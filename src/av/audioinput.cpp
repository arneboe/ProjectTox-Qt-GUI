#include "audioinput.hpp"

AudioInput::AudioInput(const QAudioFormat& format, QObject *parent)
: QAudioInput(format, parent)
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
}
