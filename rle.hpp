#ifndef rle_hpp
#define rle_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>

class RLE{
private:
    std::ifstream &FileIn;
    std::ofstream &FileOut;
    const unsigned char indicator;
public:
    RLE(std::ifstream &In, std::ofstream &Out);
    void ReverseTransformation();
    void Transformation();
};
#endif /* rle_hpp */
