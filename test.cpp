#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, const char * argv[]) {
    std::ofstream NewTest("test_new.txt", std::ios::binary);
    std::ifstream File1("test1.txt", std::ios::binary);
    unsigned char Byte1, Byte2;
    
    size_t counterr = 0, limit = 500;
    while(counter++ < limit){
        File1.read(reinterpret_cast<char*>(&Byte1), sizeof(Byte1));
        NewTest.write(reinterpret_cast<const char*>(&Byte), sizeof(unsigned char));
    }
    File1.close();
    NewTest.close();
    return 0;
}
