#include "mainwindow.h"
#include <QApplication>

MainWindow::MainWindow() : QWidget()
{
  setWindowTitle("New Message");
  la_host_address = new QLabel("Host Address : None");
  la_enter_message = new QLabel("Enter message : ");
  le_message_in = new QLineEdit;
  button_quit = new QPushButton("Quit");
  button_connect = new QPushButton("Connect");
  button_send = new QPushButton("Send");
  grid = new QGridLayout(this);
  grid->addWidget(la_host_address, 0, 0, 1, 3);
  grid->addWidget(la_enter_message, 1, 0, 1, 1); grid->addWidget(le_message_in, 1, 1, 1, 2);
  grid->addWidget(button_quit, 2, 0, 1, 1); grid->addWidget(button_connect, 2, 1, 1, 1); grid->addWidget(button_send, 2, 2, 1, 1);

  connection_window = new ConnectionWindow(&host_address, &port, &udp_socket);

  connect(le_message_in, SIGNAL(returnPressed()), this, SLOT(flushAndSendUdp()));
  connect(button_quit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
  connect(button_send, SIGNAL(clicked()), this, SLOT(flushAndSendUdp()));
  connect(button_connect, SIGNAL(clicked()), connection_window, SLOT(show()));
  connect(connection_window, SIGNAL(confirmed()), this, SLOT(updateLaHostAddress)); 
}

void MainWindow::updateLaHostAddress() 
{ 
  la_host_address->setText("Host Address : " + host_address.toString()); 
}

bool MainWindow::flushAndSendUdp()
{
  le_message_in->clear();
}

