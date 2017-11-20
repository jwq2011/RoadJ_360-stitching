#ifndef V4L2_WORK_FOURIN_H
#define V4L2_WORK_FOURIN_H

#include <QThread>
#include <QString>
#include  <QObject>
#include "g2d_yuyv_bgra.h"
#include "g2d.h"
#include <linux/videodev2.h>


class CameraWorkFourin : public QThread
{
#define BUILD_FOR_ANDROID 1


#define G2D_CACHEABLE    0

#define TFAIL -1
#define TPASS 0

#define NUMBER_BUFFERS    4
#define NUMBER_CAMERAS  4

    Q_OBJECT

    struct buffer {
        void * start;
        size_t length;
    };

    struct testbuffer
    {
            unsigned char *start;
            size_t offset;
            unsigned int length;
    };

    typedef struct __CameraStruct
    {
        char dev_name[20];
        int fd;
        struct testbuffer  buf[NUMBER_BUFFERS];
        unsigned int n_buffers;
        unsigned char * data;
    }CameraStruct,* PCameraStruct;

    typedef struct __FourCameraStruct
    {
        CameraStruct topCamera;
        CameraStruct bottomCamera;
        CameraStruct leftCamera;
        CameraStruct rightCamera;
    }FourCameraStruct, *PFourCameraStruct;

public:
    CameraWorkFourin(QObject *parent = Q_NULLPTR);
    ~CameraWorkFourin();

    void setCameraName(CameraStruct *camera,QString name);
    int video_init();
    void setWhichCamera(int theCamera);
    unsigned char * setCameraData(unsigned char **data);

private:
    int init(CameraStruct *camera,int theCamera);
    int start_capturing(CameraStruct *camera);
    int  v4l_capture_setup(CameraStruct *camera);
    void uninit(CameraStruct *camera,int theCamera);
    int cameraRun(CameraStruct *camera,int theCamera);
    int read_frame(CameraStruct *camera,int theCamera);
    int prepare_g2d_buffers(CameraStruct *camera,int theCamera);

private:
    void run();

signals: void Frame(unsigned int);

public:
    static const unsigned int bufferCount = 4;
    int countCamera = 4;
    struct g2d_buf *g_g2d_src_buffer[NUMBER_BUFFERS];
    struct g2d_buf *g_g2d_dst_buffer[NUMBER_BUFFERS];
    struct g2d_buf *g_g2d_test_buffer[NUMBER_BUFFERS];
    unsigned char * startData = NULL;

private:

    QString cameraName;
    FourCameraStruct fourCamera;
    const bool analog = true;
    const unsigned int count = 4;
    const unsigned int width = 1280; //宽，必须是16的倍数
    const unsigned int height = 800; //高，必须是16的倍数
    unsigned int offsetCount = 0;

    int theCamera;


    char v4l_capture_dev[100];
    char save_file[100];
    int fd_capture_v4l = 0;
    int fd_fb_display = 0;
    int fd_ipu = 0;
    unsigned char * g_fb_display = NULL;
    int g_input = 1;
    int g_display_num_buffers = 3;
    int g_capture_num_buffers = NUMBER_BUFFERS;
    int g_in_width = 0;
    int g_in_height = 0;
    int g_in_fmt = V4L2_PIX_FMT_YUYV;
    int g_display_width = 1200;
    int g_display_height = 720;
    int g_display_top = 0;
    int g_display_left = 0;
    int g_display_fmt = V4L2_PIX_FMT_BGR32;
    int g_display_base_phy;
    int g_display_size;
    int g_display_fg = 1;
    int g_display_id = 1;
    //struct fb_var_screeninfo g_screen_info;
    //int g_frame_count = 0x7FFFFFFF;
    int g_frame_count = 0x1;
    int g_frame_size;
    bool g_g2d_render = 1;
    int g_g2d_fmt;
//    int g_mem_type = V4L2_MEMORY_MMAP;
    int g_mem_type = V4L2_MEMORY_USERPTR;
    struct testbuffer display_buffers[3];
    struct testbuffer capture_buffers[NUMBER_BUFFERS];

    unsigned long file_length;

    bool initG2DBufferStatus = false;
};

#endif // V4L2_WORK_FOURIN_H
