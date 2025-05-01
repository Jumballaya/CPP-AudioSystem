#include <miniaudio.h>

#include <cstring>
#include <iostream>
#include <thread>

#include "audio/SoundBuffer.hpp"
#include "audio/Voice.hpp"
#include "audio/VoiceManager.hpp"

std::unique_ptr<audio::VoiceManager> voiceManager;
std::shared_ptr<audio::SoundBuffer> buffer;

int main() {
  buffer = audio::SoundBuffer::loadWav("assets/acid-wool-cloth.wav");
  voiceManager = std::make_unique<audio::VoiceManager>();

  ma_device_config config = ma_device_config_init(ma_device_type_playback);
  config.playback.format = ma_format_f32;
  config.playback.channels = buffer->channels();
  config.sampleRate = buffer->sampleRate();
  config.dataCallback = [](ma_device*, void* output, const void*, ma_uint32 frameCount) {
    voiceManager->mix(static_cast<float*>(output), frameCount, buffer->channels());
  };
  config.pUserData = nullptr;

  ma_device device;
  if (ma_device_init(nullptr, &config, &device) != MA_SUCCESS) {
    std::cerr << "Failed to init device" << std::endl;
    return 1;
  }

  ma_device_start(&device);

  std::cout << "Press [Enter] to play sound. Ctrl+C to quit.\n";
  std::string line;

  float g = 1.0f;
  while (std::getline(std::cin, line)) {
    voiceManager->play(buffer, g);
    g *= 0.8f;
  }

  ma_device_uninit(&device);
  return 0;
}