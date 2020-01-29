#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QxtCsvModel>
#include <QMainWindow>
#include "sockettest.h"
#include <QFileDialog>
#include "csv.h"
#include "tcpthread.h"
#include <QStandardItemModel>

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
    void on_b_dbgClear_clicked();

    void on_dbg_Connect_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_lineEdit_3_editingFinished();

    void on_lineEdit_4_editingFinished();

    void on_lineEdit_5_editingFinished();

    void on_lineEdit_6_editingFinished();

    void on_sendButton_clicked();

    void on_b_getFile_clicked();

    void on_btn_selectPositionFile_clicked();

    void showResponse(const QString &fortune);

private:
    Ui::MainWindow *ui;
    SocketTest cTest;

    tcpThread thread;
    void updateCmd();

};
#endif // MAINWINDOW_H
