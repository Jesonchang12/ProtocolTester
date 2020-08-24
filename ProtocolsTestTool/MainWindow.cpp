#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_serialWidget = new SerialTestWidget();
    ui->verticalLayout->addWidget(m_serialWidget);

    m_tcpWidget = new TcpTestWidget();
    ui->verticalLayout->addWidget(m_tcpWidget);
    m_tcpWidget->setHidden(true);

    m_udpWidget = new UdpTestWidget();
    ui->verticalLayout->addWidget(m_udpWidget);
    m_udpWidget->setHidden(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index == 0){
        m_serialWidget->setHidden(false);
        m_tcpWidget->setHidden(true);
        m_udpWidget->setHidden(true);
    }
    else if(index == 1){
        m_serialWidget->setHidden(true);
        m_tcpWidget->setHidden(false);
        m_udpWidget->setHidden(true);
    }
    else{
        m_serialWidget->setHidden(true);
        m_tcpWidget->setHidden(true);
        m_udpWidget->setHidden(false);
    }
}
