#include "application.h"
#include "windows.h"
#include "datagram.h"
#include <cassert>
#include <iostream>

Application::Application() : QObject(), mainwindow(this), connectionwindow(this), cryptowindow(this), identity("Aurélien")
{
  connect(&udpsocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionFailed(QAbstractSocket::SocketError)));

  connect(&udpsocket, SIGNAL(connected()), &mainwindow, SLOT(updateLaHostInformation()));
  connect(&udpsocket, SIGNAL(connected()), this, SLOT(sendIdentity()));

  connect(this, SIGNAL(messageSent()), &mainwindow, SLOT(flush()));
}

void Application::sendMessage()
{
  if (hostaddress.isNull())
  {
    alertwindow.setMessageAndShow("Host Address not or incorrectly configured... The message will no be sent.");
    return;
  }

  outmessage.setDataIn(mainwindow.le_messagein->text().toUtf8().constData(), mainwindow.le_messagein->text().toUtf8().size());
 
  while (outmessage.getNextDatagramToSend(outdatagram))
    if (!sendOutDatagram())
      return;
  emit messageSent();
}

void Application::attemptConnectToHost()
{ 
  if (!hostaddress.setAddress(connectionwindow.le_hostaddress->text())) 
  {
    alertwindow.setMessageAndShow("Host Address not understood...");
    return;
  }
  bool ok;
  port = connectionwindow.le_port->text().toUInt(&ok);
  if (!ok) 
  {
    alertwindow.setMessageAndShow("Port not understood...");
    return;
  }
  udpsocket.connectToHost(hostaddress, port);
  if (!udpsocket.waitForConnected(delayconnection)) connectionFailed(udpsocket.error());
  else connectionwindow.close();
}

void Application::connectionFailed(QAbstractSocket::SocketError SErr)
{
  alertwindow.setMessageAndShow(QString("Connection Failed : ") + socketerrorname(SErr));
}

void Application::sendIdentity()
{
  outdatagram.fillWithIdentity((char*) &identity);
  sendOutDatagram();
}

bool Application::sendOutDatagram()
{
//  std::cout << sizeof(DatagramHD) 
//            << " " << DATAGRAMSIZE
//            << " " << sizeof(DatagramHeader) 
//            << " " << sizeof(DatagramData) 
//            << " " << sizeof(DatagramMessageHD)
//            << " " << sizeof(DatagramMessageHeader)
//            << " " << MESSAGEDATASIZE
//            << std::endl;
  assert(sizeof(DatagramHD)==DATAGRAMSIZE);
  int nbbytes = udpsocket.write((char*) &outdatagram, DATAGRAMSIZE);
  if (nbbytes < DATAGRAMSIZE || nbbytes == -1)
  {
    alertwindow.setMessageAndShow("Error while sending a datagram... \n ");
    return false;
  }
  return true;
}
