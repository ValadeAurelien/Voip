#include <QObject>
#include <QWidget>
#include <QString>
#include <QVector>
#include "datagram.h"

class OutMessage
{
  public : 
    OutMessage();
    void setDataIn(const char * _data, quint64 _size);
    bool getNextDatagramToSend(DatagramHD& datagram);
    quint64 getNbDatagramToSend();
    quint64 getNbDatagramSent();
   
  private :
    const char *data;
    quint64 size,
            nbdgsent,
            nbdg2send,
            nbbytessent;
    DatagramMessageHeader header;
};

class InMessage : public QObject
{
  Q_OBJECT
  public :
    InMessage();
    InMessage(const DatagramHD& dg);
    bool completeWithDatagram(const DatagramHD& dg);
    bool isSameMessage(const DatagramHD& dg);
    bool isComplete();
    inline const QString& getString() { return message; }

  signals :
    void completed();

  private :
    void buildMessage();

    QString message;
    QVector<QString> partialmessages;
    DatagramMessageHeader header;
    quint64 checksum,
            actualsum;
};
