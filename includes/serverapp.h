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
    void treatIncomingIdentity();

  signals :
    void receivedClientIdentity();

  private :
    void completeAndAnswerIdentity(Identity& id);
    void sendIdentityToIdentity(const Identity& id, const Identity& dest, const DatagramIdentityHeader& idh);
    bool sendOutDatagramToIdentity(const Identity& dest);

    void coutIdentity();
    void reportError(QString err);

    QHostAddress senderaddress;
    quint16 senderport;
    Identity selfidentity, 
             client1identity,
             client2identity;
 
    static const quint16 port = 54321;
    QUdpSocket socket;

    SocketErrorName socketerrorname;
    DatagramHD indatagram, 
               outdatagram;
};


#endif
