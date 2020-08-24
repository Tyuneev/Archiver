//Архиватор (Huffman + BWT + MTF + RLE)

//1
//1000
//Необходимо реализовать два известных метода сжатия данных для сжатия одного файла.
//
//Формат запуска должен быть аналогичен формату запуска программы gzip, должны быть поддержаны следующие ключи:
//-c выводмим в stdout
//-d распаковка
//-k копирует
//-l
    //Для каждого сжатого файла печатать следующие данные:
    //compressed size: размер сжатого файла
    //uncompressed size: размер оригинального файла
    //ratio: коэффициент сжатия (0.0%, если неизвестен)
    //uncompressed_name: имя оригинального файла
//-r зайдет в каталог и сожмет рекурсивно содержание
//-t проверка целостности сжатого файла
//-1 compress faster
//-9 compress better


#include <iostream>
#include <fstream>
#include <vector>
#include "archiver.hpp"



int main(int argc, const char * argv[]) {
    Archiver A(argc, argv);
    return 0;
}
