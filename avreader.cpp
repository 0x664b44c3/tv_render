#include "avreader.h"

AVReader::AVReader()
{

}



//// ffmpeg_sample.c
//// Date: Sep 05, 2013
//// Code based on a https://raw.github.com/phamquy/FFmpeg-tutorial-samples/master/tutorial01.c
//// Tested on CentOS 5.7, GCC 4.1.2,FFMPEG 0.10.1
//// libavcodec.so.53.60.100  libavdevice.so.53.4.100  libavfilter.so.2.60.100
//// libavformat.so.53.31.100  libavutil.so.51.34.101  libswresample.so.0.6.100
//// libswscale.so.2.1.100
////
//// A small sample program that shows how to use libavformat to decode a video file and save it as Y frames.
////
//// Build (assuming libavformat, libavcodec, libavutils are correctly installed on your system).
////
//// gcc -o sample ffmpeg_sample.c -lavformat
////
//// Run using
////
//// ./sample myvideofile.mpg
////
//// To view the generated output
////
//// mplayer -demuxer rawvideo -rawvideo w=[LINESIZE]:h=[HEIGHT]:format=y8 out.raw -loop 0

//#include
//#include

//int readsave_frames(int videoStreamIdx
//                , AVFormatContext *pFormatCtx
//                , AVCodecContext  *pCodecCtx
//                , AVCodec         *pCodec
//)
//{
//    int             y, i;
//    FILE           *pFile;
//    AVPacket        packet;
//    int             frameFinished;
//    AVFrame        *pFrame;


//    // Open file
//    pFile=fopen("out.raw", "wb");
//    if(pFile==NULL)
//    {
//        printf("Unable to open output file\n");
//        return -1;
//    }

//    /// Allocate video frame
//    pFrame = avcodec_alloc_frame();

//    printf("\n");
//    for(i=0; av_read_frame(pFormatCtx, &packet) >= 0;) {

//        // Is this a packet from the video stream?
//        if(packet.stream_index==videoStreamIdx) {
//            i++;

//            /// Decode video frame
//            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

//            // Did we get a video frame?
//            if(frameFinished) {
//                printf("\rFrame [%d]: pts=%lld, pkt_pts=%lld, pkt_dts=%lld", i, pFrame->pts, pFrame->pkt_pts, pFrame->pkt_dts);
//                // Write pixel data
//                for(y=0; yheight; y++)
//                    fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, pFrame->linesize[0], pFile);
//            }
//        }

//        // Free the packet that was allocated by av_read_frame
//        av_free_packet(&packet);
//    }
//    printf("\n");

//    /// Free the Y frame
//    av_free(pFrame);

//    // Close file
//    fclose(pFile);

//    return 0;
//}


//int main(int argc, char *argv[]) {
//    AVFormatContext *pFormatCtx;
//    int             videoStreamIdx;
//    AVCodecContext  *pCodecCtx;
//    AVCodec         *pCodec;

//    if(argc < 2) {
//        printf("Please provide a movie file\n");
//        return -1;
//    }
//    // Register all formats and codecs
//    av_register_all();

//    pFormatCtx = avformat_alloc_context();

//    /// Open video file
//    if(avformat_open_input(&pFormatCtx, argv[1], 0, NULL) != 0)
//    {
//        printf("avformat_open_input failed: Couldn't open file\n");
//        return -1; // Couldn't open file
//    }

//    /// Retrieve stream information
//    if(avformat_find_stream_info(pFormatCtx, NULL) < 0)
//    {
//        printf("avformat_find_stream_info failed: Couldn't find stream information\n");
//        return -1; // Couldn't find stream information
//    }

//    /// Dump information about file onto standard error
//    av_dump_format(pFormatCtx, 0, argv[1], 0);


//    /// Find the first video stream
//    {
//        int i = 0;
//        videoStreamIdx=-1;
//        for(i=0; inb_streams; i++)
//        {
//            if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) { //CODEC_TYPE_VIDEO
//                videoStreamIdx=i;
//                break;
//            }
//        }
//    }
//    /// Check if video stream is found
//    if(videoStreamIdx==-1)
//        return -1; // Didn't find a video stream


//    /// Get a pointer to the codec context for the video stream
//    pCodecCtx = pFormatCtx->streams[videoStreamIdx]->codec;


//    /// Find the decoder for the video stream
//    pCodec = avcodec_find_decoder( pCodecCtx->codec_id);
//    if(pCodec==NULL) {
//        fprintf(stderr, "Unsupported codec!\n");
//        return -1; // Codec not found
//    }

//    /// Open codec
//    if( avcodec_open2(pCodecCtx, pCodec, NULL) < 0 )
//        return -1; // Could not open codec

//    // Read frames and save them to disk
//    if ( readsave_frames(videoStreamIdx, pFormatCtx, pCodecCtx, pCodec) < 0)
//    {
//        return -1;
//    }

//    /// Close the codec
//    avcodec_close(pCodecCtx);

//    /// Close the video file
//    avformat_close_input(&pFormatCtx);

//    return 0;
//}
