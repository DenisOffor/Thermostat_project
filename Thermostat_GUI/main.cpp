#include <QApplication>
#include <QObject>
#include "mainwindow.h"
#include "com_port.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    com_port *my_com_this;
    my_com_this = new com_port;
    my_com_this->Open("COM15");

    QObject::connect(my_com_this, &com_port::sig_TempertureInBuffer, &w, &MainWindow::slot_DisplayTemperatureValue);
    QObject::connect(my_com_this, &com_port::sig_TempertureInBuffer, &w, &MainWindow::slot_PlotGraph);
    QObject::connect(&w, &MainWindow::sig_WriteNewTemperature, my_com_this, &com_port::slot_SendData);


    return a.exec();
}
