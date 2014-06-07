#include "av.hpp"

#include "audioinput.hpp"
#include "audiooutput.hpp"

#include <QThread>

AV::AV(Tox* tox, QObject *parent)
: QObject(parent), tox(tox)
{
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    audioThread = new QThread(this);

    AudioInput* audioInput = new AudioInput(format);
    audioInput->moveToThread(audioThread);

    AudioOutput* audioOutput = new AudioOutput(format);
    audioOutput->setVolume(0.2);
    audioOutput->moveToThread(audioThread);

    connect(audioThread, &QThread::started, audioInput, &AudioInput::start);
    connect(audioThread, &QThread::started, audioOutput, &AudioOutput::start);
}

AV::~AV()
{
    delete audioThread;
}

void AV::start()
{
    audioThread->start();
}
