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
  
  int enemy_hspeed()  const;
  
 protected: 
  Storage();

 private:
  static Storage *storage_;

  int hmargin_;
  int vmargin_;
  int enemy_size_;
  int hspace_;
  int vspace_;

  int enemy_hspeed_;
};
