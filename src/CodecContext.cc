#include <iostream>

#include "CodecContext.h"

CodecContext::CodecContext(AVCodecContext* context) : codec_context(context) {}

CodecContext CodecContext::alloc(const AVCodec* codec) {
  AVCodecContext* codec_context = avcodec_alloc_context3(codec);
  if (codec_context == nullptr) {
    throw std::runtime_error("Could not allocate codec context");
  } else {
    return CodecContext(codec_context);
  }
}

CodecContext::~CodecContext() { avcodec_free_context(&codec_context); }
