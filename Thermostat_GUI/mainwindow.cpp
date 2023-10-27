#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MyGraph = new TemperatureGraph(ui->groupBox);

    for(uint8_t i = 0; i < AMOUNT_OF_BTN; i++)
        clicke[i] = false;
    pause = false;

    ui->SetPointTemperatureValueText->hide();
    ui->SetTempLabel->hide();

    this->setFixedSize(600,800);
    ShiftWidgets(-600);

    ui->BtnSaveAs->setIcon(ui->BtnSaveAs->style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->BtnSaveAs->setIconSize(QSize(25,25));
    ui->BtnClearGraph->setIcon(ui->BtnSaveAs->style()->standardIcon(QStyle::SP_DialogResetButton));
    ui->BtnClearGraph->setIconSize(QSize(25,25));
    ui->BtnPause->setIcon(ui->BtnSaveAs->style()->standardIcon(QStyle::SP_MediaPause));
    ui->BtnPause->setIconSize(QSize(25,25));

    connect(ui->Dial_Temperature, &QDial::sliderReleased, this, &MainWindow::slot_QDialReleased);
    connect(ui->Dial_Temperature, &QDial::valueChanged, this, &MainWindow::slot_DisplaySetPointTemperatureValue);

    connect(ui->TurnOnBtn, &QPushButton::clicked, this, &MainWindow::Slot_TurnOnBtn_clicked);
    connect(ui->BtnGraphicFeatures, &QPushButton::clicked, this, &MainWindow::Slot_BtnExpandGraphicFeatures_clicked);
    connect(ui->BtnThermostatFeatures, &QPushButton::clicked, this, &MainWindow::Slot_BtnExpandThermostatFeatures_clicked);
    connect(ui->BtnClearGraph, &QPushButton::clicked, this, &MainWindow::Slot_ClearGraph);
    connect(ui->BtnSaveAs, &QPushButton::clicked, this, &MainWindow::Slot_SaveGraphAs);
    connect(ui->BtnPause, &QPushButton::clicked, this, &MainWindow::Slot_PauseGraph);
    connect(ui->BtnStartHeat, &QPushButton::clicked, this, &MainWindow::Slot_HeatDuring);

    connect(ui->CB_TuneAuto, &QCheckBox::stateChanged, this, &MainWindow::Slot_CB_AutoPushed);
    connect(ui->CB_TuneManual, &QCheckBox::stateChanged, this, &MainWindow::Slot_CB_ManualPushed);

    connect(ui->XMinLineEdit, &QLineEdit::editingFinished, this, &MainWindow::Slot_ManualAxisScale);
    connect(ui->XMaxLineEdit, &QLineEdit::editingFinished, this, &MainWindow::Slot_ManualAxisScale);
    connect(ui->YMinLineEdit, &QLineEdit::editingFinished, this, &MainWindow::Slot_ManualAxisScale);
    connect(ui->YMaxLineEdit, &QLineEdit::editingFinished, this, &MainWindow::Slot_ManualAxisScale);

    connect(MyGraph->GetPlot(), &QCustomPlot::mouseWheel, this, &MainWindow::Slot_MouseAxisScale);
    connect(MyGraph->GetPlot(), &QCustomPlot::mouseMove, this, &MainWindow::Slot_MouseAxisScale);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit sig_WriteNewTemperature(CMD_TURN_OFF, 0x00, STANDART_SIZE_OF_PARCAL*sizeof(uint8_t));
    event->accept();
}

MainWindow::~MainWindow()
{
    delete MyGraph;
    delete ui;
}

void MainWindow::Slot_SaveGraphAs() {
    if(clicke[BTN_TURNON] == false)
        return;

    MyGraph->SaveGraphAs();
}

void MainWindow::slot_DisplaySetPointTemperatureValue() {
    ui->TemperatureValueText->hide();
    ui->CurrTempLabel->hide();
    ui->SetPointTemperatureValueText->show();
    ui->SetTempLabel->show();

    QString StrTempepature = QString::number(ui->Dial_Temperature->value());
    ui->SetPointTemperatureValueText->setText(StrTempepature + "°C");
        ui->ValueTargetTempLabel->setText(StrTempepature + "°C");

        const uint8_t temperature = StrTempepature.toInt();
    qDebug() << "Temperature: " << temperature << "Time: " << GetTickCount();
    emit sig_WriteNewTemperature(CMD_SEND_AIM_TEMPERATURE, temperature, STANDART_SIZE_OF_PARCAL*sizeof(uint8_t));
}

void MainWindow::slot_QDialReleased() {
    ui->TemperatureValueText->show();
    ui->CurrTempLabel->show();
    ui->SetPointTemperatureValueText->hide();
    ui->SetTempLabel->hide();
}

void MainWindow::slot_DisplayTemperatureValue(const QByteArray& data, uint8_t sensor_number)
{
    // if(MyGraph->CheckTemperatureCorrectness(data)) {
    if (sensor_number == 0) {
        QString StrTempepature = QString(data);
        ui->TemperatureValueText->setText(StrTempepature.toUtf8() + "°C");
    }
    // }
}

void MainWindow::Slot_ClearGraph() {
    if(clicke[BTN_TURNON] == false)
        return;
    if(pause == true)
        emit ui->BtnPause->clicked();

    MyGraph->ClearGraphs();
}

void MainWindow::slot_PlotGraph(const QByteArray& data, uint8_t sensor_number) {
    if(clicke[BTN_EXPAND_GPAPH_FEATURES] && !pause)
    {
        MyGraph->PlotGraph(data, ui->CB_TuneAuto->checkState(), sensor_number);

        ui->XMinLineEdit->setText(QString::number(round(MyGraph->GetXRange().first)));
        ui->XMaxLineEdit->setText(QString::number(round(MyGraph->GetXRange().second)));

        ui->YMinLineEdit->setText(QString::number(round(MyGraph->GetYRange().first)));
        ui->YMaxLineEdit->setText(QString::number(round(MyGraph->GetYRange().second)));
    }
}

void MainWindow::Slot_MouseAxisScale() {
    MyGraph->SetRange(MyGraph->GetPlot()->xAxis->range().lower, MyGraph->GetPlot()->xAxis->range().upper,
                      MyGraph->GetPlot()->yAxis->range().lower, MyGraph->GetPlot()->yAxis->range().upper);

    ui->XMinLineEdit->setText(QString::number(round(MyGraph->GetXRange().first)));
    ui->XMaxLineEdit->setText(QString::number(round(MyGraph->GetXRange().second)));

    ui->YMinLineEdit->setText(QString::number(round(MyGraph->GetYRange().first)));
    ui->YMaxLineEdit->setText(QString::number(round(MyGraph->GetYRange().second)));
}

void MainWindow::Slot_ManualAxisScale() {
    MyGraph->SetRange(ui->XMinLineEdit->text().toDouble(), ui->XMaxLineEdit->text().toDouble(),
                      ui->YMinLineEdit->text().toDouble(), ui->YMaxLineEdit->text().toDouble());
}

void MainWindow::Slot_CB_ManualPushed() {
    if(ui->CB_TuneManual->checkState() == Qt::Checked)
        ui->CB_TuneAuto->setCheckState(Qt::Unchecked);

    if(ui->CB_TuneManual->checkState() == Qt::Unchecked)
        ui->CB_TuneAuto->setCheckState(Qt::Checked);
}

void MainWindow::Slot_CB_AutoPushed() {
    if(ui->CB_TuneAuto->checkState() == Qt::Checked)
        ui->CB_TuneManual->setCheckState(Qt::Unchecked);

    if(ui->CB_TuneAuto->checkState() == Qt::Unchecked)
        ui->CB_TuneManual->setCheckState(Qt::Checked);
}

void MainWindow::Slot_TurnOnBtn_clicked()
{
    if(!clicke[BTN_TURNON]){
        ui->TurnOnBtn->setStyleSheet("QPushButton::hover{ "
                                     "background-color: white;"
                                     "border-style: outset; "
                                     "border-width: 3px; "
                                     "border-radius: 15px; "
                                     "border-color: white; "
                                     "min-width: 10em; "
                                     "color: black "
                                     "}"
                                     "QPushButton{"
                                     "background-color: white; "
                                     "border-style: outset; "
                                     "border-width: 2px; "
                                     "border-radius: 13px; "
                                     "border-color: white; "
                                     "min-width: 10em; "
                                     "color: black}");
        clicke[BTN_TURNON] = !clicke[BTN_TURNON];
        ui->Dial_Temperature->setEnabled(true);
        emit ui->BtnClearGraph->clicked();
        emit sig_WriteNewTemperature(START, 0x00, STANDART_SIZE_OF_PARCAL*sizeof(uint8_t));
    }
    else
    {
        ui->TurnOnBtn->setStyleSheet("QPushButton::hover{ "
                                     "background-color: transparent; "
                                     "border-style: outset; "
                                     "border-width: 3px; "
                                     "border-radius: 13px; "
                                     "border-color: white; "
                                     "min-width: 10em; "
                                     "color: white "
                                     "}"
                                     "QPushButton{"
                                     "background-color: transparent; "
                                     "border-style: outset; "
                                     "border-width: 2px; "
                                     "border-radius: 13px; "
                                     "border-color: white; "
                                     "min-width: 10em; "
                                     "color: white}");
        ui->ValueTargetTempLabel->setText("--");
        ui->TemperatureValueText->setText("--");
        ui->Dial_Temperature->setEnabled(false);
        if(pause == true)
            emit ui->BtnPause->clicked();
        emit sig_WriteNewTemperature(CMD_TURN_OFF, 0x00, STANDART_SIZE_OF_PARCAL*sizeof(uint8_t));
        clicke[BTN_TURNON] = !clicke[BTN_TURNON];
    }
}

void MainWindow::Slot_BtnExpandGraphicFeatures_clicked() {
    if(!clicke[BTN_EXPAND_GPAPH_FEATURES]){
        ui->BtnGraphicFeatures->setStyleSheet("QPushButton::hover{ "
                                              "background-color: rgb(90,90,90);"
                                              "border-style: outset; "
                                              "border-width: 5px; "
                                              "border-radius: 14px; "
                                              "border-color: rgb(105,105,105); "
                                              "color: white;}"
                                              "QPushButton{"
                                              "background-color: rgb(90,90,90); "
                                              "border-style: outset; "
                                              "border-width: 3px; "
                                              "border-radius: 13px; "
                                              " border-color: rgb(100,100,100); "
                                              "color: white; }");

        if(clicke[BTN_EXPAND_THERMOSTAT_FEATURES] == false) {
            this->setFixedSize(1230,800);
        }
        else {
            this->setFixedSize(1830,800);
        }

        clicke[BTN_EXPAND_GPAPH_FEATURES] = !clicke[BTN_EXPAND_GPAPH_FEATURES];
    }
    else
    {
        ui->BtnGraphicFeatures->setStyleSheet("QPushButton::hover{ "
                                              "background-color: rgb(115,115,115);"
                                              "border-style: outset; "
                                              "border-width: 5px; "
                                              "border-radius: 14px; "
                                              "border-color: rgb(105,105,105); "
                                              "color: white;}"
                                              "QPushButton{"
                                              "background-color: rgb(115,115,115); "
                                              "border-style: outset; "
                                              "border-width: 3px; "
                                              "border-radius: 13px; "
                                              "border-color: rgb(105,105,105); "
                                              "color: white;}");

        if(clicke[BTN_EXPAND_THERMOSTAT_FEATURES] == false) {
            this->setFixedSize(600,800);
        }
        else {
            this->setFixedSize(1200,800);
        }

        if(pause == true)
            emit ui->BtnPause->clicked();
        emit ui->BtnClearGraph->clicked();
        clicke[BTN_EXPAND_GPAPH_FEATURES] = !clicke[BTN_EXPAND_GPAPH_FEATURES];
    }
}

void MainWindow::Slot_BtnExpandThermostatFeatures_clicked() {
    if(!clicke[BTN_EXPAND_THERMOSTAT_FEATURES]){
        ui->groupBox->setUpdatesEnabled(false);
        this->repaint();
        ui->BtnThermostatFeatures->setStyleSheet("QPushButton::hover{ "
                                                 "background-color: rgb(90,90,90);"
                                                 "border-style: outset; "
                                                 "border-width: 5px; "
                                                 "border-radius: 14px; "
                                                 "border-color: rgb(105,105,105); "
                                                 "color: white;}"
                                                 "QPushButton{"
                                                 "background-color: rgb(90,90,90); "
                                                 "border-style: outset; "
                                                 "border-width: 3px; "
                                                 "border-radius: 13px; "
                                                 " border-color: rgb(100,100,100); "
                                                 "color: white; }");
        clicke[BTN_EXPAND_THERMOSTAT_FEATURES] = !clicke[BTN_EXPAND_THERMOSTAT_FEATURES];
        if(clicke[BTN_EXPAND_GPAPH_FEATURES] == false) {
            ShiftWidgets(600);
            this->setFixedSize(1200,800);
        }
        else {
            ShiftWidgets(600);
            this->setFixedSize(1830,800);
        }
        this->setGeometry(this->x() - 599, this->y() + 31, this->width(), this->height());
        this->repaint();
        ui->groupBox->setUpdatesEnabled(true);

    }
    else
    {
        ui->groupBox->setUpdatesEnabled(false);
        this->repaint();
        ui->BtnThermostatFeatures->setStyleSheet("QPushButton::hover{ "
                                                 "background-color: rgb(115,115,115);"
                                                 "border-style: outset; "
                                                 "border-width: 5px; "
                                                 "border-radius: 14px; "
                                                 "border-color: rgb(105,105,105); "
                                                 "color: white;}"
                                                 "QPushButton{"
                                                 "background-color: rgb(115,115,115); "
                                                 "border-style: outset; "
                                                 "border-width: 3px; "
                                                 "border-radius: 13px; "
                                                 "border-color: rgb(105,105,105); "
                                                 "color: white;}");
        clicke[BTN_EXPAND_THERMOSTAT_FEATURES] = !clicke[BTN_EXPAND_THERMOSTAT_FEATURES];
        if(clicke[BTN_EXPAND_GPAPH_FEATURES] == false) {
            ShiftWidgets(-600);
            this->setFixedSize(600,800);
        }
        else {
            ShiftWidgets(-600);
            this->setFixedSize(1230,800);
        }
        this->setGeometry(this->x() + 601, this->y() + 31, this->width(), this->height());
        this->repaint();
        ui->groupBox->setUpdatesEnabled(true);
    }
}

void MainWindow::Slot_PauseGraph() {

    if(clicke[BTN_TURNON] == false)
        return;
    if(!pause){
        MyGraph->Pause_time.first = GetTickCount();
        ui->BtnPause->setStyleSheet("QPushButton::hover{ "
                                    "background-color: rgb(90,90,90);"
                                    "border-style: outset; "
                                    "border-width: 5px; "
                                    "border-radius: 14px; "
                                    "border-color: rgb(105,105,105); "
                                    "color: white;}"
                                    "QPushButton{"
                                    "background-color: rgb(90,90,90); "
                                    "border-style: outset; "
                                    "border-width: 3px; "
                                    "border-radius: 13px; "
                                    " border-color: rgb(100,100,100); "
                                    "color: white; }");
        pause = !pause;
    }
    else
    {
        MyGraph->Pause_time.second += GetTickCount() - MyGraph->Pause_time.first;
        ui->BtnPause->setStyleSheet("QPushButton::hover{ "
                                    "background-color: rgb(115,115,115);"
                                    "border-style: outset; "
                                    "border-width: 5px; "
                                    "border-radius: 14px; "
                                    "border-color: rgb(105,105,105); "
                                    "color: white;}"
                                    "QPushButton{"
                                    "background-color: rgb(115,115,115); "
                                    "border-style: outset; "
                                    "border-width: 3px; "
                                    "border-radius: 13px; "
                                    "border-color: rgb(105,105,105); "
                                    "color: white;}");
        pause = !pause;
    }
}

void MainWindow::Slot_HeatDuring() {
    if(clicke[BTN_TURNON] == false)
        return;

    int time = ui->HeatTimeLineEdit->text().toInt();
    if(time <= 0 || time > 60)
        return;

    emit sig_WriteNewTemperature(CMD_SEND_HEAT_TIME, time, STANDART_SIZE_OF_PARCAL*sizeof(uint8_t));
}

void MainWindow::ShiftWidgets(int shift) {
    QList<QWidget*> childWidgets = ui->groupBox->findChildren<QWidget*>();
    for (QWidget* childWidget : childWidgets) {
        QPoint currentPos = childWidget->pos();
        childWidget->move(currentPos.x() + shift, currentPos.y());
    }
}
