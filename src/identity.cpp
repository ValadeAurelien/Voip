#include "identity.h"
#include <iostream>

const Identity& Identity::fromDatagramIdentity(const DatagramIdentityHD &dgi)
{
  memcpy(&data, dgi.data, sizeof(IdentityData));
  return *this;
}

void Identity::setName(const QString& _name) 
{ 
  QByteArray nameutf8 = _name.toUtf8();
  data.namesize = ( MAXNAMESIZE < nameutf8.size() ? MAXNAMESIZE : nameutf8.size() );
  memcpy(data.name, nameutf8.data(), data.namesize);
} 

void Identity::setAddress(const QHostAddress& _ha)
{
  data.address = _ha.toIPv4Address();
}

void Identity::setPort(quint16 _p)
{
  data.port = _p;
}

bool Identity::getNextDatagramToSend(DatagramHD& datagram, const DatagramIdentityHeader& idh) const
{
  datagram.dheader.type = IDENTITY;
  if (sizeof(Identity) < IDENTITYDATASIZE)
  {
    datagram.fillWithIdentity((char*) &data, &idh, sizeof(Identity));
    return true;
  }
  else 
    throw "id to big";
  return false;
}

bool Identity::isNull() const
{
  std::cout << QString(data.name).isNull() << " " << QHostAddress(data.address).isNull() << " " << (data.port == 0) << std::endl;
  return ((QString(data.name).isNull() || QHostAddress(data.address).isNull()) || data.port == 0);
}

QString Identity::getName() const { return QString().fromUtf8(data.name, (int) data.namesize); }

QHostAddress Identity::getAddress() const { return QHostAddress(data.address); }

quint16 Identity::getPort() const { return data.port; }
