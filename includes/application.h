#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>
#include <QUdpSocket>
#include <QByteArray>
#include <QHostAddress>
#include "windows.h"

struct SocketErrorName
{
  const char* operator()(int i) const { return errors[i+1]; } 
  const char* errors [24] = {"UnknownSocketError", "ConnectionRefusedError",
                              "RemoteHostClosedError", "HostNotFoundError",
                              "SocketAccessError", "SocketResourceError",
                              "SocketTimeoutError", "DatagramTooLargeError",
                              "NetworkError", "AddressInUseError",
                              "SocketAddressNotAvailableError",
                              "UnsupportedSocketOperationError",
                              "ProxyAuthenticationRequiredError",
                              "SslHandshakeFailedError",
                              "UnfinishedSocketOperationError",
                              "ProxyConnectionRefusedError", 
                              "ProxyConnectionClosedError", 
                              "ProxyConnectionTimeoutError",
                              "ProxyNotFoundError", "ProxyProtocolError",
                              "OperationError", "SslInternalError",
                              "SslInvalidUserDataError",
                              "TemporaryError"};
};

struct SocketStateName
{
  const char* operator()(int i) const { return states[i]; } 
  const char* states [7] = {"UnconnectedState",
                              "HostLookupState",
                              "ConnectingStat",
                              "ConnectedState",
                              "BoundState",
                              "ClosingState	",
                              "ListeningState"};
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
    inline const char* get_socketstatename() { return socketstatename(udpsocket.state()); }
    inline const qint16& get_port() { return port; }
  private :
    MainWindow mainwindow;
    ConnectionWindow connectionwindow;
    CryptoWindow cryptowindow;
    AlertWindow alertwindow;

    QUdpSocket udpsocket;
    QHostAddress hostaddress;
    qint16 port;

    SocketErrorName socketerrorname;
    SocketStateName socketstatename;

    static const unsigned delayconnection = 3000;
};

#endif
