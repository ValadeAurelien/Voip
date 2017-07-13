#ifndef DATAGRAM_H
#define DATAGRAM_H
#define DATAGRAMSIZE 512
#include <string.h>

// ----- GENERAL HEADER -----
enum Type {IDENTITY=0, MESSAGE=1};

struct DatagramHeader
{
  Type type;
};


// ----- SPECIFIC HEADERS  -----
enum About {SERVER=0, SELF=1, PEER=2};
struct DatagramIdentityHeader
{
  About about;
};

struct DatagramMessageHeader
{
  unsigned id, nbtot,
           nb,
           size;
};

// ----- SPECIFIC STRUCTS -----
#define IDENTITYDATASIZE (DATAGRAMSIZE-sizeof(DatagramIdentityHeader)-sizeof(DatagramHeader))
struct DatagramIdentityHD
{
  DatagramIdentityHeader header;
  char data [IDENTITYDATASIZE];
};

#define MESSAGEDATASIZE (DATAGRAMSIZE-sizeof(DatagramMessageHeader)-sizeof(DatagramHeader))
struct DatagramMessageHD
{
  DatagramMessageHeader header;
  char data [MESSAGEDATASIZE];
};


// ---- DATAGRAM STRUCT -----

union DatagramData
{
  DatagramIdentityHD identityhd; 
  DatagramMessageHD messagehd; 
};

struct DatagramHD
{
  void fillWithIdentity(const char *data, const DatagramIdentityHeader *ih=NULL, size_t size=IDENTITYDATASIZE)
  {
    dheader.type = IDENTITY;
    if (ih) memcpy(&(ddata.identityhd.header), ih, sizeof(DatagramIdentityHeader));
    if (data) memcpy(&(ddata.identityhd.data), data, size);
  }
  void fillWithMessage(const char *data, const DatagramMessageHeader *mh=NULL, size_t size=MESSAGEDATASIZE)
  {
    dheader.type = MESSAGE;
    if (mh) memcpy(&(ddata.messagehd.header), mh, sizeof(DatagramMessageHeader));
    if (data) memcpy(&(ddata.messagehd.data), data, size);
  }

  const DatagramMessageHD& getMessageHD() const { return ddata.messagehd; }
  const DatagramIdentityHD& getIdentityHD() const { return ddata.identityhd; }
//  DatagramMessageHD getMessageHD() const { return ddata.messagehd; }
//  DatagramIdentityHD getIdentityHD() const { return ddata.identityhd; }

  DatagramHeader dheader;
  DatagramData ddata;
};

#endif  
