#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>
#include <QUdpSocket>
#include <QByteArray>
#include <QHostAddress>
#include "windows.h"

struct SocketErrorName
{
  QString operator()(int i) const { return QString(errors[i+1]); } 
  char * errors [] = {"UnknownSocketError" ,
                      "ConnectionRefusedError" ,
                      "RemoteHostClosedError" ,
                      "HostNotFoundError" ,
                      "SocketAccessError" ,
                      "SocketResourceError" ,
                      "SocketTimeoutError" ,
                      "DatagramTooLargeError" ,
                      "NetworkError" ,
                      "AddressInUseError" ,
                      "SocketAddressNotAvailableError" ,
                      "UnsupportedSocketOperationError" ,
                      "ProxyAuthenticationRequiredError" ,
                      "SslHandshakeFailedError" ,
                      "UnfinishedSocketOperationError" ,
                      "ProxyConnectionRefusedError" ,
                      "ProxyConnectionClosedError" ,
                      "ProxyConnectionTimeoutError" ,
                      "ProxyNotFoundError" ,
                      "ProxyProtocolError" ,
                      "OperationError" ,
                      "SslInternalError" ,
                      "SslInvalidUserDataError" ,
                      "TemporaryError"};
};

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
  private :
    MainWindow mainwindow;
    ConnectionWindow connectionwindow;
    CryptoWindow cryptowindow;
    AlertWindow alertwindow;

    QUdpSocket udpsocket;
    QHostAddress hostaddress;
    qint16 port;

    SocketErrorName socketerrorname;
};

#endif
