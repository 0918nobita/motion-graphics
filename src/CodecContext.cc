#include "CodecContext.h"

#include <iostream>

CodecContext::CodecContext(AVCodecContext* context) : codec_context(context) {}

CodecContext CodecContext::alloc(const AVCodec* codec) {
  AVCodecContext* codec_context = avcodec_alloc_context3(codec);
  if (codec_context == nullptr) {
    throw std::runtime_error("Could not allocate codec context");
  } else {
    return CodecContext(codec_context);
  }
}

void CodecContext::set_codec_params(const AVCodecParameters* params) {
  if (avcodec_parameters_to_context(codec_context, params) < 0)
    throw std::runtime_error("avcodec_paa_parameters_to_context failed");
}

void CodecContext::init_to_use_codec(const AVCodec* codec) {
  if (avcodec_open2(codec_context, codec, nullptr) != 0)
    throw std::runtime_error("avcodec_open2 failed");
}

void CodecContext::send_packet(AVPacket* packet) {
  if (avcodec_send_packet(codec_context, packet) != 0)
    throw std::runtime_error("avcodec_send_packet failed");
}

int CodecContext::receive_frame(AVFrame* frame) {
  return avcodec_receive_frame(codec_context, frame);
}

CodecContext::~CodecContext() { avcodec_free_context(&codec_context); }
