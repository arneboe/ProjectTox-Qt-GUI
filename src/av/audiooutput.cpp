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
