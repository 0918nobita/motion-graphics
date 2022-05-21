#include <iostream>
#include <memory>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

class FormatContext {
  AVFormatContext* format_context;

  FormatContext(AVFormatContext* context) { format_context = context; }

 public:
  static FormatContext open_input(const char* url) {
    AVFormatContext* format_context = nullptr;
    avformat_open_input(&format_context, url, nullptr, nullptr);
    if (format_context == nullptr) {
      throw std::runtime_error("Could not open input");
    } else {
      return FormatContext(format_context);
    }
  }

  AVStream* find_video_stream() {
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

  ~FormatContext() {
    avformat_close_input(&format_context);
  }
};

class CodecContext {
  AVCodecContext* codec_context;

  CodecContext(AVCodecContext* context) { codec_context = context; }

 public:
  static CodecContext alloc(const AVCodec* codec) {
    AVCodecContext* codec_context = avcodec_alloc_context3(codec);
    if (codec_context == nullptr) {
      throw std::runtime_error("Could not allocate codec context");
    } else {
      return CodecContext(codec_context);
    }
  }

  ~CodecContext() {
    avcodec_free_context(&codec_context);
  }
};

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
    return EXIT_FAILURE;
  }

  try {
    auto format_context = FormatContext::open_input(argv[1]);

    auto video_stream = format_context.find_video_stream();

    const AVCodec* codec =
        avcodec_find_decoder(video_stream->codecpar->codec_id);
    if (codec == nullptr) {
      throw std::runtime_error("No supported decoder found");
    }

    auto codec_context = CodecContext::alloc(codec);
  } catch (std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
