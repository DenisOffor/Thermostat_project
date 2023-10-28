#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QDebug>
#include "temperaturegraph.h"
#include "com_port.h"

#define AMOUNT_OF_BTN 3
#define BTN_TURNON 0
#define BTN_EXPAND_GPAPH_FEATURES 1
#define BTN_EXPAND_THERMOSTAT_FEATURES 2

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    TemperatureGraph* MyGraph;
    com_port *my_com_this;
    Ui::MainWindow *ui;
    //bool for buttoms push check
    bool clicke[AMOUNT_OF_BTN];
    //bool for set program pause state (Graph will not clear, temperature measurment will be paused, aim temeprature also not clear)
    bool pause;
    void ShiftWidgets(int shift);

public slots:
    void slot_DisplayTemperatureValue(const QByteArray& data, uint8_t sensor_number);
    void slot_PlotGraph(const QByteArray& data, uint8_t sensor_number);
signals :
    void sig_WriteNewData(const uint8_t& cmd,const uint8_t data[], const int size);

private slots:
    void slot_DisplaySetPointTemperatureValue();
    void slot_QDialReleased();
    void closeEvent(QCloseEvent *event);
    void Slot_TurnOnBtn_clicked();
    void Slot_BtnExpandGraphicFeatures_clicked();
    void Slot_BtnExpandThermostatFeatures_clicked();
    void Slot_CB_ManualPushed();
    void Slot_CB_AutoPushed();
    void Slot_ManualAxisScale();
    void Slot_MouseAxisScale();
    void Slot_SaveGraphAs();
    void Slot_ClearGraph();
    void Slot_PauseGraph();
    void Slot_HeatDuring();
    void Slot_SendPidCoef();
    void Slot_DisplayGraphOnMC();
};
#endif // MAINWINDOW_H
