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


        QStandardItem *item  = new QStandardItem(Qdesignator);
        QStandardItem *item2 = new QStandardItem(QFootprint);
        QStandardItem *item3 = new QStandardItem(QX);
        QStandardItem *item4 = new QStandardItem(QY);
        QStandardItem *item5 = new QStandardItem(QLayer);
        QStandardItem *item6 = new QStandardItem(QRotation);

        QString commandStr = "CMOVE;X" + QX + ";Y" + QY +
                ";Z-1;P"+ QRotation +";" ;

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
