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
  udpsocket.readDatagram((char*) &indatagram, DATAGRAMSIZE, &host1address, &host1port);
  switch( indatagram.dheader.type )
  {
    case IDENTITY:
      coutIdentity();
      break;
    case MESSAGE:
      coutMessage();
    default:
      reportError("Package corrupted");
      break;
  }
}

void ServerApp::bindingFailed(QAbstractSocket::SocketError serr)
{
  reportError(QString("Socket error : ") + socketerrorname(serr));
}

void ServerApp::coutMessage()
{
  std::cout << "Message from : " << host1address.toString().toStdString() << ":" << host1port 
            << " (" << indatagram.ddata.messagehd.header.size  << " bytes) > " 
            << QString().fromUtf8(indatagram.ddata.messagehd.data, indatagram.ddata.messagehd.header.size).toStdString() << std::endl;
}

void ServerApp::coutIdentity()
{
  std::cout << "Message from : " << host1address.toString().toStdString() << ":" << host1port 
            << " (" << 64 << " bytes) IDENTITYi, ID > " 
            << host1identity.fromDatagram(indatagram).getName().toStdString()
            << std::endl;
}

void ServerApp::reportError(QString err)
{
  std::cout << err.data() << std::endl;
}


