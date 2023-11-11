#ifndef TEMPERATUREGRAPH_H
#define TEMPERATUREGRAPH_H

#include <QObject>
#include <QVector>
#include <QPair>
#include <QElapsedTimer>
#include <QByteArray>
#include "qcustomplot.h"

#define AMOUT_OF_GRAPH 3

class TemperatureGraph
{
public:
    //first - time when *pause* begin, second - full time of pause;
    QPair<double, double> Pause_time;
private:
    QCustomPlot* plot;
    //vectors for plot graph
    QVector<double> Time[AMOUT_OF_GRAPH], Temperature_value[AMOUT_OF_GRAPH];
    //XRange and Yrange for Graph obviously and YMinmax for more accurate auto scale axis with considering min and max y value
    QPair<double, double> XRange, YRange, Yminmax;
    //Timer for start time measurment
    QElapsedTimer Timer;
    int first_in = 0;
    double start_time = 0;
    double prev_data = -100;

public:
    //need to get GroupBox to constuctor in order to place QCustomPlot in widget in GropuBox
    TemperatureGraph(QGroupBox* groupBox);
    ~TemperatureGraph();
    void PlotGraph(const QByteArray&, Qt::CheckState, uint8_t sensor_number);
    void SaveGraphAs();
    void AddGraph(QColor color, uint8_t sensor_number);
    void ClearGraphs();
    void SetRange(double x_low, double x_up, double y_low, double y_up);
    void AutoAxisScale(const QByteArray&);
    bool CheckTemperatureCorrectness(const QByteArray&);
    QPair<double, double> GetXRange();
    QPair<double, double> GetYRange();
    QCustomPlot* GetPlot();
    void Vertical_line(uint8_t line_number);
    //void AutoAxisScale(QMainWindow* ui, const QByteArray& data);
    //bool CheckTemperatureCorrectness(double temprerature);
};

#endif // TEMPERATUREGRAPH_H
