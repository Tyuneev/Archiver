//
//  huffman.cpp
//  DA_KP
//
//  Created by Алексей Тюнеев on 05.12.2019.
//  Copyright © 2019 Алексей Тюнеев. All rights reserved.
//

#include "huffman.hpp"

struct Comparator{
    bool operator()(const HuffmanTreeNode *a, const HuffmanTreeNode *b) const{
        return a->Frequency > b->Frequency;
    }
};

HuffmanTreeNode::HuffmanTreeNode(HuffmanTreeNode *LeftSon, HuffmanTreeNode *RightSon){
    this->LeftSon = LeftSon;
    this->RightSon = RightSon;
    this->Parent = NULL;
    Frequency = LeftSon->Frequency + RightSon->Frequency;
    //str = LeftSon->str + RightSon->str;
    LeftSon->Parent = this;
    RightSon->Parent = this;
}

HuffmanTreeNode::HuffmanTreeNode(unsigned char byte, size_t Frequency){
    this->byte = byte;
    //this->str = byte;
    this->Frequency = Frequency;
    Parent = NULL;
    LeftSon = NULL;
    RightSon = NULL;
}

HuffmanTreeNode::HuffmanTreeNode(bool IsLeftSon, HuffmanTreeNode *Parent){
    this->Parent = Parent;
    this->LeftSon = NULL;
    this->RightSon = NULL;
    //this->str = "-";
    if(IsLeftSon){
        Parent->LeftSon = this;
    }
    else{
        Parent->RightSon = this;
    }
}

HuffmanTreeNode::HuffmanTreeNode(bool IsLeftSon, HuffmanTreeNode *Parent, unsigned char byte){
    this->Parent = Parent;
    this->LeftSon = NULL;
    this->RightSon = NULL;
    if(IsLeftSon){
        Parent->LeftSon = this;
    }
    else{
        Parent->RightSon = this;
    }
    this->byte = byte;
    //this->str = byte;
//    HuffmanTreeNode *CurterntNode = Parent;
//    while(CurterntNode){
//        CurterntNode->str = (CurterntNode->LeftSon ? CurterntNode->LeftSon->str : "") + (CurterntNode->RightSon ? CurterntNode->RightSon->str : "");
//        CurterntNode = CurterntNode->Parent;
//    }
}

HuffmanTreeNode::HuffmanTreeNode(){
    Parent = NULL;
    LeftSon = NULL;
    RightSon = NULL;
    //str = "-";
}
Huffman::Huffman(std::ifstream &In, std::ofstream &Out) : FileIn(In), FileOut(Out){}

void Huffman::FillHuffmanTree(){
    std::map<unsigned char, size_t> Frequency;
    unsigned char Byte;
    while(FileIn.read(reinterpret_cast<char*>(&Byte), sizeof(Byte))){
        if(Frequency.find(Byte) == Frequency.end()){
            Frequency.insert(std::make_pair(Byte, 1));
        }
        else{
            ++Frequency[Byte];
        }
    }
    if(Frequency.size() == 0){
        HuffmanTree = NULL;
        return;
    }
    FileIn.clear();
    FileIn.seekg(0);
    std::priority_queue<HuffmanTreeNode*, std::vector<HuffmanTreeNode*>,  Comparator> Queue;
    if (Frequency.size() < 2){
        Queue.push(new HuffmanTreeNode((unsigned char)0, 0));
    }
    for(auto f: Frequency){
        Queue.push(new HuffmanTreeNode(f.first, f.second));
    }
    while(Queue.size() > 1){
        HuffmanTreeNode * RightSon = Queue.top();
        Queue.pop();
        HuffmanTreeNode * LeftSon = Queue.top();
        Queue.pop();
        Queue.push(new HuffmanTreeNode(LeftSon, RightSon));
    }
    HuffmanTree = (HuffmanTreeNode*) Queue.top();
}

void Huffman::FillCodes(){
    std::vector<bool> code;
    DFS(HuffmanTree, code);
}

void Huffman::DFS(HuffmanTreeNode *Node, std::vector<bool> &code){
    if(Node->Parent){
        CodedTree.push_back((Node->Parent->LeftSon == Node) ? 0 : 1);
    }
    if(Node->LeftSon){
        code.push_back(0);
        DFS(Node->LeftSon, code);
    }
    if(Node->RightSon){
        code.push_back(1);
        DFS(Node->RightSon, code);
    }
    if(Node == HuffmanTree){
        CodedTree.push_back(1);
    }
    if(!(Node->RightSon) && !(Node->LeftSon)){
        Leaves.push(Node->byte);
        Code.insert(std::make_pair(Node->byte, code));
    }
    code.pop_back();
}

void Huffman::Transformation(){
    FillHuffmanTree();
    if(HuffmanTree){
        FillCodes();
    }
    else{
        return;
    }
    unsigned char LeavesNumber = (unsigned char)(Leaves.size() - 1);
    FileOut.write(reinterpret_cast<const char*>(&LeavesNumber), sizeof(unsigned char));
    while(!Leaves.empty()){
        FileOut.write(reinterpret_cast<const char*>(&(Leaves.front())), sizeof(unsigned char));
        Leaves.pop();
    }
    unsigned char counter = 0, Byte = 0, Buffer = 0;
    unsigned short int CodedTreeeBitsNumber = CodedTree.size();
     FileOut.write(reinterpret_cast<const char*>(&CodedTreeeBitsNumber), sizeof(CodedTreeeBitsNumber));
    std::ios::pos_type pos = FileOut.tellp();
    FileOut.write(reinterpret_cast<const char*>(&Buffer), sizeof(Buffer));
    for(auto b: CodedTree){
        ++counter;
        if(b){
            Buffer |= ((unsigned char)1 << (8 - counter));
        }
        if(counter == 8){
            FileOut.write(reinterpret_cast<const char*>(&Buffer), sizeof(Buffer));
            counter = 0;
            Buffer = 0;
        }
    }
    while(FileIn.read(reinterpret_cast<char*>(&Byte), sizeof(Byte))){
        for(auto b: Code[Byte]){
            ++counter;
            if(b){
                Buffer |= ((unsigned char) 1) << (8 - counter);
            }
            if(counter == 8){
                FileOut.write(reinterpret_cast<const char*>(&Buffer), sizeof(Buffer));
                counter = 0;
                Buffer = 0;
            }
        }
    }
    if(counter != 0){
        counter = 8 - counter;
        FileOut.write(reinterpret_cast<const char*>(&Buffer), sizeof(Buffer));
        FileOut.seekp(pos);
        FileOut.write(reinterpret_cast<const char*>(&counter), sizeof(counter));
    }
}

void Huffman::ReverseTransformation(){
    FileIn.seekg(0, std::ios::end);
    std::ios::pos_type LastByte = FileIn.tellg();
    FileIn.seekg(0, std::ios::beg);
    unsigned char LeavsNumber = 0;
    if(!FileIn.read(reinterpret_cast<char *>(&LeavsNumber),  sizeof(LeavsNumber))){
        return;
    }
    unsigned char ExtraBits, Buffer = 0, counter = 8;
    do{
        FileIn.read(reinterpret_cast<char *>(&Buffer), sizeof(Buffer));
        Leaves.push(Buffer);
    } while (LeavsNumber-- != 0);
    
    unsigned short int CodedTreeeBitsNumber;
    FileIn.read(reinterpret_cast<char*>(&CodedTreeeBitsNumber), sizeof(CodedTreeeBitsNumber));
    FileIn.read(reinterpret_cast<char *>(&ExtraBits), sizeof(ExtraBits));
    for (size_t i = 0; i < CodedTreeeBitsNumber; i++){
        if(counter == 8){
            FileIn.read(reinterpret_cast<char *>(&Buffer), sizeof(Buffer));
            counter = 0;
        }
        ++counter;
        CodedTree.push_back((Buffer) & ((unsigned char)1 << (8 - counter)));
    }
    MakeTree();
    HuffmanTreeNode *CurrentNode = HuffmanTree;
    while(!(FileIn.tellg() == LastByte && ExtraBits == (unsigned char)(8 - counter))){
        if(counter == 8){
            if (!FileIn.read(reinterpret_cast<char *>(&Buffer), sizeof(Buffer))){
                break;
            }
            counter = 0;
        }
        ++counter;
        if(CurrentNode){
            CurrentNode = ((Buffer) & ((unsigned char)1 << (8 - counter))) ? CurrentNode->RightSon : CurrentNode->LeftSon;
            if(CurrentNode && (CurrentNode->LeftSon == NULL && CurrentNode->RightSon == NULL)){
                FileOut.write(reinterpret_cast<const char *>(&(CurrentNode->byte)), sizeof(CurrentNode->byte));
                CurrentNode = HuffmanTree;
            }
        }
    }
}

void Huffman::MakeTree(){
    size_t i = 0;
    HuffmanTree = new HuffmanTreeNode();
    HuffmanTreeNode *CurrentNode = HuffmanTree;
    bool IsLeftSon = 0, FromLeftSon = 0;
    while (i < CodedTree.size() - 1) {
        IsLeftSon = !CodedTree[i++];
        if(!IsLeftSon){
            do{
                FromLeftSon = (CurrentNode == (CurrentNode->Parent->LeftSon));
                CurrentNode = CurrentNode->Parent;
            }while(!FromLeftSon);
            IsLeftSon = 0;
        }
        if(i < CodedTree.size() && CodedTree[i]){
            CurrentNode = new HuffmanTreeNode(IsLeftSon, CurrentNode, Leaves.front());
            Leaves.pop();
        }
        else{
            CurrentNode = new HuffmanTreeNode(IsLeftSon, CurrentNode);
        }
    }
}

void Huffman::DestroyHuffmanTree(HuffmanTreeNode *Node){
    if(Node->LeftSon != NULL){
        DestroyHuffmanTree(Node->LeftSon);
    }
    if(Node->RightSon != NULL){
        DestroyHuffmanTree(Node->RightSon);
    }
    delete Node;
}

void Huffman::DebugPrintLeaves(){
    std::queue<unsigned char> tmpLeaves = Leaves;
    unsigned char c;
    std::cout << "Leaves: ";
    while (!tmpLeaves.empty())
    {
        c = tmpLeaves.front();
        std::cout << c << " ";
        tmpLeaves.pop();
    }
    std::cout << std::endl;
}
void Huffman::DebugPrintCodedTree(){
    std::cout << "CodedTree: ";
    for(auto b: CodedTree){
        std::cout << (b ? "U" : "D");
    }
    std::cout << std::endl;
}

void Huffman::DebugPrintCode(){
    std::cout << "\nCodes: " << std::endl;
    for(auto C: Code){
        std::cout << C.first << " : ";
        for(auto b: C.second){
            std::cout << b;
        }
        std::cout << std::endl;
    }
}

void Huffman::DebugPrintHuffmanTree(){
    std::cout << "-------------Tree-------------" << std::endl;
    DebugPrintNode(HuffmanTree, 0);
    std::cout << "\n------------------------------" << std::endl;
}

void Huffman::DebugPrintNode(HuffmanTreeNode *Node, size_t spase){
    std::cout << std::endl;
    for(int i = 0; i < spase; i++){
        std::cout << "\t";
    }
   // std::cout << Node->str;
    
    if(Node->LeftSon){
        DebugPrintNode(Node->LeftSon, spase + 1);
    }
    if(Node->RightSon){
        DebugPrintNode(Node->RightSon, spase + 1);
    }
}
