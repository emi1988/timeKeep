#ifndef CAMERAUYE_H
#define CAMERAUYE_H

#include <QObject>

#include "uEye.h"

#ifndef __OPENCV_CORE_HPP__
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#endif

using namespace cv;


class cameraUye : public QObject
{
    Q_OBJECT
public:
    explicit cameraUye(QObject *parent = 0);

    int initCamera(int camId);
    int captureImage(Mat *returnImg);
    void setAes(double brightness);

signals:
    void signalAesFinished();

private:
    HIDS m_hCam;

    int m_imgWidth, m_imgHeight, m_imgBpp;

    char* m_imgMem;

    int m_memId;

    bool m_flagAesEnabled;

public slots:
};

#endif // CAMERAUYE_H
