#include "camerauye.h"

#include <QDebug>


cameraUye::cameraUye(QObject *parent) : QObject(parent)
{
    m_flagAesEnabled = false;
}

int cameraUye::initCamera(int camId)
{
    m_hCam = camId;

    int nRet = is_InitCamera(&m_hCam, NULL);
    qDebug("%s - is_InitCamera, handle: %d, returned: %d", Q_FUNC_INFO, m_hCam, nRet);

    if(nRet != IS_SUCCESS)
    {
        return -1;
    }

    m_imgWidth=2048, m_imgHeight=1536, m_imgBpp=8;
    //m_imgWidth=800 , m_imgHeight=600, m_imgBpp=8;

    is_AllocImageMem(m_hCam, m_imgWidth, m_imgHeight, m_imgBpp, &m_imgMem, &m_memId);
    is_SetImageMem (m_hCam, m_imgMem, m_memId);
    is_SetDisplayMode (m_hCam, IS_SET_DM_DIB);
    is_SetColorMode (m_hCam, IS_CM_MONO8);
    //is_SetImageSize (m_hCam, m_imgWidth, m_imgHeight);


    double enable = 1;
    double disable = 0;
    is_SetAutoParameter (m_hCam, IS_SET_ENABLE_AUTO_GAIN, &enable, 0);
    is_SetAutoParameter (m_hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &enable, 0);
    is_SetAutoParameter (m_hCam, IS_SET_ENABLE_AUTO_FRAMERATE, &disable, 0);
    is_SetAutoParameter (m_hCam, IS_SET_ENABLE_AUTO_SHUTTER, &disable, 0);
    is_SetAutoParameter (m_hCam, IS_SET_ENABLE_AUTO_SENSOR_GAIN, &enable, 0);
    is_SetAutoParameter (m_hCam, IS_SET_ENABLE_AUTO_SENSOR_WHITEBALANCE,&enable,0);
    is_SetAutoParameter (m_hCam, IS_SET_ENABLE_AUTO_SENSOR_SHUTTER, &disable, 0);

    double FPS,NEWFPS;
    FPS = 4;
    double fps;
    is_SetFrameRate(m_hCam,FPS,&NEWFPS);

    double parameter = 80;
    int error = is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*) &parameter, sizeof(parameter));
    if (error != IS_SUCCESS) {
        qDebug() << "): failed" <<parameter;
    }

 return 0;
}

int cameraUye::captureImage(Mat *returnImg)
{
    if(is_FreezeVideo(m_hCam, IS_WAIT) == IS_SUCCESS)
    {
        void *pMemVoid; //pointer to where the image is stored
        is_GetImageMem (m_hCam, &pMemVoid);


        Mat image(Size(m_imgWidth, m_imgHeight), CV_8UC1, pMemVoid, Mat::AUTO_STEP);

      //  namedWindow( "Display windowTest", WINDOW_NORMAL); // Create a window for display.
      //  imshow( "Display windowTest", image); // Show our image inside it.

        *returnImg = image;

        /*
        IplImage * img;
        img=cvCreateImage(cvSize(m_imgWidth, m_imgHeight), IPL_DEPTH_8U, 1);
        img->nSize=112;
        img->ID=0;
        img->nChannels=1;
        img->alphaChannel=0;
        img->depth=8;
        img->dataOrder=0;
        img->origin=0;
        img->align=4;
        img->width=m_imgWidth;
        img->height=m_imgHeight;
        img->roi=NULL;
        img->maskROI=NULL;
        img->imageId=NULL;
        img->tileInfo=NULL;
        img->imageSize=m_imgWidth*m_imgHeight;
        img->imageData=(char*)pMemVoid;  //the pointer to imagaData
        img->widthStep=m_imgWidth;
        img->imageDataOrigin=(char*)pMemVoid; //and again
        //now you can use your img just like a normal OpenCV image
        //cvNamedWindow( "A", 1 );
        //cvShowImage("A",img);

        returnImg = img;
        */

        //check AES status
        if(m_flagAesEnabled == true)
        {
            double aesActive;
            is_SetAutoParameter (m_hCam, IS_GET_AUTO_BRIGHTNESS_ONCE, &aesActive, 0);

            if(aesActive == 0)
            {
                //aes is finished
                m_flagAesEnabled = false;
                emit signalAesFinished();
            }
        }
        return 0;
    }
    else
    {
        return -1;
    }
}

void cameraUye::setAes(double brightness)
{
    double dEnable = 1;
    int nRetCode = is_SetAutoParameter (m_hCam, IS_SET_ENABLE_AUTO_SHUTTER, &dEnable, 0);
    qDebug() << "activate auto shutter returns: " << nRetCode;

    nRetCode = is_SetAutoParameter (m_hCam, IS_SET_AUTO_REFERENCE, &brightness, 0);
    qDebug() << "set auto shutter reference returns: " << nRetCode;

    nRetCode = is_SetAutoParameter (m_hCam, IS_SET_AUTO_BRIGHTNESS_ONCE, &dEnable, 0);
    qDebug() << "set auto shutter reference returns: " << nRetCode;

    m_flagAesEnabled = true;

}


