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
    bool getNextDatagramToSend(DatagramHD& dgm);
    quint64 getNbDatagramToSend();
    quint64 getNbDatagramSent();
   
  private :
    quint64 size,
            nbdgsent,
            nbdg2send,
            nbbytessent;
    DatagramMessageHeader header;
    const char *data;
};

class InMessage : public QObject
{
  Q_OBJECT
  public :
    InMessage();
    InMessage(const DatagramMessageHD& dgm);
    bool completeWithDatagramMessage(const DatagramMessageHD& dgm);
    bool isSameMessage(const DatagramMessageHD& dgm);
    bool isComplete();
    inline const QString& getMessage() { return message; }

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
