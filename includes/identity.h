#ifndef IDENTITY_H
#define IDENTITY_H
#include <QString>
#include <QByteArray>
#include <QHostAddress>
#include <QWidget>
#include "datagram.h"
#include <QString>
#include <QHostAddress>

#define MAXNAMESIZE 64 

struct IdentityData
{
  char namesize;
  char name [MAXNAMESIZE];
  quint32 address;
  quint16 port;
};

class Identity
{
  public : 
    const Identity& fromDatagramIdentity(const DatagramIdentityHD &dgi);
    void setName(const QString& _name);
    void setAddress(const QHostAddress& _ha);
    void setPort(quint16 _p);
    bool getNextDatagramToSend(DatagramHD& datagram, const DatagramIdentityHeader& idh) const;
    
    bool isNull() const;
    QString getName() const;
    QHostAddress getAddress() const;
    quint16 getPort() const;

  private : 
    IdentityData data;
};

#endif
