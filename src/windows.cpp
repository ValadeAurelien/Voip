#include <QApplication>
#include "windows.h"
#include "application.h"

// ----- MAIN WINDOW -----

MainWindow::MainWindow(Application *_application) : QWidget(), application(_application)
{
  setWindowTitle("New Message");
  la_hostinformation = new QLabel("Host Address : None\nState : Unknown");
  la_peerinformation = new QLabel("Peer Address : None\nState : Unknown");
  la_entermessage = new QLabel("Enter message : ");
  te_editmessage = new QTextEdit;
  te_messages = new QTextEdit;
  te_messages->setReadOnly(true);
  te_messages->setMinimumSize(0, 50);

  button_quit = new QPushButton("Quit");
  button_connect = new QPushButton("Connect");
  button_send = new QPushButton("Send");
  grid = new QGridLayout(this);
  grid->addWidget(la_hostinformation, 0, 0, 1, 1); grid->addWidget(la_peerinformation, 0, 1, 1, 2);
  grid->addWidget(te_messages, 1, 0, 1, 3);
  grid->addWidget(la_entermessage, 2, 0, 1, 1); grid->addWidget(te_editmessage, 2, 1, 1, 2);
  grid->addWidget(button_quit, 3, 0, 1, 1); grid->addWidget(button_connect, 3, 1, 1, 1); grid->addWidget(button_send, 3, 2, 1, 1);

  connect(button_quit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
  connect(button_send, SIGNAL(clicked()), application, SLOT(sendMessageToPeer()));
  connect(te_editmessage, SIGNAL(textChanged()), this, SLOT(isReturnPressed()));
  connect(this, SIGNAL(returnPressed()), application, SLOT(sendMessageToPeer()));
  connect(button_connect, SIGNAL(clicked()), application, SLOT(showConnectionWindow()));
}

void MainWindow::updateLaHostInformation() 
{ 
  la_hostinformation->setText("Host Address : " + application->getHostAddress().toString() + " (port : " + QString("%1").arg(application->getHostPort()) + ") \n"
                           + "State : " + application->getHostSocketStateName()); 
}

void MainWindow::updateLaPeerInformation() 
{ 
  la_peerinformation->setText("Peer Address : " + application->getPeerAddress().toString() + " (port : " + QString("%1").arg(application->getPeerPort()) + ") \n"
                           + "State : " + application->getPeerSocketStateName()); 
}

bool MainWindow::flush()
{
  te_messages->append("\nsent: " + te_editmessage->toPlainText());
  te_editmessage->clear();
}
//
void MainWindow::showNewMessage()
{
  te_messages->append("\nreceived : " + application->getNewMessage());
}

void MainWindow::isReturnPressed()
{
  if (te_editmessage->toPlainText().isEmpty()) return;
  if (te_editmessage->toPlainText().endsWith(QString("\n")))
  {
    const QString& tmp = te_editmessage->toPlainText();
    te_editmessage->setPlainText(tmp.left(tmp.size()-1));
    emit returnPressed();
  }
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
void AlertWindow::setMessageAndShow(QString mess) { la_mess->setText(mess); show(); }

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
  connect(le_port, SIGNAL(returnPressed()), application, SLOT(attemptConnectToHost()));
  connect(button_confirm, SIGNAL(clicked()), application, SLOT(attemptConnectToHost()));
}


// ----- CRYPTO WINDOW -----

CryptoWindow::CryptoWindow(Application *_application) : QWidget(), application (_application)
{

}
