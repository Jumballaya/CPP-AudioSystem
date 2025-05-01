#include <miniaudio.h>

#include <cstring>
#include <iostream>
#include <thread>

#include "audio/SoundBuffer.hpp"
#include "audio/Voice.hpp"

std::unique_ptr<audio::Voice> voice;

int main() {
  auto buffer = audio::SoundBuffer::loadWav("assets/acid-wool-cloth.wav");
  voice = std::make_unique<audio::Voice>(buffer);

  // Audio callback
  ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = ma_format_f32;
  deviceConfig.playback.channels = buffer->channels();
  deviceConfig.sampleRate = buffer->sampleRate();
  deviceConfig.dataCallback = [](ma_device* device, void* output, const void*, ma_uint32 frameCount) {
    float* out = static_cast<float*>(output);
    std::memset(out, 0, sizeof(float) * frameCount * voice->buffer()->channels());  // clear first
    if (voice && !voice->isFinished()) {
      voice->mix(out, frameCount);
    }
  };
  deviceConfig.pUserData = nullptr;

  ma_device device;
  if (ma_device_init(nullptr, &deviceConfig, &device) != MA_SUCCESS) {
    std::cerr << "Failed to init device\n";
    return 1;
  }

  ma_device_start(&device);
  std::cout << "Playing..." << std::endl;

  while (!voice->isFinished()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  ma_device_uninit(&device);
  return 0;
}