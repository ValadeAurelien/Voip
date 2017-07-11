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
    void sendMessage();
    void attemptConnectToHost();
    void connectionFailed(QAbstractSocket::SocketError SErr);
    void sendIdentity();
    
    inline void showMainWindow() { mainwindow.show(); }
    inline void showConnectionWindow() { connectionwindow.show(); }
    inline void showCryptoWindow() { cryptowindow.show(); }

    inline const QHostAddress& get_hostaddress() { return hostaddress; }
    inline const char* get_socketstatename() { return socketstatename(udpsocket.state()); }
    inline const quint16& get_port() { return port; }

  signals :
    void messageSent();

  private :
    bool sendOutDatagram();
    MainWindow mainwindow;
    ConnectionWindow connectionwindow;
    CryptoWindow cryptowindow;
    AlertWindow alertwindow;

    OutMessage outmessage;
    InMessage inmessage;

    static const size_t datagramsize = DATAGRAMSIZE;
    QUdpSocket udpsocket;
    QHostAddress hostaddress;
    quint16 port;

    SocketErrorName socketerrorname;
    SocketStateName socketstatename;

    static const unsigned delayconnection = 3000;

    Identity identity;
    DatagramHD outdatagram;
};

#endif
