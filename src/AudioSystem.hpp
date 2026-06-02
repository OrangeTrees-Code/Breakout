#pragma once

#include "Global.hpp"
#include "InitKey.hpp"
#include <memory>
#include <miniaudio.h>
#include <string>
#include <unordered_map>
class AudioSystem {
public:
  static AudioSystem &Instance();
  static void Init(InitKey);
  AudioSystem(const AudioSystem &) = delete;
  AudioSystem &operator=(const AudioSystem &) = delete;
  ~AudioSystem();
  void ShutDown();
  void LoadSound(const std::string &name, const Path &path);
  void LoadMusic(const std::string &name, const Path &path);
  void Play(const std::string &name, bool isMusic = false);
  void Stop(const std::string &name, bool isMusic = false);

private:
  static std::unique_ptr<AudioSystem> instance_ptr_;
  AudioSystem() = default;

private:
  void init_();

private:
  ma_engine engine_;
  std::unordered_map<std::string, std::unique_ptr<ma_sound>> sounds_;
  std::unordered_map<std::string, std::unique_ptr<ma_sound>> musics_;
};
