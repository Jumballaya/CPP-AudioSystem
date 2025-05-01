#include "VoiceManager.hpp"

#include <algorithm>
#include <cstring>

namespace audio {

void VoiceManager::play(std::shared_ptr<SoundBuffer> buffer) {
  std::lock_guard lock(_mutex);
  _voices.emplace_back(std::make_unique<Voice>(std::move(buffer)));
}

void VoiceManager::mix(float* output, uint32_t frameCount, uint32_t channels) {
  std::lock_guard lock(_mutex);
  std::memset(output, 0, sizeof(float) * frameCount * channels);

  for (auto& voice : _voices) {
    if (!voice->isFinished()) {
      voice->mix(output, frameCount);
    }
  }

  _voices.erase(std::remove_if(
                    _voices.begin(),
                    _voices.end(),
                    [](const std::unique_ptr<Voice>& v) {
                      return v->isFinished();
                    }),
                _voices.end());
}

}  // namespace audio