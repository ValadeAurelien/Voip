#ifndef IDENTITY_H
#define IDENTITY_H
#include <QString>
#include <QByteArray>
#include <QHostAddress>
#include <QWidget>
#include "datagram.h"

struct Identity
{
  const Identity& fromDatagram(const DatagramIdentityHD &dgi)
  {
    memcpy(name, dgi.data, 64);
    memcpy((char*) &address, dgi.data+64, 4);
    memcpy((char*) &port, dgi.data+68, 2);
    return *this;
  }
  inline void setName(const QString _name) 
  { 
    memcpy(name, _name.constData(), ( 64 < _name.size() ? 64 : _name.size() ) ); 
  } 
  inline QString getName() const { return QString(name); }
  char name [64];
  quint32 address;
  quint16 port;
};

#endif
