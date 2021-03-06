#include <QTime>
#include <iostream>
#include "application.h"
#include "windows.h"
#include "datagram.h"

Application::Application(const Identity& _id) : QObject(), mainwindow(this), connectionwindow(this), cryptowindow(this), selfidentity(_id)
{
  socket.bind(selfidentity.getAddress(), selfidentity.getPort());
  mainwindow.updateLaSelfInformation();

  connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionFailed(QAbstractSocket::SocketError)));
  connect(&socket, SIGNAL(readyRead()), this, SLOT(treatInDatagram()));
  connect(this, SIGNAL(waitingForConnectionToHost()), this, SLOT(timerConnectionToHost()));
  connect(this, SIGNAL(waitingForConnectionToPeer()), this, SLOT(timerConnectionToPeer()));

  connect(this, SIGNAL(hostAnsweredHostIdentity()), this, SLOT(confirmAndUpdateHostIdentity()));
  connect(this, SIGNAL(hostAnsweredSelfIdentity()), this, SLOT(confirmAndUpdateSelfIdentity()));
  connect(this, SIGNAL(hostAnsweredPeerIdentity()), this, SLOT(confirmAndUpdatePeerIdentity()));
  connect(this, SIGNAL(peerAnsweredPeerIdentity()), this, SLOT(confirmAndUpdatePeerIdentity()));

  connect(this, SIGNAL(hostIdentityConfirmed()), &mainwindow, SLOT(updateLaHostInformation()));
  connect(this, SIGNAL(hostIdentityConfirmed()), &connectionwindow, SLOT(close()));
  connect(this, SIGNAL(hostIdentityConfirmed()), &waitwindow, SLOT(close()));

  connect(this, SIGNAL(selfIdentityConfirmed()), &mainwindow, SLOT(updateLaSelfInformation()));

  connect(this, SIGNAL(peerIdentityConfirmed()), &mainwindow, SLOT(updateLaPeerInformation()));

  connect(this, SIGNAL(messageSent()), &mainwindow, SLOT(flush()));
  connect(&inmessage, SIGNAL(completed()), &mainwindow, SLOT(showNewMessage()));
}

void Application::attemptConnectToHost()
{ 
  QHostAddress had (connectionwindow.le_hostaddress->text());
  if (had.isNull())
  {
    reportError("Host Address not understood...");
    return;
  }
  else hostidentity.setAddress(had);
  bool ok;
  quint16 hp = connectionwindow.le_port->text().toUInt(&ok);
  if (!ok) 
  {
    reportError("Port not understood...");
    return;
  }
  else hostidentity.setPort(hp);
  hostidentityhasbeenconfirmed = false;
  selfidentityhasbeenconfirmed = false;
  sendSelfIdentityToIdentity(hostidentity);
  waitwindow.setMessageAndShow("Connection to host, please wait..."); 
  emit waitingForConnectionToHost();
}

void Application::attemptConnectToPeer()
{
  peeridentityhasbeenconfirmed = false;
  sendSelfIdentityToIdentity(peeridentity);
}

void Application::connectionFailed(QAbstractSocket::SocketError SErr)
{
  reportError(QString("Connection Failed : ") + socketerrorname(SErr));
}

void Application::sendMessageToPeer()
{
  if (peeridentity.isNull())
  {
    reportError("Peer Address not or incorrectly configured...\nThe message will no be sent.");
    return;
  }

  outmessage.setDataIn(mainwindow.te_editmessage->toPlainText().toUtf8().constData(), mainwindow.te_editmessage->toPlainText().toUtf8().size());
  while (outmessage.getNextDatagramToSend(outdatagram))
    if (!sendOutDatagramToPeer())
      return;
  emit messageSent();
}

void Application::sendSelfIdentityToIdentity(const Identity& id) 
{
  DatagramIdentityHeader idh; idh.about = SELF;
  selfidentity.getNextDatagramToSend(outdatagram, idh);
  if (!sendOutDatagramToIdentity(id))
    return;
}

void Application::treatInDatagram()
{
  socket.readDatagram((char*) &indatagram, DATAGRAMSIZE, &senderaddress, &senderport);
  switch ( indatagram.dheader.who ) 
  {
    case SERVER : 
      switch ( indatagram.dheader.type )
      {
        case IDENTITY:
          switch ( indatagram.getIdentityHD().header.about )
          {
            case HOST : 
              emit hostAnsweredHostIdentity();
              break;
            case SELF : 
              emit hostAnsweredSelfIdentity();
              break;
            case PEER :
              emit hostAnsweredPeerIdentity();
              break;
            default :
              reportError("Incoming datagram not understood... SERVER+IDENTITY");
              break;
          }
          break;
        case MESSAGE :
          reportError("Incoming message from a server...");
          break;
        default:
          reportError("Incoming datagram not understood... SERVER+???");
          break;
      }
      break;
    case CLIENT:
      switch ( indatagram.dheader.type )
      {
        case IDENTITY:
          switch ( indatagram.getIdentityHD().header.about )
          {
            case HOST : 
              reportError("Peer talks about a server...");
              break;
            case SELF : 
              reportError("Peer talks about me...");
              break;
            case PEER :
              emit peerAnsweredPeerIdentity();
              break;
            default :
              reportError("Incoming datagram not understood... CLIENT+IDENTITY+????");
              break;
          }
          break;
        case MESSAGE : 
          if (inmessage.isSameMessage(indatagram.getMessageHD()))
            inmessage.completeWithDatagramMessage(indatagram.getMessageHD());
          else
            inmessage.newMessageFromDatagramMessage(indatagram.getMessageHD());
          break;
        default:
          reportError("Incoming datagram not understood...CLIENT+????");
          break;
      }
      break;
    default :
      reportError("Incoming datagram not understood... ????");
      break;
  }
}


void Application::timerConnectionToHost()
{
  QTime dieTime= QTime::currentTime().addSecs(delayhostanswer);
  while (QTime::currentTime() < dieTime)
  {
    if (hostidentityhasbeenconfirmed && selfidentityhasbeenconfirmed)
      return;
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
  }
  connectionwindow.close();
  waitwindow.close();
  reportError("Host took too long to answer...");
}

void Application::timerConnectionToPeer()
{
  QTime dieTime= QTime::currentTime().addSecs(delayhostanswer);
  while (QTime::currentTime() < dieTime)
  {
    if (peeridentityhasbeenconfirmed)
      return;
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
  }
  reportError("Peer took too long to answer...");
}

void Application::reportError(QString err)
{
  alertwindow.setMessageAndShow(err);
}

void Application::logMessage(QString mess)
{
  std::cout << mess.toStdString() << std::endl;
}

bool Application::sendOutDatagramToIdentity(const Identity& dest) 
{
  outdatagram.dheader.who = CLIENT;
  int nbbytes = socket.writeDatagram((char*) &outdatagram, DATAGRAMSIZE, dest.getAddress(), dest.getPort());
  if (nbbytes < DATAGRAMSIZE || nbbytes == -1)
  {
    reportError(QString("Error while sending a datagram : ") + socketerrorname(socket.error()));
    return false;
  }
  return true;
}

bool Application::sendOutDatagramToHost() { return sendOutDatagramToIdentity(hostidentity); }
bool Application::sendOutDatagramToPeer() { return sendOutDatagramToIdentity(peeridentity); }

void Application::confirmAndUpdateHostIdentity() 
{ 
  hostidentity.fromDatagramIdentity(indatagram.getIdentityHD()); 
  emit hostIdentityConfirmed();
  hostidentityhasbeenconfirmed = true;
}
void Application::confirmAndUpdateSelfIdentity() 
{ 
  selfidentity.fromDatagramIdentity(indatagram.getIdentityHD()); 
  emit selfIdentityConfirmed();
  selfidentityhasbeenconfirmed = true;
}
void Application::confirmAndUpdatePeerIdentity() 
{ 
  peeridentity.fromDatagramIdentity(indatagram.getIdentityHD()); 
  emit peerIdentityConfirmed();
  peeridentityhasbeenconfirmed = true;
}

