#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QxtCsvModel>
#include <QMainWindow>
#include "sockettest.h"
#include <QFileDialog>
#include "csv.h"
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
    void on_btn_clearDebugConsole_clicked();

    void on_btn_connect_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_txt_xPosition_editingFinished();

    void on_txt_yPosition_editingFinished();

    void on_txt_zPosition_editingFinished();

    void on_txt_phiPosition_editingFinished();

    void on_sendButton_clicked();

    void on_b_getFile_clicked();

    void on_btn_selectPositionFile_clicked();

    void on_btn_disconnect_clicked();

    void on_pushButton_13_clicked();

    void on_btn_moveCommand_clicked();

private:
    Ui::MainWindow *ui;
    SocketTest cTest;

    void updateCmd();

};
#endif // MAINWINDOW_H
