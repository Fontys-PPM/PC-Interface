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
    ui->imageLabel->setPixmap(pix);
    ui->buildLabel->setText(build);

    QRegExp rx("^[0-9]{1,2}([.][0-9]{1,4})?$");
    QValidator *validator = new QRegExpValidator(rx, this);

    ui->lineEdit_3->setValidator(validator);
    ui->lineEdit_4->setValidator(validator);
    ui->lineEdit_5->setValidator(validator);
    ui->lineEdit_6->setValidator(validator);


    //QPixmap pcbImage(":/images/figures/PCB.png");
    //ui->img_demoPCBImage->setPixmap(pcbImage);
    //connect(pushButton_11, SIGNAL (released()), this, SLOT (test()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateCmd()
{
    QString state = ui->comboBox->currentText();
    QString command;
    if(state == "Power on")
    {
        command = "CPWON;";
    }
    else if(state == "Home")
    {
        command = "CHOME;";
    }
    else if(state == "Move")
    {
        int x = (int)( ui->lineEdit_3->text().toFloat() * 1000);
        int y = (int)( ui->lineEdit_4->text().toFloat() * 1000);
        int z = (int)( ui->lineEdit_5->text().toFloat() * 1000);
        int p = (int)( ui->lineEdit_6->text().toFloat() * 1000);

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
    ui->cmdLabel->setText(command);

}


void MainWindow::on_b_dbgClear_clicked()
{
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
    ui->lineEdit_5->setText("");
    ui->lineEdit_6->setText("");
    ui->cmdLabel->setText("");
    ui->dbgLabel->setPlainText("");
}

void MainWindow::on_dbg_Connect_clicked()
{
    //connect to the TCP/IP server on the PLC

    QString result;
    result = cTest.Connect(ui->lineEdit_20->text(),ui->lineEdit_19->text().toInt());
    ui->dbgLabel->setPlainText(result);
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(ui->comboBox->currentText() != "Move")
    {
        ui->lineEdit_3->setReadOnly(true);
        ui->lineEdit_4->setReadOnly(true);
        ui->lineEdit_5->setReadOnly(true);
        ui->lineEdit_6->setReadOnly(true);

        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Base,Qt::gray);
        palette->setColor(QPalette::Text,Qt::darkGray);
        ui->lineEdit_3->setPalette(*palette);
        ui->lineEdit_4->setPalette(*palette);
        ui->lineEdit_5->setPalette(*palette);
        ui->lineEdit_6->setPalette(*palette);
    }
    else
    {
        ui->lineEdit_3->setReadOnly(false);
        ui->lineEdit_4->setReadOnly(false);
        ui->lineEdit_5->setReadOnly(false);
        ui->lineEdit_6->setReadOnly(false);

        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Base,Qt::white);
        palette->setColor(QPalette::Text,Qt::black);
        ui->lineEdit_3->setPalette(*palette);
        ui->lineEdit_4->setPalette(*palette);
        ui->lineEdit_5->setPalette(*palette);
        ui->lineEdit_6->setPalette(*palette);
    }

    updateCmd();
}

void MainWindow::on_lineEdit_3_editingFinished()
{
    updateCmd();
}

void MainWindow::on_lineEdit_4_editingFinished()
{
    updateCmd();
}

void MainWindow::on_lineEdit_5_editingFinished()
{
    updateCmd();
}

void MainWindow::on_lineEdit_6_editingFinished()
{
    updateCmd();
}

void MainWindow::on_sendButton_clicked()
{
    QString result;



    result = cTest.Connect(ui->lineEdit_20->text(),ui->lineEdit_19->text().toInt(), ui->cmdLabel->text());
    ui->dbgLabel->setPlainText(result);

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
