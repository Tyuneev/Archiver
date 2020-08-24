//
//  mtf.cpp
//  DA_KP
//
//  Created by Алексей Тюнеев on 05.12.2019.
//  Copyright © 2019 Алексей Тюнеев. All rights reserved.
//

#include "mtf.hpp"

MTF::MTF(size_t BlockSize, std::ifstream &In, std::ofstream &Out) : maxBlockSize(BlockSize), FileIn(In), FileOut(Out){
}

void MTF::FillAlphabet(){
    unsigned char byte = 0;
    do{
        Alphabet.push_back(byte++);
    }while(byte != 0);
}

void MTF::ReverseTransformation(){
    size_t blockSize = 0;
    unsigned char Byte;
    std::vector<unsigned char> Block;
    while(!FileIn.eof()){
        if(FileIn.read(reinterpret_cast<char*>(&Byte), sizeof(Byte))){
            Block.push_back(Byte);
            ++blockSize;
        }
        if(FileIn.eof() || blockSize == maxBlockSize){
            std::vector<unsigned char> mtfReverse = ReverseTransformationBlock(Block);
            for(auto b: mtfReverse){
                FileOut.write(reinterpret_cast<const char*>(&b), sizeof(b));
            }
            Block.clear();
            blockSize = 0;
        }
    }
}

void MTF::Transformation(){
    size_t blockSize = 0;
    unsigned char Byte;
    std::vector<unsigned char> Block;
    while(!FileIn.eof()){
        if(FileIn.read(reinterpret_cast<char*>(&Byte), sizeof(Byte))){
            Block.push_back(Byte);
            ++blockSize;
        }
        if(FileIn.eof() || blockSize == maxBlockSize){
            std::vector<unsigned char> mtf = TransformationBlock(Block);
            for(auto b: mtf){
                FileOut.write(reinterpret_cast<const char*>(&b), sizeof(b));
            }
            
            Block.clear();
            blockSize = 0;
        }
    }
}
std::vector<unsigned char> MTF::TransformationBlock(const std::vector<unsigned char> &Block){
    std::vector<unsigned char> Result;
    FillAlphabet();
    for(unsigned char c: Block){
        auto c_it = find(Alphabet.begin(), Alphabet.end(), c);
        Result.push_back(distance(Alphabet.begin(), c_it));
        Alphabet.erase(c_it);
        Alphabet.insert(Alphabet.begin(), c);
    }
    Alphabet.clear();
    return Result;
}

std::vector<unsigned char> MTF::ReverseTransformationBlock(const std::vector<unsigned char> &Block){
    std::vector<unsigned char> Result;
    FillAlphabet();
    for(unsigned char n: Block){
        auto n_it = Alphabet.begin();
        advance(n_it, n);
        unsigned char tmp_c = *n_it;
        Result.push_back(tmp_c);
        Alphabet.erase(n_it);
        Alphabet.insert(Alphabet.begin(), tmp_c);
    }
    Alphabet.clear();
    return Result;
}
