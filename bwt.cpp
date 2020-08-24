//
//  bwt.cpp
//  DA_KP
//
//  Created by Алексей Тюнеев on 05.12.2019.
//  Copyright © 2019 Алексей Тюнеев. All rights reserved.
//

#include "bwt.hpp"
#include <iostream>


bool Comparator(const std::vector<unsigned char> a, const std::vector<unsigned char> b){
    for(size_t i = a.size() - 1; i >= 0; i--){
        if(a[i] != b[i]){
            return a[i] < b[i];
        }
    }
    return 0;
}

size_t CharToSize_t(const unsigned char &c){ //костыль, избавиться
    size_t Result = 0;
    const size_t size_t_byte = 1;
    const size_t char_byte = 1;
    for(size_t i = 0; i < 8; i++){
        if((c & (char_byte << i)) != 0){
            Result |= (size_t_byte << i);
        }
    }
    return Result;
}

BWT::BWT(size_t BlockSize, std::ifstream &In, std::ofstream &Out) : maxBlockSize(BlockSize), FileIn(In), FileOut(Out){}

void BWT::ReverseTransformation(){
    size_t blockSize = 0;
    unsigned char Byte;
    std::vector<unsigned char> Block;
    while(!FileIn.eof()){
        if(FileIn.read(reinterpret_cast<char*>(&Byte), sizeof(Byte))){
            Block.push_back(Byte);
            ++blockSize;
        }
        if((FileIn.eof() || blockSize == maxBlockSize + 1) && (Block.size() > 0)){
            std::vector<unsigned char> bwtReverse = ReverseTransformationBlock(Block);
            for(auto b: bwtReverse){
                FileOut.write(reinterpret_cast<const char*>(&b), sizeof(b));
            }
            Block.clear();
            blockSize = 0;
        }
    }
}

void BWT::Transformation(){
    size_t blockSize = 0;
    unsigned char Byte;
    std::vector<unsigned char> Block;
    while(!FileIn.eof()){
        if(FileIn.read(reinterpret_cast<char*>(&Byte), sizeof(Byte))){
            Block.push_back(Byte);
            ++blockSize;
        }
        if(FileIn.eof() || blockSize == maxBlockSize){
            std::vector<unsigned char> bwt = TransformationBlock(Block);
            for(auto b: bwt){
                FileOut.write(reinterpret_cast<const char*>(&b), sizeof(b));
            }
            Block.clear();
            blockSize = 0;
        }
    }
}

std::vector<unsigned char> BWT::TransformationBlock(const std::vector<unsigned char> &Block){
    std::list<std::vector<unsigned char>> Shifts;
    std::vector<unsigned char> result;
    for(auto i = Block.begin(); i != Block.end(); i++){
        std::vector<unsigned char> Shift;
        Shift.insert(Shift.begin(), i, Block.end());
        Shift.insert(Shift.begin() + distance(i, Block.end()), Block.begin(), i);
        Shifts.push_back(Shift);
        Shifts.sort(Comparator);
    }
    for(auto s: Shifts){
        result.push_back(s.front());
    }
    if(Block.size() != 0){
        size_t position = distance(Shifts.begin(), find(Shifts.begin(), Shifts.end(), Block));
        result.push_back(static_cast<unsigned char>(position));
        Shifts.clear();
    }
    return result;
}

std::vector<unsigned char> BWT::ReverseTransformationBlock(const std::vector<unsigned char> &Block){
    std::vector<unsigned char> result;
    size_t position = CharToSize_t(Block.back());
    std::vector<size_t> L(Block.size() - 1);
    std::map<unsigned char, size_t> K;
    std::map<unsigned char, size_t> M;
    for(auto b = Block.begin(); b != Block.end() - 1; b++){
        if(K.find(*b) == K.end()){
            K.insert(std::make_pair(*b, 1));
            M.insert(std::make_pair(*b, 1));
        }
        else{
            ++K[*b];
        }
    }
    for(auto i = ++K.begin(); i != K.end(); i++){
        auto j = i;
        --j;
        M[i->first] = j->second + M[j->first];
    }
    for(size_t i = 0; i < Block.size() - 1; i++){
        L[i] = M[Block[i]]++;
    }
    size_t k = position + 1;
    for(size_t i = 0; i < Block.size() - 1; i++){
        result.push_back(Block[k - 1]);
        k = L[k - 1];
    }
    return result;
}
