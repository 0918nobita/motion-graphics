#include <iostream>

#include "CodecContext.h"
#include "FormatContext.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
    return EXIT_FAILURE;
  }

  try {
    auto format_context = FormatContext::open_input(argv[1]);

    auto video_stream = format_context.find_video_stream();

    auto codec = avcodec_find_decoder(video_stream->codecpar->codec_id);
    if (codec == nullptr) {
      throw std::runtime_error("No supported decoder found");
    }

    auto codec_context = CodecContext::alloc(codec);

    codec_context.set_codec_params(video_stream->codecpar);

    codec_context.init_to_use_codec(codec);

    auto on_frame_decoded = [](AVFrame* frame) {
      (void)frame;
      std::cout << "f" << std::flush;
    };

    format_context.decode_frames(video_stream, codec_context, on_frame_decoded);

    std::cout << std::endl << "Done" << std::endl;
  } catch (std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
