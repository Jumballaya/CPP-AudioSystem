#include <miniaudio.h>

#include <cstring>
#include <iostream>
#include <thread>

#include "audio/audio.hpp"

int main() {
  audio::AudioSystem audio;
  auto buffer = audio::SoundBuffer::loadWav("assets/acid-wool-cloth.wav");

  std::cout << "Looping sound. Press Enter to play one-shot. Type 'fade' to fade out.\n";

  audio.play(buffer, 0.7f, true);

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line == "fade") {
      audio.fadeOutAll(1.0f);  // 1 second fade
    } else {
      audio.play(buffer);  // one-shot
    }
  }

  return 0;
}