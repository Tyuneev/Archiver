#ifndef huffman_hpp
#define huffman_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <list>
#include <cinttypes>

class HuffmanTreeNode{
public:
    HuffmanTreeNode(HuffmanTreeNode *LeftSon, HuffmanTreeNode *RightSon);
    HuffmanTreeNode(unsigned char byte, size_t Frequency);
    HuffmanTreeNode(bool IsLeftSon, HuffmanTreeNode *Parent);
    HuffmanTreeNode(bool IsLeftSon, HuffmanTreeNode *Parent, unsigned char byte);
    HuffmanTreeNode();
    HuffmanTreeNode *Parent, *LeftSon, *RightSon;
    size_t Frequency;
    unsigned char byte;
    //std::string str;
};

class Huffman{
private:
    std::ifstream &FileIn;
    std::ofstream &FileOut;
    std::queue<unsigned char> Leaves;
    std::vector<bool> CodedTree;
    std::map<unsigned char, std::vector<bool>> Code;
    HuffmanTreeNode *HuffmanTree;
    void MakeTree();
    void FillCodes();
    void FillHuffmanTree();
    void DFS(HuffmanTreeNode *Node, std::vector<bool> &code);
    void DestroyHuffmanTree(HuffmanTreeNode *Node);
    void DebugPrintLeaves();
    void DebugPrintCodedTree();
    void DebugPrintCode();
    void DebugPrintHuffmanTree();
    void DebugPrintNode(HuffmanTreeNode *Node, size_t spase);
public:
    Huffman(std::ifstream &In, std::ofstream &Out);
    void ReverseTransformation();
    void Transformation();
};

#endif /* huffman_hpp */
