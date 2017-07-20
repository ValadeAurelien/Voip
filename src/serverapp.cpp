#include "serverapp.h"
#include "datagram.h"
#include "identity.h"
#include <QString>
#include <iostream>
#include <string>


ServerApp::ServerApp() : QObject()
{
  selfidentity.setName("Serveur");
  selfidentity.setAddress(QHostAddress::LocalHost);
  selfidentity.setPort(port);

  socket.bind(QHostAddress::LocalHost, port);
  connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(bindingFailed(QAbstractSocket::SocketError)));
  connect(&socket, SIGNAL(readyRead()), this, SLOT(treatInDatagram()));

  connect(this, SIGNAL(receivedClientIdentity()), this, SLOT(treatIncomingIdentity()));
}

void ServerApp::treatInDatagram()
{
  socket.readDatagram((char*) &indatagram, DATAGRAMSIZE, &senderaddress, &senderport);
  switch ( indatagram.dheader.who ) 
  {
    case SERVER : 
      reportError("Another server is talking to me, wtf?");
      break;
    case CLIENT:
      switch( indatagram.dheader.type )
      {
        case IDENTITY:
          switch ( indatagram.getIdentityHD().header.about )
          {
            case HOST : 
            case PEER : 
              reportError("Peer taling shit...");
              break;
            case SELF:
              emit receivedClientIdentity();
              break;
            default:
              reportError("What is it about ?");
              break;
          }
          break;
        case MESSAGE:
          reportError("Received message from client...");
          break;
        default:
          reportError("What type ?");
          break;
      }
      break;
    default:
      reportError("Who's talking to me?");
      break;
  }
}

void ServerApp::bindingFailed(QAbstractSocket::SocketError serr)
{
  reportError(QString("Socket error : ") + socketerrorname(serr));
}

void ServerApp::treatIncomingIdentity()
{
  if (client1identity.isNull())
  {
    client1identity.fromDatagramIdentity(indatagram.getIdentityHD());
    completeAndAnswerIdentity(client1identity);
    return;
  }
  else if (client2identity.isNull())
  {
    client2identity.fromDatagramIdentity(indatagram.getIdentityHD());
    completeAndAnswerIdentity(client2identity);
    return;
  }
  else 
    reportError("More than two clients connected ! ");
}

void ServerApp::completeAndAnswerIdentity(Identity& id)
{
  id.setAddress(senderaddress);
  id.setPort(senderport);
  DatagramIdentityHeader idh; 
  idh.about = SELF;
  sendIdentityToIdentity(id, id, idh);
  idh.about = HOST;
  sendIdentityToIdentity(selfidentity, id, idh);
}

void ServerApp::sendIdentityToIdentity(const Identity& id, const Identity& dest, const DatagramIdentityHeader& idh)
{
  outdatagram.dheader.type = IDENTITY;
  id.getNextDatagramToSend(outdatagram, idh);
  if (!sendOutDatagramToIdentity(dest))
    return;
}

bool ServerApp::sendOutDatagramToIdentity(const Identity& dest) 
{
  outdatagram.dheader.who = SERVER;
  std::cout << "sending message to " << dest.getAddress().toString().toStdString() << " " << dest.getPort() << std::endl;
  int nbbytes = socket.writeDatagram((char*) &outdatagram, DATAGRAMSIZE, dest.getAddress(), dest.getPort());
  if (nbbytes < DATAGRAMSIZE || nbbytes == -1)
  {
    reportError(QString("Error while sending a datagram : ") + socketerrorname(socket.error()));
    return false;
  }
  return true;
}

void ServerApp::reportError(QString err)
{
  std::cout << err.toStdString() << std::endl;
}


