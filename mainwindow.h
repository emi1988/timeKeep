#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "camerauye.h"
#include <zbar.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace zbar;

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
    void onImageTimer();
    void onAesTimer();
    void onAesFinished();
    void on_pushButtonStartStop_clicked();

    void on_pushButtonAes_clicked();

private:
    Ui::MainWindow *ui;

    cameraUye m_camera;
    ImageScanner* m_scanner;

    QTimer *m_imageTimer;
    QTimer *m_aesTimer;

    bool flagAesFinsihed;

    void init();
};

#endif // MAINWINDOW_H
