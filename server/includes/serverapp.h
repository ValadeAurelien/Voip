#ifndef SERVERAPP_H
#define SERVERAPP_H
#include "socketmembersnames.h"
#include "identity.h"
#include "datagram.h"
#include <QUdpSocket>
#include <QHostAddress>

class ServerApp : public QObject
{
  Q_OBJECT
  public : 
    ServerApp();

  private slots :
    void bindingFailed(QAbstractSocket::SocketError);
    void treatInDatagram();

  private :
    void coutMessage();
    void coutIdentity();
    void reportError(QString err);

    QHostAddress host1address,
                 host2address;
    quint16 host1port,
            host2port;
    Identity host1identity,
             host2identity;
    quint64 host1dginsize,
            host2dginsize;
    QByteArray ba_host1messagein,
               ba_host2messagein;

    static const quint16 port = 54321;
    QUdpSocket udpsocket;

    SocketErrorName socketerrorname;
    DatagramHD indatagram;
};


#endif
