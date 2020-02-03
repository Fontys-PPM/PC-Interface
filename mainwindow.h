#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QxtCsvModel>
#include <QMainWindow>
#include "sockettest.h"
#include <QFileDialog>
#include "csv.h"
#include "tcpthread.h"
#include <QStandardItemModel>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:


private slots:
    void on_btn_clearDebugConsole_clicked();

    void on_btn_connect_clicked();

    void on_txt_xPosition_editingFinished();

    void on_txt_yPosition_editingFinished();

    void sendCommand();

    void on_txt_phiPosition_editingFinished();

    void on_btn_send_clicked();

    void on_b_getFile_clicked();

    void on_btn_selectPositionFile_clicked();

    void on_btn_moveCommand_clicked();

    void showResponse(const QString &fortune);


    void on_cbx_showCommandString_stateChanged(int arg1);

    void on_btn_homeDevice_clicked();

    void on_btn_powerOn_clicked();

    void on_btn_powerOff_clicked();

    void on_btn_sendBatch_clicked();

private:
    Ui::MainWindow *ui;
    SocketTest cTest;

    tcpThread thread;
    void updateCmd(QString state);


    void doBatch();

    bool batchLoaded = false;
    bool inBatch = false;
    int countBatch = 0;
    int lenBatch = -1;
    QStandardItemModel *model = new QStandardItemModel;

};
#endif // MAINWINDOW_H
