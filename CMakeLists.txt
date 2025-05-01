cmake_minimum_required(VERSION 3.20)
project(cpp_audio_demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  miniaudio
  GIT_REPOSITORY https://github.com/mackron/miniaudio.git
  GIT_TAG master
)

FetchContent_MakeAvailable(miniaudio)

add_executable(audio_demo
  src/main.cpp
  src/audio/miniaudio_impl.cpp
  src/audio/SoundBuffer.cpp
  src/audio/Voice.cpp
  src/audio/VoiceManager.cpp
  src/audio/AudioSystem.cpp
)

target_include_directories(audio_demo PRIVATE ${miniaudio_SOURCE_DIR})

target_link_libraries(audio_demo
  miniaudio
  pthread
  dl
)