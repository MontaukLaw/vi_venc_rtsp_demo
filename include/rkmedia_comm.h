#ifndef EXAMPLES_RKMEDIA_COMM_H
#define EXAMPLES_RKMEDIA_COMM_H

#include "rkmedia_api.h"
#include <stdio.h>
#include <string.h>
#include "../../librtsp/rtsp_demo.h"
#include "../../common/sample_common.h"

#ifdef __cplusplus
extern "C" {
#endif

int bind_vi_venc(RK_S32 cameraId, RK_U32 viChnId, RK_U32 vencChnId);

int destroy_venc_chn(RK_U32 vencChn);

int disable_vi_chn(RK_U32 cameraId, RK_U32 viChnId);

int reg_venc_cb(RK_S32 vencChn, OutCbFunc cb);

int create_venc_chn(RK_U32 fps, RK_U32 u32Width, RK_U32 u32Height, RK_U32 vencChn);

int create_vi_chn(RK_U32 u32Width, RK_U32 u32Height, RK_S32 s32CamId, RK_U32 u32ViChnId, RK_CHAR *pDeviceName);

void init_rtsp_h264(rtsp_demo_handle g_rtsplive, rtsp_session_handle g_rtsp_session);

void init_isp(RK_S32 s32CamId, RK_U32 fps, RK_CHAR *pIqfilesPath);

int unbind_vi_venc(RK_S32 viChn, RK_S32 vencChn);

int create_rga_chn(RK_S32 rgaChn, RK_U32 u32WidthIn, RK_U32 u32HeightIn, RK_U32 u32WidthOut, RK_U32 u32HeightOut);

int bind_vi_rga(RK_S32 cameraId, RK_U32 viChnId, RK_U32 rgaChnId);

int bind_rga_venc(RK_S32 rgaChnId, RK_S32 vencChnId);

int unbind_vi_vga(RK_S32 viChn, RK_S32 vgaChn);

int unbind_vga_venc(RK_S32 vgaChn, RK_S32 vencChn);

int destroy_vga_chn(RK_U32 vgaChn);

#ifdef __cplusplus
}
#endif


#endif //EXAMPLES_RKMEDIA_COMM_H
