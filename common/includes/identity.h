#ifndef IDENTITY_H
#define IDENTITY_H
#include <QString>
#include <QByteArray>
#include "datagram.h"

class Identity
{
  public:
    Identity(){};
    Identity(const QString& _name) 
    {
      memcpy(name, _name.toUtf8().constData(), _name.toUtf8().size());
    }
    Identity(const DatagramHD& dg) { fromDatagram(dg); }
    const Identity& fromDatagram(const DatagramHD &dg)
    {
      memcpy(name, dg.ddata.identityhd.data, 64);
      return *this;
    }

    inline QString getName() const { return QString(name); }
  private :
    char name [64];
};

#endif
