#pragma once

#include <memory>
#include <mutex>
#include <vector>

#include "SoundBuffer.hpp"
#include "Voice.hpp"

namespace audio {
class VoiceManager {
 public:
  void play(std::shared_ptr<SoundBuffer> buffer);
  void mix(float* output, uint32_t frameCount, uint32_t channels);

 private:
  std::vector<std::unique_ptr<Voice>> _voices;
  std::mutex _mutex;
};
}  // namespace audio