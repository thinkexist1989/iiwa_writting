#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_gcodeBtn_clicked()
{
    gcodeFileName = QFileDialog::getOpenFileName(this, tr("Select G-Code File"), gcodeDefaultDirName);

    QFile gcodeFile(gcodeFileName);
    if(!gcodeFile.exists()) {
        qDebug() << "No such file!!";
    }
    gcodeFile.open(QIODevice::ReadWrite | QIODevice::Text);

    ui->plainTextEdit->setPlainText(gcodeFile.readAll());

    gcodeFile.close();
}

void MainWindow::on_execBtn_clicked()
{
    if(tcpSock == nullptr) {
        tcpSock = new QTcpSocket();
        tcpSock->connectToHost(ui->ipEdit->text(), ui->portEdit->text().toInt());
    }
//    tcpSock->connectToHost("172.31.1.150", 4009);


//    if(tcpSock.state() != QTcpSocket::ConnectedState) {
//        return;
//    }

    QFile gcodeFile(gcodeFileName);
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
                tcpSock->write(data);
                isWritten = false;
                data.clear();
            }
            data += str + "\n";
        }
        else if(str.startsWith("G01")){ //
            if(!isWritten) {
                data += "\n";
//                qDebug() << data;
                tcpSock->write(data);
                isWritten = true;
                data.clear();
            }
            data += str + "\n";
        }
        else { //坐标
            data += str +"\n";
        }

    }

    tcpSock->write("G00\nX0.0Y0.0\n\n");

    tcpSock->close();
//    tcpSock->deleteLater();


}
