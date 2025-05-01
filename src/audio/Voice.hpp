#pragma once

#include <cstddef>
#include <memory>

#include "SoundBuffer.hpp"

namespace audio {
class Voice {
 public:
  Voice(std::shared_ptr<SoundBuffer> buffer);

  bool isFinished() const;
  void mix(float* out, uint32_t frameCount);
  std::shared_ptr<SoundBuffer> buffer() const {
    return _buffer;
  }

 private:
  std::shared_ptr<SoundBuffer> _buffer;
  size_t _cursor = 0;
};
}  // namespace audio
