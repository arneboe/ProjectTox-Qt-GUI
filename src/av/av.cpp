#include "av.hpp"

#include <QThread>
#include <QIODevice>

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

    audioInput = new AudioInput(format);
    audioInput->moveToThread(audioThread);
    connect(audioThread, &QThread::started, audioInput, &AudioInput::start);

    audioOutput = new AudioOutput(format);
    audioOutput->moveToThread(audioThread);   
    connect(audioThread, &QThread::started, audioOutput, &AudioOutput::start);
}

AV::~AV()
{
    delete audioThread;
}

void AV::start()
{
    audioThread->start();
    connect(audioInput->getDevice(), &QIODevice::bytesWritten, this, &AV::handleAudioInput);
}

void AV::handleAudioInput(int nbytes)
{
    QByteArray input(nbytes, 0);
    nbytes = audioInput->getDevice()->read(input.data(), nbytes);
    audioOutput->getDevice()->write(input.data(), nbytes);
}
