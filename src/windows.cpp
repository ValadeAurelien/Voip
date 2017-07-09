#include <QApplication>
#include "windows.h"
#include "application.h"

// ----- MAIN WINDOW -----

MainWindow::MainWindow(Application *_application) : QWidget(), application(_application)
{
  setWindowTitle("New Message");
  la_hostaddress = new QLabel("Host Address : None");
  la_entermessage = new QLabel("Enter message : ");
  le_messagein = new QLineEdit;
  button_quit = new QPushButton("Quit");
  button_connect = new QPushButton("Connect");
  button_send = new QPushButton("Send");
  grid = new QGridLayout(this);
  grid->addWidget(la_hostaddress, 0, 0, 1, 3);
  grid->addWidget(la_entermessage, 1, 0, 1, 1); grid->addWidget(le_messagein, 1, 1, 1, 2);
  grid->addWidget(button_quit, 2, 0, 1, 1); grid->addWidget(button_connect, 2, 1, 1, 1); grid->addWidget(button_send, 2, 2, 1, 1);

  connect(button_quit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
  connect(button_send, SIGNAL(clicked()), this, SLOT(flush()));
  connect(le_messagein, SIGNAL(returnPressed()), this, SLOT(flush()));
  connect(button_send, SIGNAL(clicked()), application, SLOT(sendMessage()));
  connect(le_messagein, SIGNAL(returnPressed()), application, SLOT(sendMessage()));
  connect(button_connect, SIGNAL(clicked()), application, SLOT(showConnectionWindow()));
}

void MainWindow::updateLaHostAddress() 
{ 
  la_hostaddress->setText("Host Address : " + application->get_hostaddress().toString()); 
}

bool MainWindow::flush()
{
  le_messagein->clear();
}


// ----- ALERT WINDOW -----

AlertWindow::AlertWindow() : QWidget()
{
  setWindowTitle("Ouch !");
  la_mess = new QLabel;
  button_ok = new QPushButton("Ok");
  vbox = new QVBoxLayout(this);
  vbox->addWidget(la_mess);
  vbox->addWidget(button_ok);
  connect(button_ok, SIGNAL(clicked()), this, SLOT(close()));
}

void AlertWindow::setMessage(QString mess) { la_mess->setText(mess); }

// ----- CONNECTION WINDOW -----

ConnectionWindow::ConnectionWindow(Application *_application) : QWidget(), application (_application) 
{
  setWindowTitle("Manage Connection");
  la_askforhostaddress = new QLabel("Enter host address (IPv4, IPv6, URL) and port : ");
  le_hostaddress = new QLineEdit;
  le_port = new QLineEdit;
  button_cancel = new QPushButton("Cancel"),
  button_confirm = new QPushButton("Confirm");
  grid = new QGridLayout(this);
  grid->addWidget(la_askforhostaddress, 0, 0, 1, 2);
  grid->addWidget(le_hostaddress, 1, 0, 1, 1); grid->addWidget(le_port, 1, 1, 1, 1);
  grid->addWidget(button_cancel, 2, 0, 1, 1);  grid->addWidget(button_confirm, 2, 1, 1, 1);

  connect(button_cancel, SIGNAL(clicked()), this, SLOT(close()));
  connect(le_hostaddress, SIGNAL(returnPressed()), application, SLOT(attemptConnectToHost()));
  connect(button_confirm, SIGNAL(clicked()), application, SLOT(attemptConnectToHost()));
}


// ----- CRYPTO WINDOW -----

CryptoWindow::CryptoWindow(Application *_application) : QWidget(), application (_application)
{

}
