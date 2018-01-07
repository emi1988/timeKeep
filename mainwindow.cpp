#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

//#include <zbar.h>
//#include<iostream>

//using namespace std;
//using namespace zbar;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    m_imageTimer = new QTimer(this);
    connect(m_imageTimer, SIGNAL(timeout()), this, SLOT(onImageTimer()));

    m_scanner = new ImageScanner();

    m_scanner->set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);
    m_camera.initCamera(1);

}

void MainWindow::onImageTimer()
{
    Mat image;
    m_camera.captureImage(&image);
    imshow( "Display window", image);

    // Obtain image data
    int width = image.cols;
    int height = image.rows;
    uchar *raw = (uchar *)(image.data);

    // Wrap image data
    Image imageZbar(width, height, "Y800", raw, width * height);

    m_scanner->scan(imageZbar);

    int n = m_scanner->scan(imageZbar);

    qDebug() << "Anzahl gefundene Codes: " << n;

    // extract results
    for(Image::SymbolIterator symbol = imageZbar.symbol_begin();
        symbol != imageZbar.symbol_end();
        ++symbol)
    {
        QString type = QString::fromStdString(symbol->get_type_name());
        QString data = QString::fromStdString(symbol->get_data());

        qDebug() << "decoded " << type;
        qDebug() << " symbol \"" << data << '"' << endl;

        ui->textEditDecoded->setText(data);
    }

    // clean up
    imageZbar.set_data(NULL, 0);

    //QString readString = m_codeReaderObject->readCode(image);
    //qDebug() << "read string:" << readString;

}

void MainWindow::onAesTimer()
{
    Mat image;

    if(m_camera.captureImage(&image) != 0)
    {
        return;
    }
    namedWindow("Display window", WINDOW_NORMAL);
    resizeWindow("Display window", 200,200);
    imshow("Display window", image);

    Scalar tempVal = mean(image);
    float mean = tempVal.val[0];
    qDebug() << "avearge of image: " << mean;
    ui->textEditDecoded->setText("avearge of image: " + QString::number(mean));

    if(flagAesFinsihed == true)
    {
        destroyWindow("Display window");
    }
}

void MainWindow::onAesFinished()
{
    flagAesFinsihed = true;

    qDebug() << "AES finished signal received";
    m_aesTimer->stop();
    ui->pushButtonAes->setEnabled(true);

    destroyWindow("Display window");
}


void MainWindow::on_pushButtonStartStop_clicked()
{
    if(m_imageTimer->isActive() == true)
    {
        m_imageTimer->stop();

        ui->pushButtonStartStop->setText("start reading");
    }
    else
    {
        m_imageTimer->start(1000);

        ui->pushButtonStartStop->setText("stop reading");
    }
}

void MainWindow::on_pushButtonAes_clicked()
{
    flagAesFinsihed = false;
    m_camera.setAes(170);
    connect(&m_camera, SIGNAL(signalAesFinished()), this, SLOT(onAesFinished()));

    m_aesTimer = new QTimer(this);
    connect(m_aesTimer, SIGNAL(timeout()), this, SLOT(onAesTimer()));

    m_aesTimer->start(200);

    ui->pushButtonAes->setEnabled(false);
}
