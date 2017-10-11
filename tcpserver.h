#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QDebug>
#include <QObject>
#include <QDateTime>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkSession>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QStringList>
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QAudioOutput>

class TcpServer : QObject
{
    Q_OBJECT
public:
    TcpServer();
    virtual ~TcpServer();

    QAudioOutput *m_audioOutput;
    QIODevice *m_dev;
private:
    QTcpServer* tcpServer;
    QNetworkSession* networkSession;
private:
    void startConnection(void);
    void sendData(QTcpSocket* client, QByteArray reply);
private slots:
    void newClient(void);
    void readData(void);
};

#endif // TCPSERVER_H
