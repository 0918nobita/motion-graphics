#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

class CodecContext {
  AVCodecContext* codec_context;

  CodecContext(AVCodecContext* context);

 public:
  static CodecContext alloc(const AVCodec* codec);

  void set_codec_params(const AVCodecParameters* params);

  void init_to_use_codec(const AVCodec* codec);

  ~CodecContext();
};
