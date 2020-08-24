#ifndef UDPTESTWIDGET_H
#define UDPTESTWIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QTimer>

namespace Ui {
class UdpTestWidget;
}

class UdpTestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UdpTestWidget(QWidget *parent = 0);
    ~UdpTestWidget();

private slots:
    void slot_readyRead();
    void slot_sendInfo();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::UdpTestWidget *ui;

    QUdpSocket* m_udpServer;
    QStringList m_connectList;
    QStringList m_autoSendList;
    int m_connectIndex;
    int m_autoSendIndex;

    QTimer* m_timer;
    QString m_recvInfo;
    QHostAddress sender;
    quint16 senderPort;
};

#endif // UDPTESTWIDGET_H
