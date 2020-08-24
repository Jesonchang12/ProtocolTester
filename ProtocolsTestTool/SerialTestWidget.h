#ifndef SERIALTESTWIDGET_H
#define SERIALTESTWIDGET_H

#include <QWidget>
#include <ProtocolDefine.h>
#include <QSerialPort>

namespace Ui {
class SerialTestWidget;
}

class SerialTestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SerialTestWidget(QWidget *parent = 0);
    ~SerialTestWidget();

private slots:
    void on_pushButton_2_clicked();
    void slot_readyRead();
    void on_pushButton_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();

    void slot_sendInfo();
    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    bool setSerialConfig(CSerialPortConfig& serianConfig);
    QSerialPort* m_serialPort;
    QStringList m_connectList;
    QStringList m_autoSendList;
    int m_connectIndex;
    int m_autoSendIndex;

    QTimer* m_timer;
    QString m_recvInfo;

private:
    Ui::SerialTestWidget *ui;
};

#endif // SERIALTESTWIDGET_H
