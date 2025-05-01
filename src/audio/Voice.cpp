#include "Voice.hpp"

#include <cstring>

namespace audio {

Voice::Voice(std::shared_ptr<SoundBuffer> buffer) : _buffer(std::move(buffer)) {}

bool Voice::isFinished() const {
  return _cursor >= _buffer->totalFrames();
}

void Voice::mix(float* out, uint32_t frameCount) {
  if (isFinished()) {
    std::memset(out, 0, sizeof(float) * frameCount * _buffer->channels());
    return;
  }

  const float* data = _buffer->data();
  uint32_t channels = _buffer->channels();
  size_t totalFrames = _buffer->totalFrames();

  for (uint32_t i = 0; i < frameCount; ++i) {
    for (uint32_t ch = 0; ch < channels; ++ch) {
      size_t sampleIndex = (_cursor + i) * channels + ch;
      if (_cursor + i < totalFrames) {
        out[i * channels + ch] += _gain * data[sampleIndex];
      } else {
        out[i * channels + ch] += 0.0f;
      }
    }
  }

  _cursor += frameCount;
}

}  // namespace audio