#include "storage.h"

Storage *Storage::storage_ = nullptr;

Storage &Storage::GetInstance() {
  if (storage_ == nullptr)
    storage_ = new Storage;

  return *storage_;
}
  
Storage::Storage()
    : hmargin_(160),
      vmargin_(30),
      enemy_size_(30),
      hspace_(20),
      vspace_(20),
      enemy_hspeed_(5.f / 16.f),
      bullet_vspeed_(0.5f),
      screen_width_(800),
      screen_height_(600) {
  // TODO: test the constants  
}

int Storage::hmargin() const {
  return hmargin_;
}

int Storage::vmargin() const {
  return vmargin_;
}

int Storage::enemy_size() const {
  return enemy_size_;
}

int Storage::hspace() const {
  return hspace_;
}

int Storage::vspace() const {
  return vspace_;
}
  
float Storage::enemy_hspeed() const {
  return enemy_hspeed_;
}

float Storage::bullet_vspeed() const {
  return bullet_vspeed_;
}

int Storage::screen_width() const {
  return screen_width_;
}

int Storage::screen_height() const {
  return screen_height_;
}
