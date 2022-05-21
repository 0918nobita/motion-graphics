#pragma once

extern "C" {
#include <libavformat/avformat.h>
}

class FormatContext {
  AVFormatContext* format_context;

  FormatContext(AVFormatContext* context);

 public:
  static FormatContext open_input(const char* url);

  AVStream* find_video_stream();

  ~FormatContext();
};
