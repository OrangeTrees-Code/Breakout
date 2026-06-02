#include "AudioSystem.hpp"

std::unique_ptr<AudioSystem> AudioSystem::instance_ptr_ = nullptr;

AudioSystem &AudioSystem::Instance() {
  if (!instance_ptr_) {
    throw std::runtime_error("AudioSystem has not been initialized");
  }
  return *instance_ptr_;
}
void AudioSystem::Init(InitKey) {
  instance_ptr_ = std::unique_ptr<AudioSystem>(new AudioSystem());
  instance_ptr_->init_();
}

void AudioSystem::init_() {
  if (ma_engine_init(NULL, &engine_) != MA_SUCCESS) {
    throw std::runtime_error("AudioSystem initialized failed");
  }
}

void AudioSystem::ShutDown() {
  for (auto &[_, sound] : sounds_) {
    ma_sound_uninit(sound.get());
  }

  for (auto &[_, music] : musics_) {
    ma_sound_uninit(music.get());
  }

  sounds_.clear();
  musics_.clear();
  ma_engine_uninit(&engine_);
}

void AudioSystem::LoadSound(const std::string &name, const Path &path) {
  auto sound = std::make_unique<ma_sound>();

  if (ma_sound_init_from_file(
          &engine_,
          path.c_str(),
          0,
          nullptr,
          nullptr,
          sound.get()) != MA_SUCCESS) {
    throw std::runtime_error("name load filed");
  }

  sounds_[name] = std::move(sound);
}

void AudioSystem::LoadMusic(const std::string &name, const Path &path) {
  auto music = std::make_unique<ma_sound>();

  if (ma_sound_init_from_file(
          &engine_,
          path.c_str(),
          MA_SOUND_FLAG_STREAM,
          nullptr,
          nullptr,
          music.get()) != MA_SUCCESS) {
    throw std::runtime_error("name load filed");
  }

  ma_sound_set_looping(music.get(), MA_TRUE);

  musics_[name] = std::move(music);
}

void AudioSystem::Play(const std::string &name, bool isMusic) {
  if (isMusic) {
    auto it = musics_.find(name);
    if (it == musics_.end()) {
      return;
    }
    ma_sound_seek_to_pcm_frame(it->second.get(), 0);
    ma_sound_start(it->second.get());
  } else {
    auto it = sounds_.find(name);
    if (it == sounds_.end()) {
      return;
    }
    ma_sound_seek_to_pcm_frame(it->second.get(), 0);
    ma_sound_start(it->second.get());
  }
}

void AudioSystem::Stop(const std::string &name, bool isMusic) {
  if (isMusic) {
    auto it = musics_.find(name);
    if (it == musics_.end()) {
      return;
    }
    ma_sound_stop(it->second.get());

  } else {
    auto it = sounds_.find(name);
    if (it == sounds_.end()) {
      return;
    }
    ma_sound_stop(it->second.get());
  }
}

AudioSystem::~AudioSystem() {
  ShutDown();
}
