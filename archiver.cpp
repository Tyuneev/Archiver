#include "archiver.hpp"

Archiver::Archiver(int argc, const char **argv){
    C = D = K = L = R = T = 0;
    CompressionLevel  = 9;
    for(size_t i = 1; i < argc; i++){
        if(argv[i][0] == '-'){
            switch(argv[i][1]){
                default:
                    CompressionLevel = std::atoi(&(argv[i][1]));
                    if(CompressionLevel > 9 && CompressionLevel < 1){
                        CompressionLevel = 9;
                        Files.push_back(argv[i]);
                        if(R){
                            RecursiveBypassDirectory(argv[i]);
                        }
                        else{
                            Files.push_back(argv[i]);
                        }
                    }
                    break;
                case 'c':
                    C = 1;
                    break;
                case 'd':
                    D = 1;
                    break;
                case 'k':
                    K = 1;
                    break;
                case 'l':
                    L = 1;
                    break;
                case 'r':
                    R = 1;
                    break;
                case 't':
                    T = 1;
                    break;
                case '1':
                    break;
            }
        }
        else if(!R){
            Files.push_back(argv[i]);
        }
        else{
            RecursiveBypassDirectory(argv[i]);
        }
    }
    for(auto file: Files){
        if(L){
            PrintNote(file);
        }
        else if(D){
            char CompresType = GetNote(file);
            size_t BlockSize = 20 * static_cast<size_t>(CompresType);
            std::ifstream File(file, std::ios::binary);
            std::ofstream File1(file + ".rhuff", std::ios::binary);
            Huffman huffman(File, File1);
            huffman.ReverseTransformation();
            File.close();
            File1.close();
            remove(file.c_str());

            std::ifstream File2(file + ".rhuff", std::ios::binary);
            std::ofstream File3(file + ".rrle", std::ios::binary);
            RLE rle2(File2, File3);
            rle2.ReverseTransformation();
            File2.close();
            File3.close();
            remove((file + ".rhuff").c_str());

            std::ifstream File4(file + ".rrle", std::ios::binary);
            std::ofstream File5(file + ".rmtf", std::ios::binary);
            MTF mtf(BlockSize, File4, File5);
            mtf.ReverseTransformation();
            File4.close();
            File5.close();
            remove((file + ".rrle").c_str());

            std::ifstream File6(file + ".rmtf", std::ios::binary);
            std::ofstream File7(file + ".rbwt", std::ios::binary);
            BWT bwt(BlockSize, File6, File7);
            bwt.ReverseTransformation();
            File6.close();
            File7.close();
            remove((file + ".rmtf").c_str());

            std::ifstream File8(file + ".rbwt", std::ios::binary);
            std::ofstream File9(file + ".rrle", std::ios::binary);
            RLE rle(File8, File9);
            rle.ReverseTransformation();
            File8.close();
            File9.close();
            remove((file + ".rbwt").c_str());
            rename((file + ".rrle").c_str(), (file.substr(0, file.length() - 3)).c_str());
        }
        else{
            size_t BlockSize = 20 * (static_cast<size_t>(CompressionLevel));
            std::ifstream File(file, std::ios::binary);
            std::ofstream File1(file + ".rle", std::ios::binary);
            RLE rle(File, File1);
            rle.Transformation();
            File.clear();
            File.close();
            File1.close();

            std::ifstream File2(file + ".rle", std::ios::binary);
            std::ofstream File3(file + ".bwt", std::ios::binary);
            BWT bwt(BlockSize, File2, File3);
            bwt.Transformation();
            File2.close();
            File3.close();
            remove((file + ".rle").c_str());

            std::ifstream File4(file + ".bwt", std::ios::binary);
            std::ofstream File5(file + ".mtf", std::ios::binary);
            MTF mtf(BlockSize, File4, File5);
            mtf.Transformation();
            File4.close();
            File5.close();
            remove((file + ".bwt").c_str());

            std::ifstream File6(file + ".mtf", std::ios::binary);
            std::ofstream File7(file + ".rle", std::ios::binary);
            RLE rle2(File6, File7);
            rle2.Transformation();
            File6.close();
            File7.close();
            remove((file + ".mtf").c_str());

            std::ifstream File8(file + ".rle", std::ios::binary);
            std::ofstream File9(file + ".huff", std::ios::binary);
            Huffman huffmun(File8, File9);
            huffmun.Transformation();
            File8.close();
            File9.close();

            remove((file + ".rle").c_str());
            rename((file + ".huff").c_str(), (file + ".zp").c_str());
            if(T){
                std::cout << (CheckCompressedFile(file) ? "Ok" : "Fail") << std::endl;
            }
            AddNote(file);
            if(!K || C){
                remove((file).c_str());
            }
        }
    }
}

bool Archiver::CheckCompressedFile(const std::string &file){
    //пробная разархивация и сравнение с оригиналом
    return 1;
}

void Archiver::PrintNote(const std::string &file){
    std::ifstream File(file, std::ios::binary);
    unsigned short int UncompressSize = 0, CompressSize = 0;
    struct stat stat_buf;
    stat(file.c_str(), &stat_buf);
    CompressSize = stat_buf.st_size;
    char type;
    if(File.read(&type, sizeof(type) && File.read(reinterpret_cast<char*>(&UncompressSize), sizeof(UncompressSize)))){
    	//UncompressSize биты переставляются, исправить
        double ratio = (UncompressSize / CompressSize)*100; 
        std::cout << "compressed size: " << CompressSize << " B" << std::endl;
        std::cout << "uncompressed size: " << UncompressSize << " B" << std::endl;
        std::cout << "ratio: " << std::fixed << ratio << "%" << std::endl;
        std::cout << "uncompressed_name: " << file.substr(0, file.length() - 3) << std::endl;
    }
    else{
        std::cout << "Error" << std::endl;
    }
    File.clear();
    File.close();
}

char Archiver::GetNote(const std::string &file){
    std::ifstream File(file, std::ios::binary);
    std::ofstream FileTmp(file + ".tmp", std::ios::binary);
    char type = 0;
    File.read(&type, sizeof(type));
    File.seekg(sizeof(char) + sizeof(unsigned short int));
    char buffer[256];
    do{
        File.read(&buffer[0], 256);
        FileTmp.write(&buffer[0], File.gcount());
    }while(File.gcount() > 0);
    File.close();
    FileTmp.close();
    remove(file.c_str());
    rename((file + ".tmp").c_str(), file.c_str());
    return type;
}

void Archiver::AddNote(const std::string &file){
    std::ifstream File(file, std::ios::binary);
    struct stat stat_buf;
    stat(file.c_str(), &stat_buf);
    unsigned short int FileSize = stat_buf.st_size;
    //std::cout << "FileSize " << FileSize << std::endl;
    std::ifstream FileCompressed(file + ".zp", std::ios::binary);
    std::ofstream FileTmp(file + ".zp.tmp", std::ios::binary);
    FileTmp.write(&CompressionLevel, sizeof(CompressionLevel));
	FileTmp.write(reinterpret_cast<char*>(&FileSize), sizeof(FileSize));
    char buffer[256];
    do{
        FileCompressed.read(&buffer[0], 256);
        FileTmp.write(&buffer[0], FileCompressed.gcount());
    }while(FileCompressed.gcount() > 0);
    File.close();
    FileTmp.clear();
    FileTmp.close();
    FileCompressed.close();
    remove((file + ".zp").c_str());
    rename((file + ".zp.tmp").c_str(), (file + ".zp").c_str());
}

void Archiver::RecursiveBypassDirectory(const std::string &Directory){
    //рекурсивный проход по дирректории
}

void Archiver::ToStdout(const std::string &file){
    std::ifstream File(file + ".zp", std::ios::binary);
    char bufer;
    while(File.read(&bufer, sizeof(bufer))){
        std::cout << bufer;
    }
    std::cout << std::endl;
    File.close();
    remove((file + ".zp").c_str());
}
