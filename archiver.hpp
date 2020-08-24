#ifndef archiver_hpp
#define archiver_hpp

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

#include <string>
#include "bwt.hpp"
#include "mtf.hpp"
#include "rle.hpp"
#include "huffman.hpp"

class Archiver{
private:

    bool C; // в std::out
    bool D; //-d распаковка
    bool K; //-k копирует
    bool R; //-r зайдет в каталог и сожмет рекурсивно содержание
    bool T; //-t проверка целостности сжатого файла
    bool L; //-l
            //Для каждого сжатого файла печатать следующие данные:
            //compressed size: размер сжатого файла
            //uncompressed size: размер оригинального файла
            //ratio: коэффициент сжатия (0.0%, если неизвестен)
            //uncompressed_name: имя оригинального файла
    char CompressionLevel; //-9 compress better
    std::vector<std::string> Files;
    void RecursiveBypassDirectory(const std::string &Directory);
    char GetNote(const std::string &file);
    void PrintNote(const std::string &file);
    void AddNote(const std::string &file);
    void ToStdout(const std::string &file);
    bool CheckCompressedFile(const std::string &file);
public:
    Archiver(int argc, const char **argv);
};
#endif /* archiver_hpp */
