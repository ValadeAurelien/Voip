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
#define MAXSIZEMESSSAGEIN 262144

class Application : public QObject
{
  Q_OBJECT
  public : 
    Application();
  public slots:
    void attemptConnectToHost();
    void attemptConnectToPeer();
    void connectionFailed(QAbstractSocket::SocketError SErr);

    void sendMessageToPeer();
    void sendIdentityToHost();
    
    inline void showMainWindow() { mainwindow.show(); }
    inline void showConnectionWindow() { connectionwindow.show(); }
    inline void showCryptoWindow() { cryptowindow.show(); }

    inline const QHostAddress& getSelfAddress() { return selfaddress; }
    inline const quint16& getSelfPort() { return selfport; }
    inline const QHostAddress& getHostAddress() { return hostaddress; }
    inline const quint16& getHostPort() { return hostport; }
    inline const QHostAddress& getPeerAddress() { return peeraddress; }
    inline const quint16& getPeerPort() { return peerport; }
    inline const QString& getNewMessage() { return inmessage.getMessage(); }
    inline const char* getHostSocketStateName() { return socketstatename(hostsocket.state()); }
    inline const char* getPeerSocketStateName() { return socketstatename(peersocket.state()); }

  private slots : 
    void treatInDatagram();

  signals :
    void messageSent();

  private :
    void reportError(QString err);

    bool sendOutDatagram(QUdpSocket& socket);
    bool sendOutDatagramToHost();
    bool sendOutDatagramToPeer();

    void setHostIdentity();
    void updateIdentity();
    void setPeerIdentity();

    MainWindow mainwindow;
    ConnectionWindow connectionwindow;
    CryptoWindow cryptowindow;
    AlertWindow alertwindow;

    OutMessage outmessage;
    InMessage inmessage;

    static const size_t datagramsize = DATAGRAMSIZE;
    QUdpSocket hostsocket,
               peersocket;

    QHostAddress selfaddress,
                 hostaddress,
                 peeraddress;
    quint16 selfport,
            hostport,
            peerport;
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
