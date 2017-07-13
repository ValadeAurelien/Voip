#ifndef WINDOWS_H
#define WINDOWS_H
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QString>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QUdpSocket>
#include <QHostAddress>

class Application;

// ----- MAIN WINDOW -----
class MainWindow : public QWidget
{
  Q_OBJECT
  friend class Application;
  public : 
    MainWindow(Application *_application);
  
  private slots:
    bool flush();
    void updateLaHostInformation();
    void updateLaPeerInformation();
    void showNewMessage();
    void isReturnPressed();
    
  signals:
    void returnPressed();
  private : 
    Application *application;

    QPushButton *button_quit,
                *button_send,
                *button_connect;
    QLabel * la_hostinformation,
           * la_peerinformation,
           * la_entermessage;
    QTextEdit *te_editmessage,
              *te_messages;
    QGridLayout *grid;
};


// ----- ALERT WINDOW -----

class AlertWindow : public QWidget
{
  Q_OBJECT
  friend class Application;
  public : 
    AlertWindow();

    void setMessage(QString mess);
    void setMessageAndShow(QString mess);
  private :
    QLabel *la_mess;
    QPushButton *button_ok;
    QVBoxLayout *vbox;
};


// ----- CONNECTION WINDOW -----

class ConnectionWindow : public QWidget
{
  Q_OBJECT
  friend class Application;
  public :
    ConnectionWindow(Application *application);
  signals:
    bool connectionSucceded();
  private :
    Application *application;
    QLabel *la_askforhostaddress;
    QLineEdit *le_hostaddress,
              *le_port;
    QPushButton *button_cancel,
                *button_confirm;
    QGridLayout *grid;
};



// ----- CRYPTO WINDOW -----

class CryptoWindow : public QWidget
{
  Q_OBJECT
  friend class Application;
  public :
    CryptoWindow(Application *_application);
  private :
    Application *application;
};
#endif
