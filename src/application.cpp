#include "application.h"
#include "windows.h"
#include "datagram.h"
#include <cassert>
#include <iostream>

Application::Application() : QObject(), mainwindow(this), connectionwindow(this), cryptowindow(this)
{
  selfidentity.setName("Aurélien");

  connect(&hostsocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionFailed(QAbstractSocket::SocketError)));

  connect(&hostsocket, SIGNAL(connected()), &mainwindow, SLOT(updateLaHostInformation()));
  connect(&hostsocket, SIGNAL(connected()), &connectionwindow, SLOT(close()));
  connect(&hostsocket, SIGNAL(connected()), this, SLOT(sendIdentityToHost()));
  connect(&hostsocket, SIGNAL(readyRead()), this, SLOT(treatInDatagram()));

  connect(&peersocket, SIGNAL(connected()), &mainwindow, SLOT(updateLaPeerInformation()));
  connect(&peersocket, SIGNAL(readyRead()), this, SLOT(treatInDatagram()));

  connect(this, SIGNAL(messageSent()), &mainwindow, SLOT(flush()));
  connect(&inmessage, SIGNAL(completed()), &mainwindow, SLOT(showNewMessage()));
}

void Application::attemptConnectToHost()
{ 
  if (!hostaddress.setAddress(connectionwindow.le_hostaddress->text())) 
  {
    reportError("Host Address not understood...");
    return;
  }
  bool ok;
  hostport = connectionwindow.le_port->text().toUInt(&ok);
  if (!ok) 
  {
    reportError("Port not understood...");
    return;
  }
  if (hostsocket.state())
    hostsocket.disconnectFromHost();
  hostsocket.connectToHost(hostaddress, hostport);
  if (!hostsocket.waitForConnected(delayconnection)) connectionFailed(hostsocket.error());
}

void Application::attemptConnectToPeer()
{

}

void Application::connectionFailed(QAbstractSocket::SocketError SErr)
{
  reportError(QString("Connection Failed : ") + socketerrorname(SErr));
}

void Application::sendMessageToPeer()
{
  if (!peersocket.state())
  {
    reportError("Peer Address not or incorrectly configured...\nThe message will no be sent.");
    return;
  }

  outmessage.setDataIn(mainwindow.te_editmessage->toPlainText().toUtf8().constData(), mainwindow.te_editmessage->toPlainText().toUtf8().size());
  while (outmessage.getNextDatagramToSend(outdatagram))
    if (!sendOutDatagramToHost())
      return;
  emit messageSent();
}

void Application::sendIdentityToHost()
{
  DatagramIdentityHeader ih; ih.about = PEER;
  outdatagram.fillWithIdentity((char*) &selfidentity, &ih);
  sendOutDatagramToHost();
}

void Application::treatInDatagram()
{
  hostsocket.read((char*) &indatagram, DATAGRAMSIZE);
  switch ( indatagram.dheader.type )
  {
    case IDENTITY:
      switch ( indatagram.getIdentityHD().header.about )
      {
        case SERVER: 
          setHostIdentity();
          break;
        case SELF:
          updateIdentity();
          break;
        case PEER:
          setPeerIdentity();
          break;
        default:
          reportError("Incoming datagram not understood...");
          break;
      }
      break;
    case MESSAGE:
      if (!inmessage.completeWithDatagramMessage(indatagram.getMessageHD()))
        reportError("Missed a message...");
      break;
    default:
      reportError("Incoming datagram not understood...");
      break;
  }
}
void Application::reportError(QString err)
{
  alertwindow.setMessageAndShow(err);
}

bool Application::sendOutDatagram(QUdpSocket& socket)
{
  assert(sizeof(DatagramHD)==DATAGRAMSIZE);
  int nbbytes = socket.write((char*) &outdatagram, DATAGRAMSIZE);
  if (nbbytes < DATAGRAMSIZE || nbbytes == -1)
  {
    reportError(QString("Error while sending a datagram : ") + socketerrorname(socket.error()));
    return false;
  }
  return true;
}

bool Application::sendOutDatagramToHost() { return sendOutDatagram(hostsocket); }
bool Application::sendOutDatagramToPeer() { return sendOutDatagram(peersocket); }

void Application::setHostIdentity() { memcpy((char*) &hostidentity, indatagram.getIdentityHD().data, sizeof(Identity)); }
void Application::updateIdentity() { memcpy((char*) &selfidentity, indatagram.getIdentityHD().data, sizeof(Identity)); }
void Application::setPeerIdentity() { memcpy((char*) &peeridentity, indatagram.getIdentityHD().data, sizeof(Identity)); }

