#ifndef COM_PORT_H
#define COM_PORT_H

#include <QObject>
#include <QSerialPort>
#include <QString>
#include <QDebug>
#include <assert.h>

#define START_BYTE 0x01
#define END_BYTE 0x09
#define DS18B20_ADDRESS 0x43
#define NTC_ADDRESS 0x44

class com_port : public QObject
{
    Q_OBJECT
private :
    QSerialPort *this_port;

public:
    com_port();
    ~com_port();

    bool Open(const QString& name_port);
    void Close();
    int SetUp();

    QByteArray buffer;

signals :
    void sig_Opened(const QString& port_name, int status_error);
    void sig_TempertureInBuffer(const QByteArray& data, uint8_t sensor_number);
public slots:
    void slot_GetData();
    void slot_SendData(const char &cmd, const char& data, const int size);

};

#endif // COM_PORT_H
