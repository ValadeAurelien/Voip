#include <iostream>
#include <stdlib.h>
#include "message.h"
#include "datagram.h"


// ----- OUT MESSAGE -----

OutMessage::OutMessage(){}

void OutMessage::setDataIn(const char *_data, quint64 _size) 
{ 
  data=_data;
  size=_size;
  header.id = rand();
  nbdg2send = size/MESSAGEDATASIZE + 1;
  nbdgsent = 0;
  nbbytessent = 0;
}

quint64 OutMessage::getNbDatagramToSend() { return nbdg2send; }
quint64 OutMessage::getNbDatagramSent() { return nbdgsent; }

bool OutMessage::getNextDatagramToSend(DatagramHD &datagram) 
{
  datagram.dheader.type = MESSAGE;
  if (nbbytessent>=size) return false;
  if (nbdgsent +1 < nbdg2send ) 
  {
    header.nb = nbdgsent;
    header.nbtot = nbdg2send;
    header.size = MESSAGEDATASIZE;
    datagram.fillWithMessage(data+nbbytessent, &header, MESSAGEDATASIZE);
    nbdgsent++;
    nbbytessent+=MESSAGEDATASIZE;
    return true;
  }
  else 
  {
    header.nb = nbdgsent;
    header.nbtot = nbdg2send;
    header.size = size-nbbytessent;
    datagram.fillWithMessage(data+nbbytessent, &header, size-nbbytessent);
    nbdgsent++;
    nbbytessent+=size-nbbytessent;
    return true;
  }
}

  
// ----- IN MESSAGE -----


InMessage::InMessage() : QObject() {}

bool InMessage::newMessageFromDatagramMessage(const DatagramMessageHD& dgm)
{
  header = dgm.header;
  actualsum = 0;
  partialmessages.clear();
  partialmessages.resize(header.nbtot);
  checksum=((header.nbtot+1)*header.nbtot)/2;
  return completeWithDatagramMessage(dgm);
}

bool InMessage::completeWithDatagramMessage(const DatagramMessageHD& dgm)
{
  DatagramMessageHeader _header = dgm.header;
  if (_header.id != header.id) return false;
  if (!partialmessages.at(_header.nb).isNull()) return true;
  partialmessages[_header.nb] = QString().fromUtf8(dgm.data, _header.size);
  actualsum+=_header.nb+1;
  isComplete();
  return true;
}

bool InMessage::isSameMessage(const DatagramMessageHD& dgm) const
{
  if (dgm.header.id == header.id)
    return true;
  return false;
}

bool InMessage::isComplete()
{
  std::cout << "in : " << actualsum << " " << checksum << std::endl;
  if (actualsum == checksum)
  {
    buildMessage();
    emit completed();
    return true;
  }
  return false;
}

void InMessage::buildMessage()
{
  message="";
  for (auto const& pm : partialmessages)
  {
    std::cout << pm.toStdString() << std::endl;
    message+=pm;
  }
}
    


 
