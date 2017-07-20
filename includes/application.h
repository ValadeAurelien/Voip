#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>
#include <QUdpSocket>
#include <QByteArray>
#include <QHostAddress>
#include "windows.h"
#include "socketmembersnames.h"
#include "identity.h"
#include "datagram.h"
#include "message.h"

class Application : public QObject
{
  Q_OBJECT
  public : 
    Application(const Identity& _id);
  public slots:
    void attemptConnectToHost();
    void attemptConnectToPeer();
    void connectionFailed(QAbstractSocket::SocketError SErr);

    void sendMessageToPeer();
    void sendSelfIdentityToIdentity(const Identity& id) ;
    
    inline void showMainWindow() { mainwindow.show(); }
    inline void showConnectionWindow() { connectionwindow.show(); }
    inline void showCryptoWindow() { cryptowindow.show(); }

    inline const Identity& getIdentityHost() const { return hostidentity; }
    inline const Identity& getIdentitySelf() const { return selfidentity; }
    inline const Identity& getIdentityPeer() const { return peeridentity; }
    inline const QString& getNewMessage() const { return inmessage.getMessage(); }

  private slots : 
    void treatInDatagram();

    void confirmAndUpdateHostIdentity();
    void confirmAndUpdateSelfIdentity();
    void confirmAndUpdatePeerIdentity();

  signals :
    void messageSent();
    void waitingForConnectionToHost();
    void waitingForConnectionToPeer();

    void hostAnsweredHostIdentity();
    void hostAnsweredSelfIdentity();
    void hostAnsweredPeerIdentity();
    void peerAnsweredPeerIdentity();

    void hostIdentityConfirmed();
    void selfIdentityConfirmed();
    void peerIdentityConfirmed();

  private :
    void reportError(QString err);

    bool sendOutDatagramToIdentity(const Identity& id);
    bool sendOutDatagramToHost();
    bool sendOutDatagramToPeer();

    MainWindow mainwindow;
    ConnectionWindow connectionwindow;
    CryptoWindow cryptowindow;
    AlertWindow alertwindow;
    WaitWindow waitwindow;

    OutMessage outmessage;
    InMessage inmessage;

    static const size_t datagramsize = DATAGRAMSIZE;
//    QUdpSocket hostsocket,
//               peersocket;
//
//    QHostAddress selfaddress,
//                 hostaddress,
//                 peeraddress;
//    quint16 selfport,
//            hostport,
//            peerport;
    QUdpSocket socket;
    QHostAddress senderaddress;
    quint16 senderport;

    Identity selfidentity,
             hostidentity,
             peeridentity;

    SocketErrorName socketerrorname;
    SocketStateName socketstatename;

    DatagramHD outdatagram, 
               indatagram;

    static const unsigned delayconnection = 3000;
};

#endif
