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
    void newIncomingIdentity();
    void completeAndAnswerIdentity(Identity& id);
    void coutIdentity();
    void reportError(QString err);

    QHostAddress senderaddress;
    quint16 senderport;
    Identity host1identity,
             host2identity;
    bool hashost1 = false,
         hashost2 = false;
 
    static const quint16 port = 54321;
    QUdpSocket udpsocket;

    SocketErrorName socketerrorname;
    DatagramHD indatagram, 
               outdatagram;
};


#endif
