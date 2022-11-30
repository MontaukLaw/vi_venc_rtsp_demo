#include "../include/rkmedia_comm.h"

int create_vi_chn(RK_U32 u32Width, RK_U32 u32Height, RK_S32 s32CamId, RK_U32 u32ViChnId, RK_CHAR *pDeviceName) {
    VI_CHN_ATTR_S vi_chn_attr;
    vi_chn_attr.pcVideoNode = pDeviceName;
    vi_chn_attr.u32BufCnt = 3;
    vi_chn_attr.u32Width = u32Width;
    vi_chn_attr.u32Height = u32Height;
    vi_chn_attr.enPixFmt = IMAGE_TYPE_NV12;
    vi_chn_attr.enBufType = VI_CHN_BUF_TYPE_MMAP;
    vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;
    int ret = RK_MPI_VI_SetChnAttr(s32CamId, u32ViChnId, &vi_chn_attr);
    ret |= RK_MPI_VI_EnableChn(s32CamId, u32ViChnId);
    if (ret) {
        printf("ERROR: create VI[%d] error! ret=%d\n", u32ViChnId, ret);
        return 0;
    }
    return -1;
}

// H264 codec
int create_venc_chn(RK_U32 fps, RK_U32 u32Width, RK_U32 u32Height, RK_U32 vencChn) {

    VENC_CHN_ATTR_S venc_chn_attr;
    memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));

    int ret = 0;
    venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H264;
    venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
    venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = fps;
    venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = u32Width * u32Height;

    // frame rate: in 30/1, out 30/1.
    venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen = 1;
    venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum = fps;
    venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen = 1;
    venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum = fps;

    venc_chn_attr.stVencAttr.imageType = IMAGE_TYPE_NV12;
    venc_chn_attr.stVencAttr.u32PicWidth = u32Width;
    venc_chn_attr.stVencAttr.u32PicHeight = u32Height;
    venc_chn_attr.stVencAttr.u32VirWidth = u32Width;
    venc_chn_attr.stVencAttr.u32VirHeight = u32Height;
    venc_chn_attr.stVencAttr.u32Profile = 77;  // main profile

    ret = RK_MPI_VENC_CreateChn(vencChn, &venc_chn_attr);
    if (ret) {
        printf("ERROR: create VENC[%d] error! ret=%d\n", vencChn, ret);
        return -1;
    }

    return 0;
}

int reg_venc_cb(RK_S32 vencChn, OutCbFunc cb) {

    MPP_CHN_S stEncChn;
    stEncChn.enModId = RK_ID_VENC;
    stEncChn.s32ChnId = vencChn;

    // 注册数据输出回调。与RK_MPI_SYS_GetMediaBuffer相比，无需缓存buffer等待用户索取，因此更节省内存。
    int ret = RK_MPI_SYS_RegisterOutCb(&stEncChn, cb);
    if (ret) {
        printf("ERROR: register output callback for VENC[0] error! ret=%d\n", ret);
        return 0;
    }
    return -1;
}


int bind_vi_venc(RK_S32 cameraId, RK_U32 viChnId, RK_U32 vencChnId) {

    MPP_CHN_S stSrcChn;
    stSrcChn.enModId = RK_ID_VI;
    stSrcChn.s32DevId = cameraId;
    stSrcChn.s32ChnId = viChnId;

    MPP_CHN_S stDestChn;
    stDestChn.enModId = RK_ID_VENC;
    // stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = vencChnId;

    int ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (ret) {
        printf("ERROR: Bind VI[0] and VENC[0] error! ret=%d\n", ret);
        return -1;
    }

    return 0;
}

int unbind_vi_venc(RK_S32 viChn, RK_S32 vencChn) {

    MPP_CHN_S stSrcChn;
    stSrcChn.enModId = RK_ID_VI;
    stSrcChn.s32ChnId = viChn;

    MPP_CHN_S stDestChn;
    stDestChn.enModId = RK_ID_VENC;
    stDestChn.s32ChnId = vencChn;

    // unbind first
    int ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (ret) {
        printf("ERROR: UnBind VI[0] and VENC[0] error! ret=%d\n", ret);
        return 0;
    }
    return -1;
}

int destroy_venc_chn(RK_U32 vencChn) {
    int ret = RK_MPI_VENC_DestroyChn(vencChn);
    if (ret) {
        printf("ERROR: Destroy VENC[%d] error! ret=%d\n", vencChn, ret);
        return 0;
    }
    return -1;
}

int disable_vi_chn(RK_U32 cameraId, RK_U32 viChnId) {
    int ret = RK_MPI_VI_DisableChn(cameraId, viChnId);
    if (ret) {
        printf("ERROR: Destroy VI[0] error! ret=%d\n", ret);
        return 0;
    }

}

void init_rtsp_h264(rtsp_demo_handle g_rtsplive, rtsp_session_handle g_rtsp_session) {

    g_rtsplive = create_rtsp_demo(554);

    g_rtsp_session = rtsp_new_session(g_rtsplive, "/live/main_stream");

    rtsp_set_video(g_rtsp_session, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);

    rtsp_sync_video_ts(g_rtsp_session, rtsp_get_reltime(), rtsp_get_ntptime());

}

void init_isp(RK_S32 s32CamId, RK_U32 fps, RK_CHAR *pIqfilesPath) {
    SAMPLE_COMM_ISP_Init(s32CamId, RK_AIQ_WORKING_MODE_NORMAL, RK_FALSE, pIqfilesPath);
    SAMPLE_COMM_ISP_Run(s32CamId);
    SAMPLE_COMM_ISP_SetFrameRate(s32CamId, fps);
}