#include "temperaturegraph.h"


TemperatureGraph::TemperatureGraph(QGroupBox* groupBox)
{
    plot = new QCustomPlot(groupBox);
    AddGraph(QColor(0,0,0), 0);
    AddGraph(QColor(20,100,20), 1);
    AddGraph(QColor(30,50,70), 2);
    plot->setObjectName(QString::fromUtf8("Graph"));
    plot->setGeometry(QRect(1210, 60, 591, 441));
    plot->setStyleSheet(QString::fromUtf8("background-color: rgb(128,128,128);"));

    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(128, 128, 128));
    gradient.setColorAt(0.38, QColor(128, 128, 128));
    gradient.setColorAt(1, QColor(128, 128, 128));
    plot->setBackground(QBrush(gradient));
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);


    plot->xAxis->setLabel("Время, с");
    plot->yAxis->setLabel("Температура, °C");

    plot->replot();
}

TemperatureGraph::~TemperatureGraph() {
    delete plot;
}

void TemperatureGraph::PlotGraph(const QByteArray& data, Qt::CheckState CheckState, uint8_t sensor_number) {
    double temperature = data.toDouble();

    if(first_in == 0) {
        start_time = GetTickCount();
        Pause_time.first = Pause_time.second = 0;
        first_in = 1;

        Yminmax.first = temperature - temperature / 10;
        Yminmax.second = temperature + temperature / 10;
    }

    if(CheckState == Qt::Checked)
        AutoAxisScale(data);

    Time[sensor_number].append((GetTickCount() - start_time - Pause_time.second) / 1000);
    Temperature_value[sensor_number].append(temperature);

    plot->graph(sensor_number)->setData(Time[sensor_number], Temperature_value[sensor_number]);
    plot->replot();
}

void TemperatureGraph::SaveGraphAs() {
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(255, 255, 255));
    gradient.setColorAt(0.38, QColor(255, 255, 255));
    gradient.setColorAt(1, QColor(255, 255, 255));
    plot->setBackground(QBrush(gradient));

    QString filename = QFileDialog::getSaveFileName();

    if( filename.endsWith(".png") ){
        plot->savePng(filename, plot->width(), plot->height() );
    }
    if( filename.endsWith(".jpg") ){
        plot->saveJpg(filename, plot->width(), plot->height() );
    }
    if( filename.endsWith(".jpeg") ){
        plot->saveJpg(filename, plot->width(), plot->height() );
    }
    if( filename.endsWith(".pdf") ){
        plot->savePdf(filename, plot->width(), plot->height() );
    }

    //If we get this far then save as a png
    plot->savePng(filename, plot->width(), plot->height());

    gradient.setColorAt(0, QColor(128, 128, 128));
    gradient.setColorAt(0.38, QColor(128, 128, 128));
    gradient.setColorAt(1, QColor(128, 128, 128));
    plot->setBackground(QBrush(gradient));
}

void TemperatureGraph::AddGraph(QColor color, uint8_t sensor_number) {
    plot->addGraph();
    QPen pen;
    pen.setWidth(2);
    pen.setColor(color);
    plot->graph(sensor_number)->setPen(pen);

    XRange.first = 0;
    XRange.second = 5;
    YRange.first = 0;
    YRange.second = 5;


    plot->xAxis->setRange(XRange.first, XRange.second);
    plot->yAxis->setRange(YRange.first, YRange.second);
}

void TemperatureGraph::ClearGraphs() {
    for(int i = 0; i < AMOUT_OF_GRAPH; i++) {
        Time[i].clear();
        Temperature_value[i].clear();
    }
    first_in = 0;
    start_time = 0;
    plot->clearGraphs();
    AddGraph(QColor(0,0,0), 0);
    AddGraph(QColor(20,100,20), 1);
    AddGraph(QColor(30,50,70), 2);
    plot->replot();
}

void TemperatureGraph::SetRange(double x_low, double x_up, double y_low, double y_up) {
    XRange.first = x_low;
    XRange.second = x_up;

    YRange.first = y_low;
    YRange.second = y_up;

    plot->xAxis->setRange(XRange.first, XRange.second);
    plot->yAxis->setRange(YRange.first, YRange.second);
    plot->replot();
}

void TemperatureGraph::AutoAxisScale(const QByteArray& data) {
    if(CheckTemperatureCorrectness(data))
    {
        double temperature = data.toDouble();

        if(temperature > Yminmax.second)
            Yminmax.second = temperature;
        if(temperature < Yminmax.first)
            Yminmax.first = temperature;

        XRange.first = 0;
        XRange.second = (GetTickCount() - start_time - Pause_time.second) / 1000 + (XRange.second - XRange.first) / 10;

        YRange.first =  Yminmax.first - Yminmax.first*0.1;
        YRange.second = Yminmax.second + Yminmax.second*0.1;

        plot->xAxis->setRange(XRange.first, XRange.second);
        plot->yAxis->setRange(YRange.first, YRange.second);
    }
}

bool TemperatureGraph::CheckTemperatureCorrectness(const QByteArray& data) {
    bool temp = (abs(prev_data - data.toDouble()) < 1) ? true : prev_data == -100 ? true : false;
    temp &= (data.at(data.size() - 1) != '.');
    temp &= (data.at(0) != '.');
    return temp;
}

QPair<double, double> TemperatureGraph::GetXRange() {
    return XRange;
}

QPair<double, double> TemperatureGraph::GetYRange() {
    return YRange;
}

QCustomPlot* TemperatureGraph::GetPlot() {
    return plot;
}
