#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QString>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QUdpSocket>
#include <QHostAddress>
#include "auxwindows.h"

class MainWindow : public QWidget
{
  Q_OBJECT
  public : 
    MainWindow();
  
  private slots:
    bool flushAndSendUdp();
    void updateLaHostAddress();
    
  private : 

    QPushButton *button_quit,
                *button_send,
                *button_connect;
    QLabel * la_host_address,
           * la_enter_message;
    QLineEdit *le_message_in;
    QGridLayout *grid;
    QHostAddress host_address;
    qint16 port;
    QUdpSocket udp_socket;

    ConnectionWindow *connection_window;
};



