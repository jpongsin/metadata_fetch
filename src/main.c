//metadata_fetch
//(c) 2026 jpongsin

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

#include <stdio.h>
#include <math.h>

///////////////////////////////////////
///global variables (will be extern soon)
//the following variables are the exact calculation as specified by TV and film standards
const double fps29 = 30000.0/1001.0;
const double fps59 = 60000.0/1001.0;

////////////////////////////////////////
///prototypes
int main(int argc,char*arg[]);
void fetchVideoInfo(const AVFormatContext *fic, int videoStreamIndex);
void validateVideo(char* videoFile,AVFormatContext*fic,int videoStreamIndex);
///////////////////////////////////////

//just runs the program
int main(int argc, char *argv[]) {
    ////////////////////////////////////////////////
    //check if you entered something.
    if (argc<2) {
        printf("You did not enter anything. Exiting.\n");
        return -1;
    }
    /////////////////////////////////////////////////
    //initialize argv[]
    char*videoFile=argv[1];
    AVFormatContext*forIOContext=NULL;
    int videoStreamIndex=-1;

    validateVideo(videoFile,forIOContext,videoStreamIndex);

    //cleanup
    avformat_close_input(&forIOContext);
    return 0;

}
void validateVideo(char* videoFile,AVFormatContext*fic,int videoStreamIndex) {
    //else... run
    printf("Loading: %s \n",videoFile);

    //check if video is supported
    if (avformat_open_input(&fic,videoFile,NULL,NULL)) {
        printf("Could not open video file. Exiting.\n");
        exit(-1);
    }

    //check if video has stream information
    if (avformat_find_stream_info(fic,NULL)<0) {
        avformat_close_input(&fic);
        printf("Could not find stream information. Exiting.\n");
        exit(-1);
    }

    //checking the
    //VIDEO
    //SOUNDTRACK
    //in loops
    for (int i=0;i<fic->nb_streams;i++) {
        if (fic->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex=i;
            break;
        }
    }

    //if outside the index....
    if (videoStreamIndex==-1) {
        printf("Could not find video stream. Exiting.\n");
        avformat_close_input(&fic);
        exit(-1);
    }

    //fetch video info
    fetchVideoInfo(fic, videoStreamIndex);
}

void fetchVideoInfo(const AVFormatContext *fic, int videoStreamIndex) {
    //proceed to calculate
    AVCodecParameters *pCodecPar=fic->streams[videoStreamIndex]->codecpar;
    const char *timecode_format="Duration: %.02ld:%.02ld:%.02ld:%.02ld seconds \n";

    //get frame rate avg
    AVStream *streams=fic->streams[videoStreamIndex];
    double frame_rate=av_q2d(streams->avg_frame_rate);
    if (frame_rate<=0) {
        frame_rate = av_q2d(streams->r_frame_rate);
    }
    printf("Frame rate: %.3f \n",frame_rate);

    //Check if it's NDF or DF
    char *tc_mode="Non Drop Frame";
    //printf("%.30f\n",fps29);
    //printf("%.30f\n",fps59);
    if (frame_rate==fps29||frame_rate==fps59) {
        tc_mode="Drop Frame";
        timecode_format="Duration: %.02ld:%.02ld:%.02ld;%.02ld seconds \n";

    }
    printf("Timecode Mode: %s \n",tc_mode);

    //frame prep for timecode format.
    int frame_rounded = (int)round(frame_rate);
    //get total frames
    int64_t totalFrames= streams->nb_frames;
    //calculate FF
    int64_t framesMod=(totalFrames % frame_rounded);
    //eg. 120 seconds
    int64_t seconds_raw=(fic->duration/AV_TIME_BASE);
    //becomes 00
    int64_t seconds=seconds_raw%60;
    //becomes 02
    int64_t minutes=seconds_raw/60;
    //becomes 00
    int64_t hours=(minutes/60)%24;

    printf("Width: %d \n",pCodecPar->width);
    printf("Height: %d \n", pCodecPar->height);
    printf(timecode_format, hours, minutes, seconds,framesMod);

}
