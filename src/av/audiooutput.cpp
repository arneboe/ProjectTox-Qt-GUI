#include "audiooutput.hpp"

AudioOutput::AudioOutput(const QAudioFormat format, QObject *parent) :
    QAudioOutput(format, parent)
{
}

QIODevice* AudioOutput::start()
{
    device.open(QIODevice::ReadWrite);
    QAudioOutput::start(&device);
    return &device;
}

QIODevice* AudioOutput::getDevice()
{
    return &device;
}
