#ifndef HUFFMAN_H
#define HUFFMAN_H

#define MAX_CHAR 256

struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

// Fonction declarations
struct MinHeapNode* newNode(char data, unsigned freq);
void compressFile(const char* inputFile, const char* outputFile);
void decompressFile(const char* inputFile, const char* outputFile);
struct MinHeapNode* buildTreeFromCodes(char codes[MAX_CHAR][MAX_CHAR], int freq[MAX_CHAR]);
long getFileSize(const char* filename);


#endif

