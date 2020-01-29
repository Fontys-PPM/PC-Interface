#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>




const QString build = "Build: alpha_27012020";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix(":/images/figures/Fontys-Logo.png");
    ui->img_fontysLogo->setPixmap(pix);
    ui->lbl_buildVersion->setText(build);

    QRegExp rx("^[0-9]{1,3}([.][0-9]{1,3})?$");
    QValidator *validator = new QRegExpValidator(rx, this);

    ui->txt_xPosition->setValidator(validator);
    ui->txt_yPosition->setValidator(validator);
    ui->txt_zPosition->setValidator(validator);
    ui->txt_phiPosition->setValidator(validator);

    connect(&thread, SIGNAL(newFortune(QString)),
             this, SLOT(showResponse(QString)));


    //QPixmap pcbImage(":/images/figures/PCB.png");
    //ui->img_demoPCBImage->setPixmap(pcbImage);
    //connect(pushButton_11, SIGNAL (released()), this, SLOT (test()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showResponse(const QString &nextFortune)
{
    ui->lbl_debugConsole->appendPlainText(nextFortune);

}


void MainWindow::updateCmd(QString state)
{
    QString command;
    if(state == "Power on")
    {
        command = "CPWON;";
    }
    else if(state == "Home")
    {
        command = "CHOME;";
    }
    else if(state == "CMOVE")
    {
        int x = (int)( ui->txt_xPosition->text().toFloat() * 1000);
        int y = (int)( ui->txt_yPosition->text().toFloat() * 1000);
        int z = (int)( ui->txt_zPosition->text().toFloat() * 1000);
        int p = (int)( ui->txt_phiPosition->text().toFloat() * 1000);

        int leading = 6; //6 at max
        std::stringstream ssX, ssY, ssZ, ssP;
        ssX <<  std::to_string(x*0.000001).substr(8-leading); //="042"
        ssY <<  std::to_string(y*0.000001).substr(8-leading);
        ssZ <<  std::to_string(z*0.000001).substr(8-leading);
        ssP <<  std::to_string(p*0.000001).substr(8-leading);
        command = "CMOVE;" + QString::fromStdString(ssX.str()) + ";" + QString::fromStdString(ssY.str()) +
                ";" + QString::fromStdString(ssZ.str()) +";"+ QString::fromStdString(ssP.str()) +";" ;
    }
    else if(state == "Ping")
    {
        command = "CPING;";
    }
    else if(state == "Power off")
    {
        command = "CPOFF;";
    }
    ui->txt_commandString->setText(command);

}


void MainWindow::on_btn_clearDebugConsole_clicked()
{
//    ui->txt_xPosition->setText("");
//    ui->txt_yPosition->setText("");
//    ui->txt_zPosition->setText("");
//    ui->txt_phiPosition->setText("");
//    ui->txt_commandString->setText("");
    ui->lbl_debugConsole->setPlainText("");
}

void MainWindow::on_btn_connect_clicked()
{
    //connect to the TCP/IP server on the PLC

//    QString result;
//    result = cTest.Connect(ui->txt_ipAddress->text(),ui->txt_portNumber->text().toInt());
//    ui->lbl_debugConsole->setPlainText(result);
    thread.sendCommand(ui->txt_ipAddress->text(), ui->txt_portNumber->text().toInt(), ui->txt_commandString->text());
}

void MainWindow::on_txt_xPosition_editingFinished()
{
    updateCmd("CMOVE");
}

void MainWindow::on_txt_yPosition_editingFinished()
{
    updateCmd("CMOVE");
}

void MainWindow::on_txt_zPosition_editingFinished()
{
    updateCmd("CMOVE");
}

void MainWindow::on_txt_phiPosition_editingFinished()
{
    updateCmd("CMOVE");
}

void MainWindow::on_btn_send_clicked()
{
    QString result;
    //result = cTest.Connect(ui->txt_ipAddress->text(),ui->txt_portNumber->text().toInt(), ui->txt_commandString->text());
    //ui->lbl_debugConsole->setPlainText(result);

    thread.sendCommand(ui->txt_ipAddress->text(), ui->txt_portNumber->text().toInt(), ui->txt_commandString->text());


}

void MainWindow::on_b_getFile_clicked()
{

}

void MainWindow::on_btn_selectPositionFile_clicked()
{
    QUrl fileName = QFileDialog::getOpenFileUrl(this);

    ui->txt_positionFilePath->setText(fileName.toLocalFile());
    //ui->txt_positionFilePath->setText("C:/Users/thijs/OneDrive/Desktop/ppm2/Files/SMT_Coordinate_Template.csv");

    //io::CSVReader<6> in("C:/Users/thijs/OneDrive/Desktop/ppm2/Files/SMT_Coordinate_Template.csv");
    io::CSVReader<6> in( fileName.toLocalFile().toStdString());
    in.read_header(io::ignore_extra_column, "Designator", "Footprint", "X","Y","Layer","Rotation");
    std::string Designator; std::string Footprint; std::string X; std::string Y; std::string Layer; std::string Rotation;

    QStandardItemModel *model = new QStandardItemModel;

    int counter = 0;

    while(in.read_row(Designator, Footprint, X, Y, Layer, Rotation))
    {
        QString Qdesignator = QString::fromStdString(Designator);
        QString QFootprint = QString::fromStdString(Footprint);
        QString QX = QString::fromStdString(X);
        QString QY = QString::fromStdString(Y);
        QString QLayer = QString::fromStdString(Layer);
        QString QRotation = QString::fromStdString(Rotation);

        QString offsetX =QString::number( QX.toFloat() + ui->txt_xOffset->text().toFloat());
        QString offsetY =QString::number( QY.toFloat() + ui->txt_yOffset->text().toFloat());

        QStandardItem *item  = new QStandardItem(Qdesignator);
        QStandardItem *item2 = new QStandardItem(QFootprint);
        QStandardItem *item3 = new QStandardItem(offsetX);
        QStandardItem *item4 = new QStandardItem(offsetY);
        QStandardItem *item5 = new QStandardItem(QLayer);
        QStandardItem *item6 = new QStandardItem(QRotation);

        QString commandStr = "CMOVE;" + offsetX + ";" + offsetY +
                ";-1;"+ QRotation +";" ;

        QStandardItem *item7 = new QStandardItem(commandStr);

        model->setItem(counter,0,item );
        model->setItem(counter,1,item2);
        model->setItem(counter,2,item3);
        model->setItem(counter,3,item4);
        model->setItem(counter,4,item5);
        model->setItem(counter,5,item6);
        model->setItem(counter,6,item7);
        counter++;
    }

    ui->tbl_positionFileView->setModel(model);
}

void MainWindow::on_btn_moveCommand_clicked()
{
    QString command;
    int x = (int)( ui->txt_xPosition->text().toFloat() * 1000);
    int y = (int)( ui->txt_yPosition->text().toFloat() * 1000);
    int z = (int)( ui->txt_zPosition->text().toFloat() * 1000);
    int p = (int)( ui->txt_phiPosition->text().toFloat() * 1000);

    int leading = 6; //6 at max
    std::stringstream ssX, ssY, ssZ, ssP;
    ssX <<  std::to_string(x*0.000001).substr(8-leading); //="042"
    ssY <<  std::to_string(y*0.000001).substr(8-leading);
    ssZ <<  std::to_string(z*0.000001).substr(8-leading);
    ssP <<  std::to_string(p*0.000001).substr(8-leading);
    command = "CMOVE;" + QString::fromStdString(ssX.str()) + ";" + QString::fromStdString(ssY.str()) +
            ";" + QString::fromStdString(ssZ.str()) +";"+ QString::fromStdString(ssP.str()) +";" ;
    updateCmd("CMOVE");
    thread.sendCommand(ui->txt_ipAddress->text(), ui->txt_portNumber->text().toInt(), ui->txt_commandString->text());
}

void MainWindow::on_cbx_showCommandString_stateChanged(int arg1)
{
    ui->txt_commandString->setEnabled(arg1);
    ui->btn_send->setEnabled(arg1);
}
