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
#include <QTimer>
#include <cstring>

#define START_BYTE 0x01
#define END_BYTE 0x09
#define GET_TEMPERATURE_FROM_MC 0x42
#define PWM_ADDRESS 0x45

#define CMD_TURN_ON_OFF 0x10
#define CMD_SEND_AIM_TEMPERATURE 0x25
#define CMD_SEND_HEAT_TIME 0x31
#define CMD_SENSOR_CHOOSE 0x32
#define CMD_STEP_HEAT 0x33
#define CMD_HEAT_IN_TIME 0x34
#define CMD_SET_RELAY_COEF 0x40
#define CMD_SET_PID_COEF 0x41
#define CMD_GET_GRAPH_ON_DISPLAY 0x43
#define CMD_DRAW_CHOOSE 0x44

#define RESET_TEMPERATURE 255


class com_port : public QObject
{
    Q_OBJECT
public:
    QSerialPort *this_port;
    QByteArray* Tx_parcel;
    QElapsedTimer Timer;
    QTimer* TimerForParcel;
    QVector<QVector<uint8_t>> Matrix8bitForGraph;
    com_port();
    ~com_port();

    bool Open(const QString& name_port);
    void Close();
    int SetUp();

    QByteArray buffer;

signals :
    void sig_Opened(const QString& port_name, int status_error);
    void sig_TempertureInBuffer(const QByteArray temp_ds, const QByteArray temp_ntc, const QByteArray temp_aht, QVector<uint8_t> sensors_state);
public slots:
    void slot_GetData();
    void slot_SendGraph(QPixmap pixmap);
    void slot_SendData(const char &cmd, const uint8_t data[], const int size);
    void slot_SendData(const char &cmd, const QByteArray data);
    void slot_SendGraphPart();
};

#endif // COM_PORT_H
