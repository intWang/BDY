#pragma once

#include <stdio.h>
#include "IRecoder.h"
#include <thread>
#include <atomic>
#include <queue>
#include <condition_variable>
#include "DecodeUtils.h"
extern "C"
{
#include <libavformat/avformat.h>
};

class Recorder :public IRecoder
{
public:
    Recorder();
    ~Recorder();
    virtual bool StartRecord(std::string& strUID, std::string& strSavePath) override;
    virtual bool StopRecord() override;
    virtual bool RegisteRecordStatuCB(std::function<void(const std::string &, RecordStatu) > func) override;
    virtual void InputData(unsigned char*data, int len) override;

protected:
    AVStream *add_stream(AVFormatContext *oc, AVCodec **codec, enum AVCodecID codec_id);
    void open_video(AVFormatContext *oc, AVCodec *codec, AVStream *st);
    int CreateMp4(const char* filename);
    void WriteVideo(void* data, int nLen);
    void CloseMp4();


    void RunPacketThread();
protected:
    int m_waitkey = 1;
    int m_frame_index = 0;
    int STREAM_FRAME_RATE = 25;
    AVFormatContext* m_pOc;
    AVCodecContext * c;
    int vi;


    ////
    std::string m_strUID;
    std::string m_strSavePath;

    std::thread m_thDecode;
    std::mutex m_mxData;
    std::atomic<bool> m_bQuit{ false };
    std::condition_variable m_covDataCome;
    std::queue<BufferData::Ptr> m_queDatas;
    std::atomic<bool> m_bWork{ false };
    std::function<void(const std::string &, RecordStatu) > m_pCBFunc = nullptr;
};

