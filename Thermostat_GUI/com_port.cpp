#include "com_port.h"

com_port::com_port()
{
    TimerForParcel = new QTimer();
    connect(TimerForParcel, &QTimer::timeout, this, &com_port::slot_SendGraphPart);

    Tx_parcel = new QByteArray();
    this_port = new QSerialPort();

    for(int row = 0; row < 320; row++) {
        QVector<uint8_t> row_vec;
        for(int col = 0; col < 30; col++)
        {
            row_vec.append(0);
        }
        Matrix8bitForGraph.append(row_vec);
    }
}

com_port::~com_port()
{
    Close();
    delete TimerForParcel;
    delete Tx_parcel;
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
    QByteArray temperature_ds;
    QByteArray temperature_ntc;
    QByteArray temperature_aht;
    QVector<uint8_t> data;

    uint8_t lowByte = 0;
    uint8_t highByte = 0;
    uint16_t number = 0;

    //if start and end bytes in buffer => parcel is got
    if(temp_buffer.at(0) == START_BYTE && temp_buffer.at(temp_buffer.size() - 1) == END_BYTE) {
        buffer = temp_buffer;
        buffer.remove(0,1); //remove start byte
        buffer.chop(1); //remove end byte
        switch (buffer.at(0)) {
        case GET_TEMPERATURE_FROM_MC:
            buffer.remove(0,1); //remove cmd
            float temp_ds, temp_ntc, temp_aht;
            std::memcpy(&temp_ds, buffer.constData(), sizeof(float));
            std::memcpy(&temp_ntc, (buffer.constData() + 4), sizeof(float));
            std::memcpy(&temp_aht, (buffer.constData() + 8), sizeof(float));
            temperature_ds.append(QString::number(temp_ds,'f', 2).toUtf8());
            temperature_ntc.append(QString::number(temp_ntc,'f', 2).toUtf8());
            temperature_aht.append(QString::number(temp_aht,'f', 2).toUtf8());
            data.append((uint8_t)buffer.at(12));
            data.append((uint8_t)buffer.at(13));
            data.append((uint8_t)buffer.at(14));
            data.append((uint8_t)buffer.at(15));
            emit sig_TempertureInBuffer(temperature_ds, temperature_ntc, temperature_aht, data);
            break;
        case PWM_ADDRESS:
            buffer.remove(0,1);
            lowByte = static_cast<uint8_t>(buffer.at(1)); // Младший байт
            highByte = static_cast<uint8_t>(buffer.at(0)); // Старший байт
            number = (static_cast<uint16_t>(highByte) << 8) | lowByte;
            qDebug() << "PWM cycle: " << number << "\n";
            break;
        default:
            return;
            break;
        }
        buffer.clear();
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
        case GET_TEMPERATURE_FROM_MC:
            buffer.remove(0,1); //remove cmd
            float temp_ds, temp_ntc, temp_aht;
            std::memcpy(&temp_ds, buffer.constData(), sizeof(float));
            std::memcpy(&temp_ntc, (buffer.constData() + 4), sizeof(float));
            std::memcpy(&temp_aht, (buffer.constData() + 8), sizeof(float));
            temperature_ds.append(QString::number(temp_ds,'f', 2).toUtf8());
            temperature_ntc.append(QString::number(temp_ntc,'f', 2).toUtf8());
            temperature_aht.append(QString::number(temp_aht,'f', 2).toUtf8());
            data.append((uint8_t)buffer.at(12));
            data.append((uint8_t)buffer.at(13));
            data.append((uint8_t)buffer.at(14));
            data.append((uint8_t)buffer.at(15));
            emit sig_TempertureInBuffer(temperature_ds, temperature_ntc, temperature_aht, data);
            break;
        case PWM_ADDRESS:
            buffer.remove(0,1);
            qDebug() << "PWM cycle: " << buffer.toInt() << "\n";
            break;
        default:
            return;
            break;
        }

        buffer.clear();
    }
}

void com_port::slot_SendData(const char &cmd, const uint8_t data[], const int size)
{
    Tx_parcel->clear();
    Tx_parcel->append(START_BYTE);
    Tx_parcel->append(cmd);
    Tx_parcel->append((size & 0x00FF));
    Tx_parcel->append(((size & 0xFF00) >> 8));
    for(int i = 0; i < size; i++)
        Tx_parcel->append(data[i]);
    Tx_parcel->append(END_BYTE);


    if (!this_port->isOpen()) {
        return;
    }

    this_port->write(*Tx_parcel);
    this_port->waitForBytesWritten(-1);
}

void com_port::slot_SendData(const char &cmd, const QByteArray data) {
    Tx_parcel->clear();
    Tx_parcel->append(START_BYTE);
    Tx_parcel->append(cmd);
    Tx_parcel->append(data.size());
    Tx_parcel->append(((data.size() & 0xFF00) >> 8));
    Tx_parcel->append(data);
    Tx_parcel->append(END_BYTE);

    this_port->write(*Tx_parcel);
    while(!this_port->waitForBytesWritten(-1));
    for(int i = 0; i < 1000; i++);
}

void com_port::slot_SendGraph(QPixmap pixmap) {
    QImage image = pixmap.toImage();
    image = image.transformed(QTransform().rotate(90));
    image = image.mirrored(false, true);

    QVector<QVector<int>> Matrix;
    for(int row = 0; row < image.height(); row++){
        QVector<int> row_vec;
        for(int col = 0; col < image.width(); col++)
        {
            row_vec.append(row*image.height());
        }
        Matrix.append(row_vec);
    }

    for(int row = 0; row < image.height(); row++) {
        for(int col = 0;col < image.width(); col++)
        {
            int n = image.pixelColor(col,row).value();
            Matrix[row][col] = (n > 250 ? 0 : 1);
        }
    }

    for(int row = 0; row < image.height(); row++){
        for(int col = 0; col < image.width(); col++)
        {
            Matrix8bitForGraph[row][col / 8] &= ~(1 << (col % 8));
            Matrix8bitForGraph[row][col / 8] |= (Matrix[row][col] << (col % 8));
        }
    }
    TimerForParcel->start(200);
}

static int parcel_iter = 0;
void com_port::slot_SendGraphPart() {
    QByteArray temp;
    for(int row = 32*parcel_iter; row < (32 + 32*parcel_iter); row++){
        for(int col = 0; col < 30; col++)
        {
            temp.append(Matrix8bitForGraph[row][col]);
        }
    }

    slot_SendData(CMD_GET_GRAPH_ON_DISPLAY,temp);

    parcel_iter++;
    if(parcel_iter == 10) {
        TimerForParcel->stop();
        this_port->waitForBytesWritten(200);
        parcel_iter = 0;
    }
}

