#include "audioinput.hpp"

#include <QDebug>

AudioInput::AudioInput(const QAudioFormat& format, QObject *parent)
: QAudioInput(format, parent)
{
}

QIODevice* AudioInput::start()
{
    device.open(QIODevice::ReadWrite);
    QAudioInput::start(&device);
    return &device;
}

QIODevice* AudioInput::getDevice()
{
    return &device;
}
