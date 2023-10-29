#ifndef COM_PORT_H
#define COM_PORT_H

#include <QObject>
#include <QSerialPort>
#include <QString>
#include <QDebug>
#include <assert.h>
#include <QImage>
#include <QPixmap>
#include <QVector>
#include <QElapsedTimer>

#define START_BYTE 0x01
#define END_BYTE 0x09
#define DS18B20_ADDRESS 0x43
#define NTC_ADDRESS 0x44
#define PWM_ADDRESS 0x45

#define CMD_TURN_OFF 0x10
#define START 0x11
#define CMD_SEND_AIM_TEMPERATURE 0x25
#define CMD_SEND_HEAT_TIME 0x31
#define CMD_SENSOR_CHOOSE 0x32
#define CMD_SET_PID_COEF 0x41
#define CMD_GET_GRAPH_ON_DISPLAY 0x43
#define CMD_DRAW_GRAPH 0x44
#define CMD_DRAW_TEMPERATURE 0x45

class com_port : public QObject
{
    Q_OBJECT
public:
    QSerialPort *this_port;
    QByteArray* Tx_parcel;
    QElapsedTimer Timer;
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
    void slot_SendGraph(QPixmap pixmap);
    void slot_SendData(const char &cmd, const uint8_t data[], const int size);
};

#endif // COM_PORT_H
