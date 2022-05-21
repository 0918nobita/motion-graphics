#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
    return EXIT_FAILURE;
  }

  AVFormatContext *format_context = nullptr;

  if (avformat_open_input(&format_context, argv[1], nullptr, nullptr) != 0) {
    std::cerr << "avformat_open_input failed" << std::endl;
    return EXIT_FAILURE;
  }

  AVStream *video_stream = nullptr;

  for (int i = 0; i < (int)format_context->nb_streams; ++i) {
    if (format_context->streams[i]->codecpar->codec_type ==
        AVMEDIA_TYPE_VIDEO) {
      video_stream = format_context->streams[i];
      break;
    }
  }

  if (video_stream == nullptr) {
    std::cerr << "No video stream found" << std::endl;
    return EXIT_FAILURE;
  }

  avformat_close_input(&format_context);

  return EXIT_SUCCESS;
}
