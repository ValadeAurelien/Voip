#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>
#include <QUdpSocket>
#include <QByteArray>
#include <QHostAddress>
#include "windows.h"
#include "socketmembersnames.h"
#define MAXSIZEMESSSAGEIN 262144
#define DATAGRAMSIZE 512


class Application : public QObject
{
  Q_OBJECT
  public : 
    Application();
  public slots:
    void sendMessage();
    void attemptConnectToHost();
    void connectionFailed(QAbstractSocket::SocketError SErr);
    
    inline void showMainWindow() { mainwindow.show(); }
    inline void showConnectionWindow() { connectionwindow.show(); }
    inline void showCryptoWindow() { cryptowindow.show(); }

    inline const QHostAddress& get_hostaddress() { return hostaddress; }
    inline const char* get_socketstatename() { return socketstatename(udpsocket.state()); }
    inline const quint16& get_port() { return port; }

  signals :
    void messageSent();

  private :
    MainWindow mainwindow;
    ConnectionWindow connectionwindow;
    CryptoWindow cryptowindow;
    AlertWindow alertwindow;

    QByteArray ba_messagein;
    static const size_t maxsize_messagein = MAXSIZEMESSSAGEIN;
    int nbdg2send_messagein,
        nbdgsent_messagein, 
        totnbdatagramsent_messagein;

    static const size_t datagramsize = DATAGRAMSIZE;
    QUdpSocket udpsocket;
    QHostAddress hostaddress;
    quint16 port;

    SocketErrorName socketerrorname;
    SocketStateName socketstatename;

    static const unsigned delayconnection = 3000;
};

#endif
