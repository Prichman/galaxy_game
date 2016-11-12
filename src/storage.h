#pragma once

#define theStorage Storage::GetInstance()

class Storage {
 public:
  static Storage &GetInstance();
  
  int hmargin()       const;
  int vmargin()       const;
  int enemy_size()    const;
  int hspace()        const;
  int vspace()        const;
  
  float enemy_hspeed()  const;
  float bullet_vspeed() const;

  int screen_width() const;
  int screen_height() const;

  void Accelerate();
  void Slow();
  
 protected: 
  Storage();

 private:
  static Storage *storage_;

  int hmargin_;
  int vmargin_;
  int enemy_size_;
  int hspace_;
  int vspace_;

  float enemy_hspeed_;
  float bullet_vspeed_;

  int screen_width_;
  int screen_height_;
};
