#include "FormatContext.h"

#include <iostream>

FormatContext::FormatContext(AVFormatContext* context)
    : format_context(context) {}

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

void FormatContext::decode_frames(
    AVStream* video_stream, CodecContext& codec_context,
    std::function<void(AVFrame*)> on_frame_decoded) {
  auto frame = av_frame_alloc();

  auto packet = AVPacket();

  while (av_read_frame(format_context, &packet) == 0) {
    if (packet.stream_index == video_stream->index) {
      codec_context.send_packet(&packet);
      while (codec_context.receive_frame(frame) == 0) {
        on_frame_decoded(frame);
      }
    }
  }

  av_packet_unref(&packet);
}

FormatContext::~FormatContext() { avformat_close_input(&format_context); }
