#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    line1Offset = QPointF(15.1, 115.1);
    line2Offset = QPointF(15.1, 15.1);

    nameOffset = QPointF(30.1, 75.1);
    sigOffset  = QPointF(130.1, 15.1);

    sigFileName = "";
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_gcode1Btn_clicked()
{
    gcode1FileName = QFileDialog::getOpenFileName(this, tr("Select G-Code File"), gcodeDefaultDirName);

    QFile gcodeFile(gcode1FileName);
    if(!gcodeFile.exists()) {
        qDebug() << "No such file!!";
    }
    gcodeFile.open(QIODevice::ReadWrite | QIODevice::Text);

    ui->text1->setPlainText(gcodeFile.readAll());

    gcodeFile.close();
}

void MainWindow::send_gcode(QString fileName,QPointF origin, QTcpSocket* tcpSocket)
{
    //先设置零点
    QString str = QString("SX%1Y%2\n\n").arg(origin.x()).arg(origin.y());
    tcpSock->write(str.toLocal8Bit());


    QFile gcodeFile(fileName);
    if(!gcodeFile.exists()) {
        qDebug() << "No such file!!";
    }
    gcodeFile.open(QIODevice::ReadWrite | QIODevice::Text);

    QTextStream in(&gcodeFile);

    bool isWritten = false;
    QByteArray data;

    while(!in.atEnd()) {
        QString str = in.readLine();
//        qDebug() << str;

        if(str.startsWith("G00")) {
            if(isWritten) {
                data += "\n";
//                qDebug() << data;
                tcpSocket->write(data);
                isWritten = false;
                data.clear();
            }
            data += str + "\n";
        }
        else if(str.startsWith("G01")){ //
            if(!isWritten) {
                data += "\n";
//                qDebug() << data;
                tcpSocket->write(data);
                isWritten = true;
                data.clear();
            }
            data += str + "\n";
        }
        else { //坐标
            data += str +"\n";
        }

    }

    tcpSocket->write("G00\n\n");
}

void MainWindow::on_execBtn_clicked()
{
    if(tcpSock == nullptr) {
        return;
    }

    if(mode == 0) {
        if(ui->check1->isChecked()) { //第一行有字

            send_gcode(gcode1FileName,line1Offset, tcpSock);
        }

        if(ui->check2->isChecked()) { //第二行有字
            send_gcode(gcode2FileName,line2Offset, tcpSock);
        }
    }
    else if(mode == 1) { //直接用line1写名字
        send_gcode(gcode1FileName, nameOffset, tcpSock);
        send_gcode(sigFileName, sigOffset, tcpSock);
    }


    tcpSock->write("G00\nSX0.0Y0.0\n\n");

}

void MainWindow::on_stickFontBtn_clicked()
{
    QProcess::startDetached("C:/Users/think/Documents/GitHub/iiwa_writting/StickFont/StickFont.exe");
}

void MainWindow::on_modeBox_currentIndexChanged(int index)
{
    qDebug() <<"index " << index << " is " << ui->modeBox->currentText();

    mode = index;

    switch(mode) {
    case 0:
        ui->check1->setDisabled(false);
        ui->check2->setDisabled(false);
        ui->gcode2Btn->setEnabled(true);
        break;
    case 1:
        ui->check1->setChecked(true);
        ui->check2->setChecked(false);
        ui->check1->setDisabled(true);
        ui->check2->setDisabled(true);
        ui->gcode2Btn->setDisabled(true);
        {
            QFile gcodeFile(sigFileName);
            if(!gcodeFile.exists()) {
                qDebug() << "No such file!!";
            }
            gcodeFile.open(QIODevice::ReadWrite | QIODevice::Text);

            ui->text2->setPlainText(gcodeFile.readAll());
        }
        break;
    default:
        break;
    }

}

void MainWindow::on_gcode2Btn_clicked()
{
    gcode2FileName = QFileDialog::getOpenFileName(this, tr("Select G-Code File"), gcodeDefaultDirName);

    QFile gcodeFile(gcode2FileName);
    if(!gcodeFile.exists()) {
        qDebug() << "No such file!!";
    }
    gcodeFile.open(QIODevice::ReadWrite | QIODevice::Text);

    ui->text2->setPlainText(gcodeFile.readAll());

    gcodeFile.close();
}

void MainWindow::on_selectDir_clicked()
{
    rootDirName = QFileDialog::getExistingDirectory(
                    this,
                    tr("Select Root Dir"),
                    "C:");

        ui->rootDir->setText(rootDirName);
        sigFileName = rootDirName + "/bySIA.nc";
}

void MainWindow::on_connectBtn_clicked()
{
    if(tcpSock == nullptr) {
        tcpSock = new QTcpSocket();
        tcpSock->connectToHost(ui->ipEdit->text(), ui->portEdit->text().toInt());

        ui->connectBtn->setText("断开");
    }
    else {
        tcpSock->close();
        tcpSock = nullptr;

        ui->connectBtn->setText("连接");
    }

}
