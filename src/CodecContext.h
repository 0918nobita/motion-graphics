#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

class CodecContext {
  AVCodecContext* codec_context;

  CodecContext(AVCodecContext* context);

 public:
  static CodecContext alloc(const AVCodec* codec);

  ~CodecContext();
};
