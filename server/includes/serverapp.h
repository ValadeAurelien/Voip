#ifndef SERVERAPP_H
#define SERVERAPP_H
#include "socketmembersnames.h"
#include <QUdpSocket>
#include <QHostAddress>

class ServerApp : public QObject
{
  Q_OBJECT
  public : 
    ServerApp();
    void listenAndCout();

  private slots :
    void bindingFailed(QAbstractSocket::SocketError);
    void coutMessage();

  private :
    void reportError(QString err);

    QHostAddress host1address,
                 host2address;
    quint16 host1port,
            host2port;

    quint64 host1dginsize,
            host2dginsize;

    QByteArray ba_host1messagein,
               ba_host2messagein;

    static const quint16 port = 54321;
    QUdpSocket udpsocket;

    SocketErrorName socketerrorname;
};


#endif
