#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QString>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QUdpSocket>
#include <QHostAddress>

class AlertWindow : public QWidget
{
  Q_OBJECT
  public : 
    AlertWindow();

    void setMessage(QString mess);
  private :
    QLabel *la_mess;
    QPushButton *button_ok;
    QVBoxLayout *vbox;
};

class ConnectionWindow : public QWidget
{
  Q_OBJECT
  public :
    ConnectionWindow(QHostAddress *_host_address, qint16 *_port, QUdpSocket *_udp_socket);

  private slots: 
    void attemptConnectToHost();
//    void connectionFailed(QAbstractSocket::SocketError SErr);
//    void emitConnectionSucceeded();

  signals:
    bool connectionSucceded();

  private :
    QLabel *ask_for_ha;
    QLineEdit *le_host_adress,
              *le_port;
    QPushButton *button_cancel,
                *button_confirm;
    QGridLayout *wc_grid;
    AlertWindow *alert_window;
    QHostAddress *host_address;
    qint16 *port;
    QUdpSocket *udp_socket;
};
