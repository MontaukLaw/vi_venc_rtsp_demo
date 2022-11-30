#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "../../common/sample_common.h"
#include "../../librtsp/rtsp_demo.h"
#include "rkmedia_api.h"
#include "rkmedia_venc.h"
#include "../include/rtsp_comm.h"
#include "../include/rkmedia_comm.h"

bool quitRun = false;

static void sigterm_handler(int sig) {
    fprintf(stderr, "signal %d\n", sig);
    quitRun = true;
}

int main(int argc, char *argv[]) {
    RK_U32 u32Width = 1920;
    RK_U32 u32Height = 1080;
    RK_CHAR *pDeviceName = "rkispp_scale0";
    RK_CHAR *pIqfilesPath = "/oem/etc/iqfiles";
    CODEC_TYPE_E enCodecType = RK_CODEC_TYPE_H264;
    RK_CHAR *pCodecName = "H264";
    RK_S32 s32CamId = 0;
    RK_BOOL bMultictx = RK_FALSE;
    int c;
    int ret = 0;
    int fps = 30;

    init_isp(s32CamId, fps, pIqfilesPath);

    init_rtsp();

    RK_MPI_SYS_Init();

    create_vi_chn(u32Width, u32Height, s32CamId, 0, pDeviceName);

    create_venc_chn(fps, u32Width, u32Height, 0);

    reg_venc_cb(0, video_packet_cb);

    bind_vi_venc(s32CamId, 0, 0);

    printf("%s initial finish\n", __func__);
    signal(SIGINT, sigterm_handler);

    while (!quitRun) {
        usleep(500000);
    }

    printf("%s exit!\n", __func__);

    clean_rtsp();

    // unbind first
    unbind_vi_venc(0, 0);

    // destroy venc before vi
    destroy_venc_chn(0);

    disable_vi_chn(s32CamId, 0);

    SAMPLE_COMM_ISP_Stop(s32CamId);

    return 0;

}