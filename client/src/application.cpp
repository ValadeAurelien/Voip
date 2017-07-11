#include "application.h"
#include "windows.h"
#include <iostream>

Application::Application() : QObject(), mainwindow(this), connectionwindow(this), cryptowindow(this)
{
  connect(this, SIGNAL(messageSent()), &mainwindow, SLOT(flush()));
  connect(&udpsocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionFailed(QAbstractSocket::SocketError)));
  connect(&udpsocket, SIGNAL(connected()), &mainwindow, SLOT(updateLaHostInformation()));
//  connect(udp_socket, SIGNAL(connected()), this, SLOT(emitconnectionSucceeded));   
}

void Application::sendMessage()
{
  if (hostaddress.isNull())
  {
    alertwindow.setMessageAndShow("Host Address not or incorrectly configured... The message will no be sent.");
    return;
  }
  ba_messagein = mainwindow.le_messagein->text().toUtf8();
  if (ba_messagein.length() == 0) return;
  if (ba_messagein.length() > maxsize_messagein)
  {
    alertwindow.setMessageAndShow("Message too big...");
    return ;
  }
  nbdg2send_messagein = ba_messagein.length()/datagramsize;
  int nbbyteswritten;
  for (nbdgsent_messagein = 0; nbdgsent_messagein < nbdg2send_messagein; nbdgsent_messagein++)
  {
    nbbyteswritten = udpsocket.write(ba_messagein.constData()+nbdgsent_messagein*datagramsize, datagramsize);
    if (nbbyteswritten < datagramsize || nbbyteswritten == -1)
    {
      alertwindow.setMessageAndShow("Error while sending the message... \n " 
                             + QString("%1").arg(nbdgsent_messagein) + " datagrams sent on " + QString("%1").arg(nbdg2send_messagein+1)) ;
      return ;
    }
  }
  nbbyteswritten = udpsocket.write(ba_messagein.constData()+nbdg2send_messagein*datagramsize, ba_messagein.length()-nbdg2send_messagein*datagramsize);
  if (nbbyteswritten < ba_messagein.length()-nbdg2send_messagein*datagramsize || nbbyteswritten == -1)
  {
    alertwindow.setMessageAndShow("Error while sending the message... \n " 
                           + QString("%1").arg(nbdgsent_messagein) + " datagrams sent on " + QString("%1").arg(nbdg2send_messagein+1)) ;
    return ;
  }
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
  port = connectionwindow.le_port->text().toInt(&ok);
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
