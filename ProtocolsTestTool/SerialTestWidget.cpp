#include "SerialTestWidget.h"
#include "ui_SerialTestWidget.h"
#include <QTimer>
#include <QSerialPortInfo>
#include <QDebug>

SerialTestWidget::SerialTestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialTestWidget)
{
    ui->setupUi(this);
    m_recvInfo.clear();
    QStringList serialNameList;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite)) {
            serial.close();
            serialNameList.append(info.portName());
        } else{
            qDebug() << info.portName() << " serial port has been occupied.";
        }
    }
    ui->comboBox->addItems(serialNameList);

    m_serialPort = new QSerialPort(this);
    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialTestWidget::slot_readyRead);

    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, &SerialTestWidget::slot_sendInfo);
}

SerialTestWidget::~SerialTestWidget()
{
    delete ui;
}

void SerialTestWidget::on_pushButton_2_clicked()
{
    m_timer->setInterval(ui->lineEdit->text().toInt());
    on_pushButton_clicked();
    on_pushButton_4_clicked();
    m_connectIndex = 0;
    m_autoSendIndex = 0;
    CSerialPortConfig serialInfo;
    setSerialConfig(serialInfo);
    if(!m_serialPort->isOpen()){
        m_serialPort->setPortName(ui->comboBox->currentText());
        if(m_serialPort->open(QIODevice::ReadWrite))
        {
            ui->pushButton_2->setStyleSheet("background-color: rgb(0, 255, 255);");
            ui->pushButton_3->setStyleSheet("background-color: rgb(255, 255, 0);");
            m_serialPort->setBaudRate(serialInfo.m_nBaudRate);
            m_serialPort->setDataBits(serialInfo.m_eDataBit);
            m_serialPort->setParity(serialInfo.m_eParity);
            m_serialPort->setFlowControl(serialInfo.m_eflowControl);
            m_serialPort->setStopBits(serialInfo.m_eStopBits);
        }
        else
        {
            ui->pushButton_2->setStyleSheet("background-color: rgb(255, 255, 0);");
            ui->pushButton_3->setStyleSheet("background-color: rgb(0, 255, 255);");
            qDebug() << "提示: 串口没有打开!!!";
        }
    } else{
        qDebug() << "提示: 串口已被占用!!!";
    }
}

void SerialTestWidget::slot_readyRead()
{
    m_recvInfo.append(m_serialPort->readAll().toHex());
    if(!m_timer->isActive()){
        m_timer->start();
    }
}

void SerialTestWidget::slot_sendInfo()
{
    m_timer->stop();
    ui->textEdit_2->append("recv: " + m_recvInfo);
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
        ui->textEdit_2->append("send: " + s_wData);
        QByteArray sendData = QByteArray::fromHex(s_wData.toLatin1());
        qDebug() << "send data size: " << m_serialPort->write(sendData);
    }
}

bool SerialTestWidget::setSerialConfig(CSerialPortConfig &serialConfig)
{
    serialConfig.m_portName = ui->comboBox->currentText();
    serialConfig.m_nBaudRate = ui->comboBox_2->currentText().toInt();
    int parityIndex = ui->comboBox_3->currentIndex();
    switch (parityIndex) {
    case 0:
        serialConfig.m_eParity = QSerialPort::NoParity;
        break;
    case 1:
        serialConfig.m_eParity = QSerialPort::EvenParity;
        break;
    case 2:
        serialConfig.m_eParity = QSerialPort::OddParity;
        break;
    case 3:
        serialConfig.m_eParity = QSerialPort::SpaceParity;
        break;
    case 4:
        serialConfig.m_eParity = QSerialPort::MarkParity;
        break;
    case 5:
        serialConfig.m_eParity = QSerialPort::UnknownParity;
        break;
    default:
        break;
    }
    serialConfig.m_eDataBit = (QSerialPort::DataBits)ui->comboBox_3->currentText().toInt();
    serialConfig.m_eStopBits = (QSerialPort::StopBits)ui->comboBox_5->currentText().toInt();
    int flowIndex = ui->comboBox_6->currentIndex();
    switch (flowIndex) {
    case 0:
        serialConfig.m_eflowControl = QSerialPort::NoFlowControl;
        break;
    case 1:
        serialConfig.m_eflowControl = QSerialPort::HardwareControl;
        break;
    case 2:
        serialConfig.m_eflowControl = QSerialPort::SoftwareControl;
        break;
    case 3:
        serialConfig.m_eflowControl = QSerialPort::UnknownFlowControl;
        break;
    default:
        break;
    }
    return true;
}

void SerialTestWidget::on_pushButton_clicked()
{
    m_connectList.clear();
    QString text = ui->textEdit->toPlainText();
    if(!text.isEmpty()){
        m_connectList = text.split("\n");
    }
    qDebug() << "connect list: " << m_connectList.size();
}

void SerialTestWidget::on_pushButton_4_clicked()
{
    m_autoSendList.clear();
    QString text = ui->textEdit_3->toPlainText();
    if(!text.isEmpty()){
        m_autoSendList = text.split("\n");
    }
    qDebug() << "auto send list: " << m_autoSendList;
}

void SerialTestWidget::on_pushButton_3_clicked()
{
    m_serialPort->close();
    ui->pushButton_2->setStyleSheet("background-color: rgb(255, 255, 0);");
    ui->pushButton_3->setStyleSheet("background-color: rgb(0, 255, 255);");
}

void SerialTestWidget::on_pushButton_5_clicked()
{
    ui->textEdit_2->clear();
}

void SerialTestWidget::on_pushButton_6_clicked()
{
    QString s_wData = "";
    m_autoSendIndex = 0;
    if(m_autoSendList.size() > 0){
        s_wData = m_autoSendList.at(m_autoSendIndex);
    }
    m_autoSendIndex++;
    if(!s_wData.isEmpty()){
        ui->textEdit_2->append("send: " + s_wData);
        QByteArray sendData = QByteArray::fromHex(s_wData.toLatin1());
        qDebug() << "send data size: " << m_serialPort->write(sendData);
    }
}
