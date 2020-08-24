#ifndef PROTOCOLDEFINE_H
#define PROTOCOLDEFINE_H

#include <QSerialPort>

class CSerialPortConfig
{
public:
    CSerialPortConfig(){
        m_nBaudRate = 9600;
        m_portName = "";
        m_eDataBit = QSerialPort::Data8;
        m_eParity = QSerialPort::NoParity;
        m_eflowControl = QSerialPort::NoFlowControl;
        m_eStopBits = QSerialPort::OneStop;
    }

    ~CSerialPortConfig(){}

    int m_nBaudRate;
    QString m_portName;
    QSerialPort::DataBits m_eDataBit;
    QSerialPort::Parity m_eParity;
    QSerialPort::FlowControl m_eflowControl;
    QSerialPort::StopBits m_eStopBits;
};


class ProtocolDefine
{
public:
    ProtocolDefine();
};

#endif // PROTOCOLDEFINE_H
