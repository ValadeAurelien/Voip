#include "serverapp.h"
#include "datagram.h"
#include "identity.h"
#include <QString>
#include <iostream>
#include <string>


ServerApp::ServerApp() : QObject()
{
  udpsocket.bind(QHostAddress::LocalHost, port);
  connect(&udpsocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(bindingFailed(QAbstractSocket::SocketError)));
  connect(&udpsocket, SIGNAL(readyRead()), this, SLOT(treatInDatagram()));
}

void ServerApp::treatInDatagram()
{
  udpsocket.readDatagram((char*) &indatagram, DATAGRAMSIZE, &senderaddress, &senderport);
  switch( indatagram.dheader.type )
  {
    case IDENTITY:
      switch ( indatagram.getIdentityHD().header.about )
      {
        case SERVER:
          reportError("Received identity from another server... WTF????");
          break;
        case SELF:
          reportError("Received identity about myself... WTF????");
        case PEER:
          newIncomingIdentity();
          break;
        default:
          reportError("Package corrupted...");
          break;
      }
    case MESSAGE:
      reportError("Received message...");
    default:
      reportError("Package corrupted");
      break;
  }
}

void ServerApp::bindingFailed(QAbstractSocket::SocketError serr)
{
  reportError(QString("Socket error : ") + socketerrorname(serr));
}

void ServerApp::newIncomingIdentity()
{
  if (!hashost1)
  {
    hashost1 = true;
    memcpy(&host1identity, indatagram.getIdentityHD().data, sizeof(Identity));
    completeAndAnswerIdentity(host1identity);
    return;
  }
  if (!hashost2)
  {
    hashost2 = true;
    memcpy(&host2identity, indatagram.getIdentityHD().data, sizeof(Identity));
    completeAndAnswerIdentity(host2identity);
    return;
  }
}

void ServerApp::completeAndAnswerIdentity(Identity& id)
{
  id.address = senderaddress.toIPv4Address();
  id.port = senderport;
  outdatagram.dheader.type = IDENTITY;
  DatagramIdentityHeader idh; idh.about = SELF;
  outdatagram.fillWithIdentity((char*) &host1identity, &idh, sizeof(Identity));
  udpsocket.writeDatagram((char*) &outdatagram, DATAGRAMSIZE, senderaddress, senderport);
}

void ServerApp::reportError(QString err)
{
  std::cout << err.data() << std::endl;
}


