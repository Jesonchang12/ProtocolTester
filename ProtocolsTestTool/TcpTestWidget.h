#ifndef TCPTESTWIDGET_H
#define TCPTESTWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QTimer>

namespace Ui {
class TcpTestWidget;
}

class TcpTestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TcpTestWidget(QWidget *parent = 0);
    ~TcpTestWidget();

private slots:
    void on_pushButton_clicked();

    void slot_newConnection();
    void on_pushButton_2_clicked();

    void onServerConnected();
    void onServerDisconnected();
    void onServerDataReady();
    void onServerBytesWritten();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();

    void slot_sendInfo();
    void on_pushButton_6_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_3_clicked();

private:
    QTcpServer m_tcpServer;
    QTcpSocket* m_tcpSocket;
    QTimer* m_timer;
    QString m_recvInfo;

    QStringList m_connectList;
    QStringList m_autoSendList;
    int m_connectIndex;
    int m_autoSendIndex;

    bool m_isServer;
private:
    Ui::TcpTestWidget *ui;
};

#endif // TCPTESTWIDGET_H
