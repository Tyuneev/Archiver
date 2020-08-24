all: 
	g++ -std=c++11 -O3 -pedantic -lm -o zp rle.cpp bwt.cpp mtf.cpp huffman.cpp archiver.cpp main.cpp

test: 
	g++ -std=c++11 -O3 -pedantic -lm -o test rle.cpp bwt.cpp mtf.cpp huffman.cpp test.cpp

clean:
	rm -f *.o zp test