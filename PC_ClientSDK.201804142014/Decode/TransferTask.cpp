#include "stdafx.h"
#include "TransferTask.h"
extern "C"
{
#include <libavformat/avformat.h>
};

TransferTask::TransferTask(TransferCallBack func, const std::string& strSrcFileName, const std::string& strDstFileName)
    : m_strSrcFile(strSrcFileName)
    , m_strDstFile(strDstFileName)
    , m_pCBFunc(func)
{
    m_thTransfer = std::thread(&TransferTask::RunTransferTask, this);
}


TransferTask::~TransferTask()
{
    EndTask();
}

void TransferTask::RunTransferTask()
{
    m_bIsComplete.store(false);

    TransferStatu emStatu = TransferStatu::Start;
    std::string strInfo = "开始转码";
    if (m_pCBFunc)
    {
        m_pCBFunc(emStatu, strInfo);
    }

    if (m_strSrcFile.size())
    {
        if (TransferH264toVideo(m_strSrcFile, m_strDstFile) == 0)
        {
            strInfo = "转码完成，文件已保存:"+ m_strDstFile;
            emStatu = End_Succeed;
            DeleteFileA(m_strSrcFile.c_str());
        }
        else
        {
            strInfo = "转码失败！";
            emStatu = End_Failed;
        }
    }
    if (m_pCBFunc)
    {
        m_pCBFunc(emStatu, strInfo);
    }
    m_bIsComplete.store(true);
}

void TransferTask::EndTask()
{
    if (m_thTransfer.joinable())
    {
        m_thTransfer.join();
    }
}

int TransferTask::TransferH264toVideo(const std::string& strSrcFileName, const std::string& strDstFileName)
{
    AVOutputFormat *ofmt = NULL;
    //创建输入AVFormatContext对象和输出AVFormatContext对象
    AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;
    AVPacket pkt;
    const char *in_filename = strSrcFileName.c_str();
    const char *filename = strDstFileName.c_str() ;
    int ret, i;
    int stream_index = 0;
    int *stream_mapping = NULL;
    int stream_mapping_size = 0;


    //打开视频文件
    if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
        //LogMessage::DebugLogInfo("ReCode", "打开视频流失败");
        return -1;
    }
    //获取视频文件信息
    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
        //LogMessage::DebugLogInfo("ReCode", "获取视频流信息失败");
        return -1;
    }

    //打印信息
    //av_dump_format(ifmt_ctx, 0, in_filename, 0);

    //输出文件分配空间
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, filename);
    if (!ofmt_ctx) {
        //LogMessage::DebugLogInfo("ReCode", "输出文件分配空间分配失败");
        return -1;
    }

    stream_mapping_size = ifmt_ctx->nb_streams;
    stream_mapping = (int *)av_mallocz_array(stream_mapping_size, sizeof(*stream_mapping));
    if (!stream_mapping) {

        //LogMessage::DebugLogInfo("ReCode", "获取mapping失败");
        return -1;
    }

    ofmt = ofmt_ctx->oformat;

    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        AVStream *out_stream;
        AVStream *in_stream = ifmt_ctx->streams[i];
        AVCodecParameters *in_codecpar = in_stream->codecpar;

        if (in_codecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE) {
            stream_mapping[i] = -1;
            continue;
        }

        stream_mapping[i] = stream_index++;

        out_stream = avformat_new_stream(ofmt_ctx, NULL);
        if (!out_stream) {
            //fprintf(stderr, "Failed allocating output stream\n");
            //LogMessage::DebugLogInfo("ReCode", "分配流对象失败");
            return -1;
        }

        ret = avcodec_parameters_copy(out_stream->codecpar, in_codecpar);
        if (ret < 0) {
            //LogMessage::DebugLogInfo("ReCode", "拷贝视频code失败");
            return -1;
        }
        out_stream->codecpar->codec_tag = 0;
    }

    //打开文件
    if (!(ofmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            //LogMessage::DebugLogInfo("ReCode", "打开输出文件失败");
            return -1;
        }
    }
    //开始写入文件头
    ret = avformat_write_header(ofmt_ctx, NULL);
    if (ret < 0) {
        //LogMessage::DebugLogInfo("ReCode", "写入文件头失败");
        return -1;
    }
    int m_frame_index = 0;
    //开始读取视频流，并获取pkt信息
    while (1) {
        AVStream *in_stream, *out_stream;

        ret = av_read_frame(ifmt_ctx, &pkt);
        if (ret < 0)
            break;

        in_stream = ifmt_ctx->streams[pkt.stream_index];
        if (pkt.stream_index >= stream_mapping_size ||
            stream_mapping[pkt.stream_index] < 0) {
            av_packet_unref(&pkt);
            continue;
        }

        pkt.stream_index = stream_mapping[pkt.stream_index];
        out_stream = ofmt_ctx->streams[pkt.stream_index];

        //从摄像头直接保存的h264文件，重新编码时得自己加时间戳，不然转换出来的是没有时间的
        if (pkt.pts == AV_NOPTS_VALUE) {
            //Write PTS
            AVRational time_base1 = in_stream->time_base;
            //Duration between 2 frames (us)
            int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(in_stream->r_frame_rate);
            //Parameters
            pkt.pts = (double)(m_frame_index*calc_duration) / (double)(av_q2d(time_base1)*AV_TIME_BASE);
            pkt.dts = pkt.pts;
            pkt.duration = (double)calc_duration / (double)(av_q2d(time_base1)*AV_TIME_BASE);
        }

        /* copy packet */
        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;

        ret = av_interleaved_write_frame(ofmt_ctx, &pkt);
        if (ret < 0) {
            break;
        }
        av_packet_unref(&pkt);
        m_frame_index++;
    }

    av_write_trailer(ofmt_ctx);

    avformat_close_input(&ifmt_ctx);

    // close output 
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
        avio_closep(&ofmt_ctx->pb);
    avformat_free_context(ofmt_ctx);

    av_freep(&stream_mapping);
    return 0;
}
