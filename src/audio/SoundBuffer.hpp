#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace audio {
class SoundBuffer {
 public:
  static std::shared_ptr<SoundBuffer> loadWav(const std::string& path);

  SoundBuffer() = default;

  const float* data() const { return _samples.data(); }
  size_t totalFrames() const { return _totalFrames; }
  uint32_t channels() const { return _numChannels; }
  uint32_t sampleRate() const { return _sampleRate; }

 private:
  std::vector<float> _samples;
  uint32_t _sampleRate = 0;
  uint32_t _numChannels = 0;
  size_t _totalFrames = 0;
};
}  // namespace audio