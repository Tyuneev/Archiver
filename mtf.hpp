#ifndef mtf_hpp
#define mtf_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>

class MTF{
private:
    const size_t maxBlockSize;
    std::ifstream &FileIn;
    std::ofstream &FileOut;
    std::vector<unsigned char> TransformationBlock(const std::vector<unsigned char> &Block);
    std::vector<unsigned char> ReverseTransformationBlock(const std::vector<unsigned char> &Block);
    std::list<unsigned char> Alphabet;
    void FillAlphabet();
public:
    MTF(size_t BlockSize, std::ifstream &In, std::ofstream &Out);
    void ReverseTransformation();
    void Transformation();
};
#endif /* mtf_hpp */
