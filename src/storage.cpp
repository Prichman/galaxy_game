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
      enemy_hspeed_(5.f / 16.f) {
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
  
int Storage::enemy_hspeed() const {
  return enemy_hspeed_;
}
