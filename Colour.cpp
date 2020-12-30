#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

struct Colour{
  public:
    std::string hex_;
    std::int16_t r_, g_, b_;

  public:
    Colour(std::string col){
      hex_ = col;
    }
    Colour(const short &R, const short &G, const short &B){
      r_ = R;
      g_ = G;
      b_ = B;
    }
    void to_rgb(){
      r_ = std::stoi(hex_.substr(1, 2), 0, 16);
      g_ = std::stoi(hex_.substr(3, 2), 0, 16);
      b_ = std::stoi(hex_.substr(5, 2), 0, 16);
    }
    void to_hex(){
      hex_.erase();
      std::stringstream ss(hex_);
      ss << "#" << std::hex << (r_ << 16 | g_ << 8 | b_);
      hex_ = ss.str();
    }
    void print_rgb(){ std::cout << r_ << " " << g_ << " " << b_ << std::endl;}
    void print_hex(){ std::cout << hex_ << std::endl;}
};

void save_PPM(const std::string &filename, int WIDTH, int HEIGHT){
  std::fstream file(filename, std::fstream::out);
  file << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";

  for (short i=0;i<HEIGHT; i++){
    for (short j=0;j<WIDTH; j++)
      file << (char) (i % 256) << (char) (j % 256) << (char) (0);
  }

  file.close();
}


//int main(){
//
//  std::string code = "#AAFF00";
//
//  Colour col(code);
//  col.to_rgb();
//  col.print_rgb();
//
//  Colour r(255, 0, 0);
//  r.to_hex();
//  r.print_hex();
//
//  const int WIDTH = 640;
//  const int HEIGHT= 480;
//
//  std::string s = "Image.ppm";
//  save_PPM(s, WIDTH, HEIGHT);
//
//  return 0;
//}
