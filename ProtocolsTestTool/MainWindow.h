#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SerialTestWidget.h"
#include "TcpTestWidget.h"
#include "UdpTestWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_comboBox_currentIndexChanged(int index);

private:
    SerialTestWidget* m_serialWidget;
    TcpTestWidget* m_tcpWidget;
    UdpTestWidget* m_udpWidget;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
