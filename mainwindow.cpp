/* Includes */
/*====================================================*/
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <QDebug>

/* Definitions */
/*====================================================*/
#define CmdMoveAbsolute = "CMOVEABS";
#define CmdMoveRelative = "CMOVERLT";
#define CmdPowerOnMotors = "CPOWERON";
#define CmdPowerOffMotors = "CPOWEROF";
#define CmdPickComponent = "CPICKCMP";
#define CmdPlaceComponent = "CPLCECMP";

/* Constants */
/*====================================================*/
const QString build = "Build: stable_06022020";

/* Variables */
/*====================================================*/

/* Public Methods */
/*====================================================*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //load the fontys logo
    QPixmap pix(":/images/figures/Fontys-Logo.png");
    ui->img_fontysLogo->setPixmap(pix);
    ui->lbl_buildVersion->setText(build);

    //set the validator for the x,y,phi line edits
    QRegExp rx("^[0-9]{1,3}([.][0-9]{1,4})?$");
    QValidator *validator = new QRegExpValidator(rx, this);

    ui->txt_xPosition->setValidator(validator);
    ui->txt_yPosition->setValidator(validator);
//    ui->txt_zPosition->setValidator(validator);
    ui->txt_phiPosition->setValidator(validator);

    //connect the tcp/ip thread to the showReponse slot
    connect(&thread, SIGNAL(newFortune(QString)),
             this, SLOT(showResponse(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showResponse(const QString &nextFortune) //Handles responses from the TCP/IP Thread
{
    //append the received ACK from the PLC
    ui->lbl_debugConsole->appendPlainText(nextFortune);
     qDebug() << nextFortune;

    //If a batch is running send a new command whenever we receive an ACK
    if(inBatch) //&& nextFortune == "Ack")
    {
        qDebug("received an Ack");
        doBatch();
    }

}


void MainWindow::updateCmd(QString state) // Parses the command string
{
    QString command;
    if(state == "CPOWERON")
    {
        command ="CPOWERON";
    }
    else if(state == "CMOVEABS")
    {
        //Get values from the line edits
        int x = (int)( ui->txt_xPosition->text().toFloat() * 10000);
        int y = (int)( ui->txt_yPosition->text().toFloat() * 10000);
        int z = 0;
        int p = (int)( ui->txt_phiPosition->text().toFloat() * 10000);


        //Add leading zeros
        int n_zero = 8;
        std::string stringX = std::string(n_zero - std::to_string(x).length(), '0') + std::to_string(x);
        std::string stringY = std::string(n_zero - std::to_string(y).length(), '0') + std::to_string(y);
        std::string stringP = std::string(n_zero - std::to_string(p).length(), '0') + std::to_string(p);

        //Parse command
        command = "CMOVEABS;" + QString::fromStdString(stringX) + ";" + QString::fromStdString(stringY) +
                ";" + QString::fromStdString(stringP) +";" ;
    }
    else if(state == "Ping")
    {
        command = "CPING;";
    }
    else if(state == "CPOWEROF")
    {
        command = "CPOWEROF;";
    }
    ui->txt_commandString->setText(command);

}

void MainWindow::sendCommand()
{
    if (!ui->cbx_showCommandString->isChecked())
    {
        thread.sendCommand(ui->txt_ipAddress->text(), ui->txt_portNumber->text().toInt(), ui->txt_commandString->text());
    }
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
    updateCmd("CMOVEABS");
}

void MainWindow::on_txt_yPosition_editingFinished()
{
    updateCmd("CMOVEABS");
}

//void MainWindow::on_txt_zPosition_editingFinished()
//{
//    updateCmd("CMOVEABS");
//}

void MainWindow::on_txt_phiPosition_editingFinished()
{
    updateCmd("CMOVEABS");
}

void MainWindow::on_btn_send_clicked()
{
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



    int counter = 0;

    while(in.read_row(Designator, Footprint, X, Y, Layer, Rotation))
    {
        QString Qdesignator = QString::fromStdString(Designator);
        QString QFootprint = QString::fromStdString(Footprint);
        QString QX = QString::fromStdString(X);
        QString QY = QString::fromStdString(Y);
        QString QLayer = QString::fromStdString(Layer);
        QString QRotation = QString::fromStdString(Rotation);

        int x = (int)(QX.toFloat() * 10000);
        int y = (int)( QY.toFloat() * 10000);
        int z = 0;
        int p = (int)( QRotation.toFloat() * 10000);

        int offsetX = (int)(x+ ui->txt_xOffset->text().toFloat());
        int offsetY = (int)(y + ui->txt_yOffset->text().toFloat());

        int n_zero = 8;
        QString stringX = QString::fromStdString(std::string(n_zero - std::to_string(offsetX).length(), '0') + std::to_string(offsetX));
        QString stringY = QString::fromStdString(std::string(n_zero - std::to_string(offsetY).length(), '0') + std::to_string(offsetY));
        QString stringP = QString::fromStdString(std::string(n_zero - std::to_string(p).length(), '0') + std::to_string(p));




        QStandardItem *item  = new QStandardItem(Qdesignator);
        QStandardItem *item2 = new QStandardItem(QFootprint);
        QStandardItem *item3 = new QStandardItem(QX);
        QStandardItem *item4 = new QStandardItem(QY);
        QStandardItem *item5 = new QStandardItem(QLayer);
        QStandardItem *item6 = new QStandardItem(QRotation);


        QString commandStr = "CMOVEABS;" + stringX + ";" + stringY +
                ";"+ stringP +";" ;

        QStandardItem *item7 = new QStandardItem(commandStr);
        QStandardItem *item8 = new QStandardItem("");

        model->setItem(counter,0,item );
        model->setItem(counter,1,item2);
        model->setItem(counter,2,item3);
        model->setItem(counter,3,item4);
        model->setItem(counter,4,item5);
        model->setItem(counter,5,item6);
        model->setItem(counter,6,item7);
        model->setItem(counter,7,item8);
        counter++;
    }

    ui->tbl_positionFileView->setModel(model);
    batchLoaded = true;
    lenBatch = counter;

}

void MainWindow::on_btn_moveCommand_clicked()
{
    updateCmd("CMOVEABS");
    sendCommand();
}


void MainWindow::on_cbx_showCommandString_stateChanged(int arg1)
{
    ui->txt_commandString->setEnabled(arg1);
    ui->btn_send->setEnabled(arg1);
}

void MainWindow::on_btn_homeDevice_clicked()
{
    //updateCmd("CHOME");
    //sendCommand();
}

void MainWindow::on_btn_powerOn_clicked()
{
    updateCmd("CPOWERON");
    sendCommand();
}

void MainWindow::on_btn_powerOff_clicked()
{
    updateCmd("CPOWEROF");
    sendCommand();
}

void MainWindow::on_btn_sendBatch_clicked()
{
    if(!inBatch)
    {
        inBatch = true;
        doBatch();
        ui->btn_sendBatch->setEnabled(false);
        ui->btn_selectPositionFile->setEnabled(false);
    }

}


void MainWindow::doBatch()
{
    if(batchLoaded)
    {
        if(inBatch)
        {
            if(countBatch < lenBatch)
            {
                QModelIndex idx = model->index(countBatch, 6);
                QStandardItem *curItem = model->itemFromIndex(idx);

                ui->txt_positionFilePath->setText(curItem->text());

                thread.sendCommand(ui->txt_ipAddress->text(), ui->txt_portNumber->text().toInt(), curItem->text());

                QStandardItem *commandItem = new QStandardItem("Sent");

                model->setItem(countBatch,7,commandItem);
                ui->tbl_positionFileView->setModel(model);

                countBatch++;

            }
            else
            {
                inBatch = false;
                countBatch = 0;
                ui->btn_sendBatch->setEnabled(true);
                ui->btn_selectPositionFile->setEnabled(true);
            }
        }
    }
}
