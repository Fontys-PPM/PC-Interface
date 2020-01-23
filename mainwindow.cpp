#include "mainwindow.h"
#include "./ui_mainwindow.h"

const QString build = "Build: alpha_23012020";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix(":/images/figures/Fontys-Logo.png");
    ui->imageLabel->setPixmap(pix);
    ui->buildLabel->setText(build);
    QPixmap pcbImage(":/images/figures/PCB.png");
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
        command = "CPOWERON;";
    }
    else if(state == "Home")
    {
        command = "CHOME;";
    }
    else if(state == "Move")
    {
        command = "CMOVE;X" + ui->lineEdit_3->text() + ";Y" + ui->lineEdit_4->text() +
                ";Z" + ui->lineEdit_5->text() +";P"+ ui->lineEdit_6->text() +";" ;
    }
    else if(state == "Ping")
    {
        command = "CPING;";
    }
    else if(state == "Power off")
    {
        command = "CPOWEROFF;";
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
    QUrl fileName = QFileDialog::getOpenFileUrl(this);

    ui->demoFile->setText(fileName.toString());

//    io::CSVReader<3> in(fileName.toString());
//    in.read_header(io::ignore_extra_column, "vendor", "size", "speed");
//    std::string vendor; int size; double speed;
//    while(in.read_row(vendor, size, speed))
//    {
//        // do stuff with the data
//    }
}
