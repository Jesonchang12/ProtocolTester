#include "TcpTestWidget.h"
#include "ui_TcpTestWidget.h"
#include <QDebug>
#include <QMessageBox>

TcpTestWidget::TcpTestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpTestWidget),
    m_tcpSocket(nullptr),
    m_isServer(true)
{
    ui->setupUi(this);
    connect(&m_tcpServer, QTcpServer::newConnection, this, &TcpTestWidget::slot_newConnection);
    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, &TcpTestWidget::slot_sendInfo);
    on_comboBox_currentIndexChanged(0);
}

TcpTestWidget::~TcpTestWidget()
{
    delete ui;
}

void TcpTestWidget::on_pushButton_clicked()
{
    if(m_isServer){
        if(m_tcpServer.listen(QHostAddress::AnyIPv4, ui->lineEdit_2->text().toInt()))       //只监听IPV4的所有客户端
        {
            ui->pushButton->setStyleSheet("background-color: rgb(0, 255, 255);");
            ui->pushButton_2->setStyleSheet("background-color: rgb(255, 255, 0);");
            ui->label_4->setText("提示: 监听!!!");
        }
        else{
            ui->pushButton->setStyleSheet("background-color: rgb(255, 255, 0);");
            ui->pushButton_2->setStyleSheet("background-color: rgb(0, 255, 255);");
            ui->label_4->setText("提示: 已监听!!!");
        }
    }
    else{
        if(!m_tcpSocket){
            m_tcpSocket = new QTcpSocket();
            connect(m_tcpSocket, &QTcpSocket::connected, this, &TcpTestWidget::onServerConnected);
            connect(m_tcpSocket, &QTcpSocket::disconnected, this, &TcpTestWidget::onServerDisconnected, Qt::QueuedConnection);
            connect(m_tcpSocket, &QTcpSocket::readyRead,this,&TcpTestWidget::onServerDataReady);
    //        connect(m_tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
            m_tcpSocket->connectToHost(ui->lineEdit->text(),ui->lineEdit_2->text().toInt());

        }
        else{
            ui->pushButton->setStyleSheet("background-color: rgb(255, 255, 0);");
            ui->pushButton_2->setStyleSheet("background-color: rgb(0, 255, 255);");
            ui->label_4->setText("提示: 关闭，请重试!!!");
            m_tcpSocket->disconnect();
            m_tcpSocket->close();
            delete m_tcpSocket;
            m_tcpSocket = nullptr;
        }
    }
}

void TcpTestWidget::on_pushButton_2_clicked()
{
    if(m_isServer){
        m_tcpServer.close();
    }
    if(m_tcpSocket){
        ui->label->setText("");
        m_tcpSocket->disconnect();
        delete m_tcpSocket;
        m_tcpSocket = nullptr;
    }
    ui->label->setText("");
    ui->label_4->setText("提示: 关闭成功!!!");
    ui->pushButton->setStyleSheet("background-color: rgb(255, 255, 0);");
    ui->pushButton_2->setStyleSheet("background-color: rgb(0, 255, 255);");
}

void TcpTestWidget::slot_newConnection()
{
    qDebug() << "onNewConnection";
    m_timer->setInterval(ui->spinBox->value());
    on_pushButton_7_clicked();
    on_pushButton_8_clicked();
    m_connectIndex = 0;
    m_autoSendIndex = 0;
    if(!m_tcpSocket){
        qDebug() << "onNewConnection m_tcpSocket";
        m_tcpSocket = m_tcpServer.nextPendingConnection();     //获取新的客户端信息
        QString info= m_tcpSocket->peerAddress().toString()+":"+QString("%1").arg(m_tcpSocket->peerPort());
        ui->label->setText(info);
        m_tcpSocket->setObjectName(info);       //设置名称,方便查找
        connect(m_tcpSocket, &QTcpSocket::connected, this, &TcpTestWidget::onServerConnected);
        connect(m_tcpSocket, &QTcpSocket::disconnected, this, &TcpTestWidget::onServerDisconnected, Qt::QueuedConnection);
        connect(m_tcpSocket, &QTcpSocket::readyRead, this, &TcpTestWidget::onServerDataReady);
        connect(m_tcpSocket, &QTcpSocket::bytesWritten, this, &TcpTestWidget::onServerBytesWritten);
    }
    else{
        ui->label_4->setText("已有一个客户端连入");
    }
}


void TcpTestWidget::onServerConnected()
{
    qDebug() << "onServerConnected";
    if(!m_isServer){
        ui->pushButton->setStyleSheet("background-color: rgb(0, 255, 255);");
        ui->pushButton_2->setStyleSheet("background-color: rgb(255, 255, 0);");
    }
    m_connectIndex = 0;
    m_autoSendIndex = 0;
}

void TcpTestWidget::onServerDisconnected()
{
    qDebug() << "onServerDisconnected";
    m_connectIndex = 0;
    m_autoSendIndex = 0;
    ui->label->setText("");
    if(m_tcpSocket){
        m_tcpSocket->disconnect();
        delete m_tcpSocket;
        m_tcpSocket = nullptr;
    }
}

void TcpTestWidget::onServerDataReady()
{
    m_recvInfo.append(m_tcpSocket->readAll().toHex());
    if(!m_timer->isActive()){
        m_timer->start();
    }
}

void TcpTestWidget::onServerBytesWritten()
{

}

void TcpTestWidget::slot_sendInfo()
{
    if(!m_tcpSocket){
        return;
    }

    m_timer->stop();
    ui->textEdit_4->append("recv: " + m_recvInfo);
    m_recvInfo.clear();
    QString s_wData = "";
    if(m_connectList.size() > m_connectIndex)
    {
        s_wData = m_connectList.at(m_connectIndex);
        m_connectIndex++;
    }
    else{
        if(m_autoSendList.size() > 0){
            if(m_autoSendList.size() > m_autoSendIndex){
                s_wData = m_autoSendList.at(m_autoSendIndex);
            }
            else{
                m_autoSendIndex = 0;
                s_wData = m_autoSendList.at(m_autoSendIndex);
            }
            m_autoSendIndex++;
        }
    }
    if(!s_wData.isEmpty()){
        ui->textEdit_4->append("send: " + s_wData);
        QByteArray sendData = QByteArray::fromHex(s_wData.toLatin1());
        if(m_tcpSocket)
            m_tcpSocket->write(sendData);
    }
}

void TcpTestWidget::on_pushButton_7_clicked()
{
    m_connectList.clear();
    QString text = ui->textEdit_5->toPlainText();
    if(!text.isEmpty()){
        m_connectList = text.split("\n");
    }
    qDebug() << "connect list: " << m_connectList.size();
}

void TcpTestWidget::on_pushButton_8_clicked()
{
    m_autoSendList.clear();
    QString text = ui->textEdit_6->toPlainText();
    if(!text.isEmpty()){
        m_autoSendList = text.split("\n");
    }
    qDebug() << "auto send list: " << m_autoSendList.size();
}

void TcpTestWidget::on_pushButton_6_clicked()
{
    ui->textEdit_4->clear();
}

void TcpTestWidget::on_comboBox_currentIndexChanged(int index)
{
    if(0 == index){
        ui->label_5->setHidden(true);
        ui->lineEdit->setHidden(true);
        m_isServer = true;
    }
    else{
        ui->label_5->setHidden(false);
        ui->lineEdit->setHidden(false);
        m_isServer = false;
    }
    m_tcpServer.close();
    if(m_tcpSocket){
        ui->label->setText("");
        m_tcpSocket->disconnect();
        delete m_tcpSocket;
        m_tcpSocket = nullptr;
    }
}

void TcpTestWidget::on_pushButton_3_clicked()
{
    QString s_wData = "";
    m_autoSendIndex = 0;
    if(m_autoSendList.size() > 0){
        s_wData = m_autoSendList.at(m_autoSendIndex);
    }
    m_autoSendIndex++;
    if(!s_wData.isEmpty()){
        ui->textEdit_4->append("send: " + s_wData);
        QByteArray sendData = QByteArray::fromHex(s_wData.toLatin1());
        qDebug() << "send data size: " << m_tcpSocket->write(sendData);
    }
}
