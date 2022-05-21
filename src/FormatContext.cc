#include "FormatContext.h"

#include <iostream>

FormatContext::FormatContext(AVFormatContext* context) {
  format_context = context;
}

FormatContext FormatContext::open_input(const char* url) {
  AVFormatContext* format_context = nullptr;
  avformat_open_input(&format_context, url, nullptr, nullptr);
  if (format_context == nullptr) {
    throw std::runtime_error("Could not open input");
  } else {
    return FormatContext(format_context);
  }
}

AVStream* FormatContext::find_video_stream() {
  AVStream* video_stream = nullptr;

  for (int i = 0; i < (int)format_context->nb_streams; ++i) {
    if (format_context->streams[i]->codecpar->codec_type ==
        AVMEDIA_TYPE_VIDEO) {
      video_stream = format_context->streams[i];
      break;
    }
  }

  if (video_stream == nullptr) {
    throw std::runtime_error("No video stream found");
  } else {
    return video_stream;
  }
}

FormatContext::~FormatContext() { avformat_close_input(&format_context); }
