#include "AudioSystem.hpp"

#include <miniaudio.h>

#include <cstring>
#include <iostream>

namespace audio {

AudioSystem::AudioSystem() {
  _voiceManager = std::make_unique<VoiceManager>();

  ma_device_config config = ma_device_config_init(ma_device_type_playback);
  config.sampleRate = 48000;
  config.playback.format = ma_format_f32;
  config.playback.channels = 2;
  config.dataCallback = audioCallback;
  config.pUserData = this;

  _sampleRate = config.sampleRate;
  _channels = config.playback.channels;

  if (ma_device_init(nullptr, &config, &_device) != MA_SUCCESS) {
    throw std::runtime_error("Failed to initialize audio device");
  }

  if (ma_device_start(&_device) != MA_SUCCESS) {
    throw std::runtime_error("Failed to start audio device");
  }
}

AudioSystem::~AudioSystem() {
  ma_device_uninit(&_device);
}

void AudioSystem::audioCallback(ma_device* device, void* output, const void*, ma_uint32 frameCount) {
  auto* self = static_cast<AudioSystem*>(device->pUserData);
  std::memset(output, 0, sizeof(float) * frameCount * self->_channels);
  self->_voiceManager->mix(static_cast<float*>(output), frameCount, self->_channels);
}

void AudioSystem::play(std::shared_ptr<SoundBuffer> buffer, float gain, bool loop) {
  _voiceManager->play(std::move(buffer), gain, loop);
}

void AudioSystem::fadeOutAll(float durationSeconds) {
  uint32_t durationFrames = static_cast<uint32_t>(durationSeconds * _sampleRate);
  _voiceManager->fadeOutAll(durationFrames);
}

void AudioSystem::update() {
  // no-op for now (streaming/music later)
}
}  // namespace audio
