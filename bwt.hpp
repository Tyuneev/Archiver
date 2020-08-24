#ifndef bwt_hpp
#define bwt_hpp

#include <stdio.h>
#include <fstream>
#include <list>
#include <vector>
#include <map>

class BWT{
private:
    const size_t maxBlockSize;
    std::ifstream &FileIn;
    std::ofstream &FileOut;
    std::vector<unsigned char> TransformationBlock(const std::vector<unsigned char> &Block);
    std::vector<unsigned char> ReverseTransformationBlock(const std::vector<unsigned char> &Block);
public:
    BWT();
    BWT(size_t BlockSize, std::ifstream &In, std::ofstream &Out);
    void ReverseTransformation();
    void Transformation();
};
#endif /* bwt_hpp */
