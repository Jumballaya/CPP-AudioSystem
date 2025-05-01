#include "SoundBuffer.hpp"

#include <miniaudio.h>

#include <stdexcept>

namespace audio {
std::shared_ptr<SoundBuffer> SoundBuffer::loadWav(const std::string& path) {
  auto buffer = std::make_shared<SoundBuffer>();

  ma_result result;
  ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, 48000);

  ma_decoder decoder;
  result = ma_decoder_init_file(path.c_str(), &config, &decoder);
  if (result != MA_SUCCESS) {
    throw std::runtime_error("Failed to load .wav file");
  }

  buffer->_sampleRate = decoder.outputSampleRate;
  buffer->_numChannels = decoder.outputChannels;

  ma_uint64 frameCount = 0;
  result = ma_decoder_get_length_in_pcm_frames(&decoder, &frameCount);
  if (result != MA_SUCCESS) {
    ma_decoder_uninit(&decoder);
    throw std::runtime_error("Failed to get length of WAV file");
  }

  buffer->_totalFrames = frameCount;
  buffer->_samples.resize(buffer->_totalFrames * buffer->_numChannels);

  ma_decoder_read_pcm_frames(&decoder, buffer->_samples.data(), buffer->_totalFrames, nullptr);
  ma_decoder_uninit(&decoder);

  return buffer;
}
}  // namespace audio
