
#include "IDecoder.h"
#include <thread>
#include "Decode.h"
#include <atomic>
#include <condition_variable>
#include <queue>
#include "DecodeUtils.h"
#include "TransferTask.h"
struct AVCodecContext;
struct AVCodec;
struct AVFrame;
struct AVCodecParserContext;
struct AVPacket;
struct SwsContext;
class H264Decode :public IDecoder
{
public:
    using Ptr = std::shared_ptr<H264Decode>;
    H264Decode();
    ~H264Decode();

    virtual bool Init(pfnDecodeCallBack pfnCallBack) override;
    virtual bool Destory() override;
    virtual void InputData(unsigned char*data, int len) override;
    virtual long GetHandle() override;

    void RunDecodeThread();

    virtual void SetUserCode(const char* pStrCode) override;
    virtual const char* GetUserCode() override;
    virtual bool StartRecord(const char* pSavePath, const char* pUserName, pfnDecodeStatuCallBack pStatusCB);
    virtual bool StopRecord(const char* pEndTimeStr) override;
protected:
    void DataCallBack(const unsigned char* pData, int width, int height, int len);
    void CreateRecordFile(const std::string& strTmpDataFile);
    void CloseRecordFile();
    void WriteRecordFile(unsigned char*data, int len);
    void OnTransferTask(TransferTask::TransferStatu emStatu, const std::string & strInfo);
protected:
    bool m_init = false;
    AVCodecContext* m_pCodecContext = nullptr;
    AVCodec * m_pH264VideoDecoder = nullptr;
    AVFrame * m_pFrameYUV = nullptr;
    AVFrame * m_pFrameRGB = nullptr;
    AVCodecParserContext *m_pParser = nullptr;
    AVPacket *m_pPacket = nullptr;
    uint8_t * m_pRGBBuffer = nullptr;
    SwsContext *m_pImgConvertCtx = nullptr;
    pfnDecodeCallBack m_pDataCallBack = nullptr;
    std::thread m_thDecode;
    std::atomic<bool> m_bQuit{ false };
    std::condition_variable m_covDataCome;
    std::string m_strUserCode;
    std::mutex m_mxLockFrame;


    ////record
    bool m_bInRecord;
    bool m_bInTransfer;
    FILE* m_pRecordFile = nullptr;
    std::string m_strSavePath;
    std::string m_strTmpPath;
    std::string m_strCurRecordFile;
    pfnDecodeStatuCallBack m_pDataStatuCallBack = nullptr;
    std::queue<TransferTask::Ptr> m_queTransferTasks;
};



