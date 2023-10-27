#include "com_port.h"

com_port::com_port()
{
    this_port = new QSerialPort();
}

com_port::~com_port()
{
    Close();
    delete this_port;
}

bool com_port::Open(const QString &name_port)
{
    this_port->setPortName(name_port);
    bool status = this_port->open(QIODevice::ReadWrite);
    assert(status == true);


    int status_error = SetUp();
    emit sig_Opened(name_port, status_error);

    connect(this_port, &QSerialPort::readyRead, this, &com_port::slot_GetData);

    return (status_error == 0 ? true : false);
}

void com_port::Close()
{
    this_port->close();
}

int com_port::SetUp()
{
    int status_error = 0;
    status_error =  this_port->setBaudRate(QSerialPort::Baud115200) == false ? (1 << 0) :
                    this_port->setDataBits(QSerialPort::Data8) == false ? (1 << 1) :
                    this_port->setFlowControl(QSerialPort::NoFlowControl) == false ? (1 << 2) :
                    this_port->setParity(QSerialPort::NoParity) == false ? (1 << 3) :
                    this_port->setStopBits(QSerialPort::OneStop) == false ? (1 << 4) : 0;

   if (!this_port->isOpen()) {
       status_error = 0xFF;
   }
   return status_error;
}

void com_port::slot_GetData()
{
    QByteArray temp_buffer = this_port->readAll();

    //if start and end bytes in buffer => parcel is got
    if(temp_buffer.at(0) == START_BYTE && temp_buffer.at(temp_buffer.size() - 1) == END_BYTE) {
        buffer = temp_buffer;
        buffer.remove(0,1); //remove start byte
        buffer.chop(1); //remove end byte

        switch (buffer.at(0)) {
        case DS18B20_ADDRESS:
            buffer.remove(0,1);
            emit sig_TempertureInBuffer(buffer, 0);
            break;
        case NTC_ADDRESS:
            buffer.remove(0,1);
            emit sig_TempertureInBuffer(buffer, 1);
            break;
        default:
            return;
            break;
        }
        return;
    }

    //if there are only start byte => just push it to buf and wait for rest of parcel
    if(temp_buffer.at(0) == START_BYTE && temp_buffer.at(temp_buffer.size() - 1) != END_BYTE) {
        buffer = temp_buffer;
        buffer.remove(0,1); //remove start byte
        return;
    }

    //if there are only end byte => rest of parcel arrived so emit signal
    if(temp_buffer.at(0) != START_BYTE && temp_buffer.at(temp_buffer.size() - 1) == END_BYTE) {
        buffer.append(temp_buffer);
        buffer.chop(1); //remove end byte

        switch (buffer.at(0)) {
        case DS18B20_ADDRESS:
            buffer.remove(0,1);
            emit sig_TempertureInBuffer(buffer, 0);
            break;
        case NTC_ADDRESS:
            buffer.remove(0,1);
            emit sig_TempertureInBuffer(buffer, 1);
            break;
        default:
            return;
            break;
        }
    }
}

void com_port::slot_SendData(const char &cmd, const char &data, const int size)
{
    char parcel[2] = {cmd, data};
    if (!this_port->isOpen()) {
        return;
    }

    this_port->write(&parcel[0], size);
}
