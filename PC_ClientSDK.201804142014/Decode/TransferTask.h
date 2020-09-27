#pragma once
#include <thread>
#include <atomic>
#include <queue>
#include <condition_variable>
class TransferTask
{
public:
    enum TransferStatu
    {
        Start,
        End_Succeed,
        End_Failed,
    };
    using Ptr = std::shared_ptr<TransferTask>;
    using TransferCallBack = std::function<void(TransferStatu, const std::string &) >;
    TransferTask(TransferCallBack func, const std::string& strSrcFileName, const std::string& strDstFileName);
    ~TransferTask();

    void RunTransferTask();
    void EndTask();
protected:
    int TransferH264toVideo(const std::string& strSrcFileName, const std::string& strDstFileName);
protected:

    std::string m_strSrcFile;
    std::string m_strDstFile;
    std::thread m_thTransfer;
    std::mutex m_mxData;
    std::atomic<bool> m_bIsComplete{ false };
    TransferCallBack  m_pCBFunc = nullptr;
};

