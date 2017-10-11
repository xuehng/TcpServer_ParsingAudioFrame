#include "tcpserver.h"
#include <QAudioFormat>

TcpServer::TcpServer()
{
    tcpServer = new QTcpServer(this);
    m_dev = NULL;

    {
        QAudioFormat format;
        // Set up the format, eg.
        format.setSampleRate(8000);
        format.setChannelCount(1);
        format.setSampleSize(8);
        format.setCodec("audio/pcm");
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setSampleType(QAudioFormat::UnSignedInt);

        QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
        if (!info.isFormatSupported(format)) {
            qWarning() << "Raw audio format not supported by backend, cannot play audio.";
            return;
        }

        m_audioOutput = new QAudioOutput(format, this);
        //connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged2(QAudio::State)));

    }

    m_dev = m_audioOutput->start();

    startConnection();
}

TcpServer::~TcpServer()
{
    delete tcpServer;
}

void TcpServer::startConnection()
{
    if (!tcpServer->listen(QHostAddress::Any, 27015)) {
        qCritical() << "Unable to start the server: " << tcpServer->errorString();
        tcpServer->close();
        return;
    }

    QTextStream qOut(stdout);
    qOut << "The server is running on IP(s): ";

    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int c = 0; c < ipAddressesList.length(); c++) {
        if(ipAddressesList.at(c).toIPv4Address()){
            qOut << ipAddressesList.at(c).toString() << " ";
        }
    }

    qOut << "Port: " << tcpServer->serverPort() << endl;
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newClient()));
}

void TcpServer::sendData(QTcpSocket* myClient, QByteArray data)
{
    qDebug() << QString(data);
    myClient->write(data);
}

void TcpServer::newClient()
{
    QTcpSocket* tcpClient = tcpServer->nextPendingConnection();
    qDebug() << "Client connected: " << tcpClient->localAddress().toString();
    connect(tcpClient, SIGNAL(disconnected()), tcpClient, SLOT(deleteLater()));
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(readData()));
}

void TcpServer::readData()
{
    qWarning() << "TcpServer::readData";
    QTcpSocket* myClient = qobject_cast<QTcpSocket*>(sender());
    QByteArray data = myClient->readAll();
    //sendData(myClient, data);
    //instantiate a QAudioOutput
    //m_dev = m_audioOutput->start();
    m_dev->write(data,data.length());

}
