#ifndef EXAMPLES_RTSP_COMM_H
#define EXAMPLES_RTSP_COMM_H

#include "rkmedia_api.h"
#include "../../librtsp/rtsp_demo.h"
#include "../../common/sample_common.h"

#ifdef __cplusplus
extern "C" {
#endif

void init_rtsp(void);

void video_packet_cb(MEDIA_BUFFER mb);

void clean_rtsp(void);

#ifdef __cplusplus
}
#endif

#endif //EXAMPLES_RTSP_COMM_H
