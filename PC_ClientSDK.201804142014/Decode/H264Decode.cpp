#include "stdafx.h"
#include "H264Decode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TransferTask.h"
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#include <objbase.h>
#define INBUF_SIZE 4096
#define OUTBUF_SIZE 10 * 1024 * 1024
#define GUID_LEN 64

typedef unsigned char BYTE;

typedef struct RGB32 {
    BYTE    rgbBlue;      // 蓝色分量
    BYTE    rgbGreen;     // 绿色分量
    BYTE    rgbRed;       // 红色分量
    BYTE    rgbReserved;  // 保留字节（用作Alpha通道或忽略）
} RGB32;

#define COLORSIZE 255


class ColorTable
{
public:
    ColorTable()
    {
        for (int i = 0; i < COLORSIZE; i++)
        {
            ///R = Y + 1.402 * (V-128);
            //R_Y[i] = 0; //没有
            //R_U[i] = 0; //没有
            R_V[i] = 1.402 * (i - 128);

            ///G = Y - 0.34413 * (U-128) - 0.71414*(V-128);
            //G_Y[i] = 0;
            G_U[i] = 0.34413 * (i - 128);
            G_V[i] = 0.71414 * (i - 128);

            /// = Y + 1.772*(U-128);
            //B_Y[i] = 0;
            B_U[i] = 1.772 * (i - 128);
            //B_V[i] = 0;
        }
    }

public:
    unsigned short R_Y[COLORSIZE], R_U[COLORSIZE], R_V[COLORSIZE];
    unsigned short G_Y[COLORSIZE], G_U[COLORSIZE], G_V[COLORSIZE];
    unsigned short B_Y[COLORSIZE], B_U[COLORSIZE], B_V[COLORSIZE];
};

ColorTable table;


static std::string generate()
{
    char buf[GUID_LEN] = { 0 };
    GUID guid;

    if (CoCreateGuid(&guid))
    {
        return std::move(std::string(""));
    }

    sprintf_s(buf,
        "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2],
        guid.Data4[3], guid.Data4[4], guid.Data4[5],
        guid.Data4[6], guid.Data4[7]);

    return std::move(std::string(buf));
}

void SaveBuf(const unsigned char* pData, int len, FILE* pFile)
{
    if (pFile)
    {
        fwrite(pData, len, 1, pFile);
    }
}

void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
    const char *filename)
{
    FILE *f;
    int i;

    fopen_s(&f, filename, "a");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    for (i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}

std::string strTmpPath = "C:\\Users\\xiaohwa2\\AppData\\Local\\Temp\\PPCS_Client\\Pic\\";
std::string strUID = generate();


void H264Decode::DataCallBack(const unsigned char* pData, int width, int height, int len)
{
    if (m_pDataCallBack)
    {
        m_pDataCallBack(m_strUserCode.c_str(), pData, width, height, len);
    }
}

void H264Decode::CreateRecordFile(const std::string& strTmpDataFile)
{
    if (!m_pRecordFile)
    {
        m_strCurRecordFile = strTmpDataFile;
        fopen_s(&m_pRecordFile, m_strCurRecordFile.c_str(), "wb");
    }
}

void H264Decode::CloseRecordFile()
{
    if (m_pRecordFile)
    {
        fflush(m_pRecordFile);
        fclose(m_pRecordFile);
        m_pRecordFile = nullptr;
    }
}

void H264Decode::WriteRecordFile(unsigned char*data, int len)
{
    if (m_pRecordFile)
    {
        SaveBuf(data, len, m_pRecordFile);
    }
}

void H264Decode::OnTransferTask(TransferTask::TransferStatu emStatu, const std::string & strInfo)
{
    if (m_pDataStatuCallBack)
    {
        switch (emStatu)
        {
        case TransferTask::Start:
            m_pDataStatuCallBack(m_strUserCode.c_str(), DecodeStatus::Start_Transfer, (void *)strInfo.c_str());
            break;
        case TransferTask::End_Succeed:
            m_pDataStatuCallBack(m_strUserCode.c_str(), DecodeStatus::Complete_Transfer, (void *)strInfo.c_str());
            break;
        case TransferTask::End_Failed:
            m_pDataStatuCallBack(m_strUserCode.c_str(), DecodeStatus::Complete_Transfer, (void *)strInfo.c_str());
            break;
        default:
            break;
        }
    }
}

H264Decode::H264Decode()
{
}


H264Decode::~H264Decode()
{
    Destory();
}

bool H264Decode::Init(pfnDecodeCallBack pfnCallBack)
{
    if (m_init)
        return TRUE;

    m_pDataCallBack = pfnCallBack;
    m_init = TRUE;

    m_pCodecContext = avcodec_alloc_context3(NULL);
    m_pH264VideoDecoder = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (m_pH264VideoDecoder == NULL)
    {
        return FALSE;
    }

    m_pParser = av_parser_init(m_pH264VideoDecoder->id);
    if (m_pParser == NULL)
    {
        return FALSE;
    }

    //初始化参数，下面的参数应该由具体的业务决定  AV_PIX_FMT_YUV420P;
    m_pCodecContext->time_base.num = 1;
    m_pCodecContext->frame_number = 1; //每包一个视频帧  
    m_pCodecContext->codec_type = AVMEDIA_TYPE_VIDEO;
    m_pCodecContext->bit_rate = 0;
    m_pCodecContext->time_base.den = 25;//帧率  
    m_pCodecContext->width = 0;//视频宽  
    m_pCodecContext->height = 0;//视频高 
    m_pCodecContext->pix_fmt = AV_PIX_FMT_YUVJ420P;
    m_pCodecContext->color_range = AVCOL_RANGE_JPEG;

    if (avcodec_open2(m_pCodecContext, m_pH264VideoDecoder, NULL) >= 0)
        m_pFrameYUV = av_frame_alloc();
    else
        return FALSE;

    ///for YUV420 to RGB
    const int maxWidth = 1920;
    const int maxHeight = 1080;
    m_pFrameRGB = av_frame_alloc();
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB32, maxWidth, maxHeight, 1);

    m_pRGBBuffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    int yuvSize = maxWidth * maxHeight * 3 / 2;

    m_pImgConvertCtx = sws_getContext(maxWidth, maxHeight, AV_PIX_FMT_YUVJ420P, \
        maxWidth, maxHeight, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
    ///end for YUV420 to RGB
    m_thDecode = std::thread([this]() {
        this->RunDecodeThread();
    });

    return TRUE;
}

bool H264Decode::Destory()
{
    if (m_init)
    {
        CloseRecordFile();

        m_bQuit.store(true);
        if (m_thDecode.joinable())
        {
            m_thDecode.join();
        }

        if (m_pParser)
        {
            av_parser_close(m_pParser);
            m_pParser = nullptr;
        }
        if (m_pCodecContext)
        {
            avcodec_free_context(&m_pCodecContext);
            m_pCodecContext = nullptr;
        }
        if (m_pFrameYUV)
        {
            av_frame_free(&m_pFrameYUV);
            m_pFrameYUV = nullptr;
        }

        if (m_pFrameRGB)
        {
            av_frame_free(&m_pFrameRGB);
            m_pFrameRGB = nullptr;
        }
        if (m_pRGBBuffer)
        {
            av_free(m_pRGBBuffer);
            m_pRGBBuffer = nullptr;
        }

        if (m_pImgConvertCtx)
        {
            sws_freeContext(m_pImgConvertCtx);
            m_pImgConvertCtx = nullptr;
        }

        while (m_queTransferTasks.size())
        {
            if (auto pTask = m_queTransferTasks.front())
            {
                pTask->EndTask();
            }
            m_queTransferTasks.pop();
        }
        m_init = false;
    }
    return true;
}


void H264Decode::InputData(unsigned char*data, int data_size)
{
    if (!m_init)
    {
        return;
    }

    WriteRecordFile(data, data_size);

    AVPacket packet = { 0 };
    packet.data = (uint8_t*)data;    //这里填入一个指向完整H264数据帧的指针  
    packet.size = data_size;        //这个填入H264数据帧的大小 
    {
        std::lock_guard<std::mutex> guard(m_mxLockFrame);
        avcodec_send_packet(m_pCodecContext, &packet);
    }
    m_covDataCome.notify_all();
}

long H264Decode::GetHandle()
{
    return (long)this;
}

void H264Decode::RunDecodeThread()
{
    std::mutex mxTmp;
    std::unique_lock<std::mutex> lock(mxTmp);
    while (true)
    {
        auto waitRet = m_covDataCome.wait_for(lock, std::chrono::milliseconds(1000));
        while (true)
        {
            if (m_bQuit.load())
            {
                return;
            }

            int width = 0;
            int height = 0;
            int ret = 0;
            {
                std::lock_guard<std::mutex> guard(m_mxLockFrame);
                ret = avcodec_receive_frame(m_pCodecContext, m_pFrameYUV);
            }

            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF || ret < 0)
            {
                break;
            }
            width = m_pCodecContext->width;
            height = m_pCodecContext->height;
            //crash 在 sws_scale的问题，改一下av_image_fill_arrays 最后一个参数，对齐顺序就可以解决了。
            av_image_fill_arrays(m_pFrameRGB->data, m_pFrameRGB->linesize, m_pRGBBuffer, AV_PIX_FMT_RGB32, width, height, 4);
            sws_scale(m_pImgConvertCtx,
                (uint8_t const * const *)m_pFrameYUV->data,
                m_pFrameYUV->linesize, 0, height, m_pFrameRGB->data,
                m_pFrameRGB->linesize);

            if (m_pRGBBuffer)
            {
                DataCallBack(m_pRGBBuffer, width, height, height*m_pFrameRGB->linesize[0]);
            }
        }
    }
}

void H264Decode::SetUserCode(const char* pStrCode)
{
    m_strUserCode = pStrCode;
}

const char* H264Decode::GetUserCode()
{
    return m_strUserCode.c_str();
}

bool H264Decode::StartRecord(const char* pSavePath, const char* pUserName, pfnDecodeStatuCallBack pStatusCB)
{
    m_strSavePath = pSavePath;
    m_strTmpPath = m_strSavePath.substr(0, max(m_strSavePath.rfind('\\'), m_strSavePath.rfind('/')));
    std::string strTmpFile = m_strTmpPath + m_strUserCode + generate() + ".h264";
    m_pDataStatuCallBack = pStatusCB;
    CreateRecordFile(strTmpFile);
    if (m_pDataStatuCallBack)
    {
        m_pDataStatuCallBack(m_strUserCode.c_str(), Start_Record, 0);
    }
    return true;
}

bool H264Decode::StopRecord(const char* pEndTimeStr)
{
    CloseRecordFile();
    if (m_pDataStatuCallBack)
    {
        m_pDataStatuCallBack(m_strUserCode.c_str(), Stop_Record, 0);
    }
    //更改录像文件名称
    m_strSavePath.insert(m_strSavePath.rfind('.'), pEndTimeStr);
    auto pTransferTask = std::make_shared<TransferTask>(std::bind(&H264Decode::OnTransferTask, this, std::placeholders::_1, std::placeholders::_2), m_strCurRecordFile, m_strSavePath);
    m_queTransferTasks.push(pTransferTask);
    return false;
}
