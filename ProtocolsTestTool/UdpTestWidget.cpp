#include "UdpTestWidget.h"
#include "ui_UdpTestWidget.h"

UdpTestWidget::UdpTestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UdpTestWidget)
{
    m_udpServer = new QUdpSocket();
    ui->setupUi(this);
    connect(m_udpServer, &QUdpSocket::readyRead,this,&UdpTestWidget::slot_readyRead);

    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, &UdpTestWidget::slot_sendInfo);
}

UdpTestWidget::~UdpTestWidget()
{
    delete ui;
}

void UdpTestWidget::slot_readyRead()
{
    while (m_udpServer->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpServer->pendingDatagramSize());

        m_udpServer->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);
        m_recvInfo.append(datagram.toHex());
        if(!m_timer->isActive()){
            m_timer->start();
        }
    }
}

void UdpTestWidget::slot_sendInfo()
{
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
        qDebug() << "send data size: " << m_udpServer->writeDatagram(sendData, sender, senderPort);;
    }
}

void UdpTestWidget::on_pushButton_clicked()
{
    on_pushButton_7_clicked();
    on_pushButton_8_clicked();
    if(m_udpServer->bind(QHostAddress::AnyIPv4, ui->lineEdit_2->text().toInt())){
        ui->pushButton->setStyleSheet("background-color: rgb(0, 255, 255);");
        ui->pushButton_2->setStyleSheet("background-color: rgb(255, 255, 0);");
    }
    else{
        ui->pushButton->setStyleSheet("background-color: rgb(255, 255, 0);");
        ui->pushButton_2->setStyleSheet("background-color: rgb(0, 255, 255);");
    }
}

void UdpTestWidget::on_pushButton_2_clicked()
{
    m_udpServer->close();
    ui->pushButton->setStyleSheet("background-color: rgb(255, 255, 0);");
    ui->pushButton_2->setStyleSheet("background-color: rgb(0, 255, 255);");
}

void UdpTestWidget::on_pushButton_7_clicked()
{
    m_connectList.clear();
    QString text = ui->textEdit_5->toPlainText();
    if(!text.isEmpty()){
        m_connectList = text.split("\n");
    }
    qDebug() << "connect list: " << m_connectList.size();
}

void UdpTestWidget::on_pushButton_8_clicked()
{
    m_autoSendList.clear();
    QString text = ui->textEdit_6->toPlainText();
    if(!text.isEmpty()){
        m_autoSendList = text.split("\n");
    }
    qDebug() << "auto send list: " << m_autoSendList.size();
}

void UdpTestWidget::on_pushButton_6_clicked()
{
    ui->textEdit_4->clear();
}
