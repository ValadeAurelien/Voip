#include "auxwindows.h"

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

ConnectionWindow::ConnectionWindow(QHostAddress *_host_address, qint16 *_port, QUdpSocket *_udp_socket) : 
  QWidget(), host_address(_host_address), port(_port), udp_socket(_udp_socket)
{
  setWindowTitle("Manage Connection");
  ask_for_ha = new QLabel("Enter host address (IPv4, IPv6, URL) and port : ");
  le_host_adress = new QLineEdit;
  le_port = new QLineEdit;
  button_cancel = new QPushButton("Cancel"),
  button_confirm = new QPushButton("Confirm");
  wc_grid = new QGridLayout(this);
  wc_grid->addWidget(ask_for_ha, 0, 0, 1, 2);
  wc_grid->addWidget(le_host_adress, 1, 0, 1, 1); wc_grid->addWidget(le_port, 1, 0, 1, 1);
  wc_grid->addWidget(button_cancel, 2, 0, 1, 1); wc_grid->addWidget(button_confirm, 2, 1, 1, 1);

  alert_window = new AlertWindow;

  connect(button_cancel, SIGNAL(clicked()), this, SLOT(close()));
//  connect(le_host_adress, SIGNAL(returnPressed()), this, SLOT(emitConfirmed()));
//  connect(button_confirm, SIGNAL(clicked()), this, SLOT(attemptConnectToHost()));
}

void ConnectionWindow::attemptConnectToHost()
{ 
  if (!host_address->setAddress(le_host_adress->text())) 
  {
    alert_window->setMessage("Host Address not understood...");
    alert_window->show();
    return;
  }
  bool ok;
  *port = le_port->text().toInt(&ok);
  if (!ok) 
  {
    alert_window->setMessage("Port not understood...");
    alert_window->show();
    return;
  }
  connect(udp_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionFailed(QAbstractSocket::SocketError)));
  connect(udp_socket, SIGNAL(connected()), this, SLOT(emitconnectionSucceeded));   
  udp_socket->connectToHost(*host_address, *port);
}


