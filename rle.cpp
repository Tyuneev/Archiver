//
//  rle.cpp
//  DA_KP
//
//  Created by Алексей Тюнеев on 05.12.2019.
//  Copyright © 2019 Алексей Тюнеев. All rights reserved.
//

#include "rle.hpp"
RLE::RLE(std::ifstream &In, std::ofstream &Out) : FileIn(In), FileOut(Out), indicator(255){}


void RLE::ReverseTransformation(){
    unsigned char Byte;
    while(FileIn.read(reinterpret_cast<char*>(&Byte), sizeof(Byte))){
        if(Byte == indicator){
            unsigned char counter;
            FileIn.read(reinterpret_cast<char*>(&counter), sizeof(counter));
            FileIn.read(reinterpret_cast<char*>(&Byte), sizeof(Byte));
            do{
                FileOut.write(reinterpret_cast<const char*>(&Byte), sizeof(Byte));
            } while(counter-- != 0);
        }
        else{
            FileOut.write(reinterpret_cast<const char*>(&Byte), sizeof(Byte));
        }
    }
}

void RLE::Transformation(){
    unsigned char oldByte = indicator, Byte = 0, counter = (unsigned char)0;
    if(!FileIn.read(reinterpret_cast<char*>(&oldByte), sizeof(oldByte))){
        return;
    }
    do{
        if(!FileIn.read(reinterpret_cast<char*>(&Byte), sizeof(Byte)) || Byte != oldByte || counter == 255){
            if(counter != 0 || indicator == oldByte){
                FileOut.write(reinterpret_cast<const char*>(&indicator), sizeof(indicator));
                FileOut.write(reinterpret_cast<const char*>(&counter), sizeof(counter));
                FileOut.write(reinterpret_cast<const char*>(&oldByte), sizeof(oldByte));
                counter = 0;
            }
            else if(oldByte != indicator){
                FileOut.write(reinterpret_cast<const char*>(&oldByte), sizeof(oldByte));
            }
        }
        else{
            ++counter;
        }
        oldByte = Byte;
    } while(!FileIn.eof());
}
