#pragma once

#include <functional>

extern "C" {
#include <libavformat/avformat.h>
}

#include "CodecContext.h"

class FormatContext {
  AVFormatContext* format_context;

  FormatContext(AVFormatContext* context);

 public:
  static FormatContext open_input(const char* url);

  AVStream* find_video_stream();

  void decode_frames(AVStream* video_stream, CodecContext& codec_context,
                     std::function<void(AVFrame*)> on_frame_decoded);

  ~FormatContext();
};
