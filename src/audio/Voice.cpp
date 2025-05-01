#include "Voice.hpp"

#include <cstring>

namespace audio {

Voice::Voice(std::shared_ptr<SoundBuffer> buffer) : _buffer(std::move(buffer)) {}

bool Voice::isFinished() const {
  return _cursor >= _buffer->totalFrames();
}

void Voice::mix(float* out, uint32_t frameCount) {
  if (isFinished()) return;

  const float* data = _buffer->data();
  uint32_t channels = _buffer->channels();
  size_t totalFrames = _buffer->totalFrames();

  for (uint32_t i = 0; i < frameCount; ++i) {
    if (_cursor >= totalFrames) {
      if (_looping) {
        _cursor = 0;
      } else {
        break;
      }
    }

    for (uint32_t ch = 0; ch < channels; ++ch) {
      size_t sampleIndex = _cursor * channels + ch;
      out[i * channels + ch] += _gain * data[sampleIndex];
    }
    _cursor++;
  }
}

}  // namespace audio