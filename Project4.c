#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "C:/Users/aksha/OneDrive/Desktop/Third Semester/DSA.1/input1.txt"   // Define the input file path
#define OUTPUT_FILE "C:/Users/aksha/OneDrive/Desktop/Third Semester/DSA.1/output.huff" 

typedef struct Node {
    char data;
    int freq;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(char data, int freq) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->left = NULL;
    temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

void printNodes(Node* node, char* code, int top) {
    if (node->left) {
        code[top] = '0';
        printNodes(node->left, code, top + 1);
    }

    if (node->right) {
        code[top] = '1';
        printNodes(node->right, code, top + 1);
    }

    if (!(node->left) && !(node->right)) {
        printf("%c: ", node->data);
        for (int i = 0; i < top; i++) {
            printf("%c", code[i]);
        }
        printf("\n");
    }
}

void buildHuffmanTree(Node* nodes[], int n) {
    while (n > 1) {
        Node* left = nodes[n - 1];
        Node* right = nodes[n - 2];

        Node* parent = createNode('$', left->freq + right->freq);
        parent->left = left;
        parent->right = right;

        int i = n - 3;
        while (i >= 0 && nodes[i]->freq < parent->freq) {
            nodes[i + 1] = nodes[i];
            i--;
        }

        nodes[i + 1] = parent;
        n--;
    }
}

void encodeHuffman(Node* root, char* code, char data, int top) {
    if (root->data == data) {
        code[top] = '\0';
        return;
    }

    if (root->left) {
        code[top] = '0';
        encodeHuffman(root->left, code, data, top + 1);
    }

    if (root->right) {
        code[top] = '1';
        encodeHuffman(root->right, code, data, top + 1);
    }
}

void compressHuffman(const char* inputFilename, const char* outputFilename) {
    FILE* input = fopen(inputFilename, "r");
    if (input == NULL) {
        printf("Error opening the input file.\n");
        return;
    }

    int char_freq[256] = { 0 };
    char c;
    while ((c = fgetc(input)) != EOF) {
        char_freq[c]++;
    }

    int n = 0;
    Node* nodes[256];
    for (int i = 0; i < 256; i++) {
        if (char_freq[i] > 0) {
            nodes[n] = createNode((char)i, char_freq[i]);
            n++;
        }
    }

    buildHuffmanTree(nodes, n);

    Node* root = nodes[0];

    char code[256];
    char data;
    int top = 0;

    FILE* output = fopen(outputFilename, "wb");
    if (output == NULL) {
        printf("Error opening the output file.\n");
        fclose(input);
        return;
    }

    fseek(input, 0, SEEK_SET);

    while ((c = fgetc(input)) != EOF) {
        data = (char)c;
        encodeHuffman(root, code, data, top);
        fprintf(output, "%s", code);
    }

    fclose(input);
    fclose(output);
}

long getFileSize(FILE* file) {
    long size;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

void printFileSize(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    long size = getFileSize(file);
    printf("%s size: %ld bytes\n", filename, size);

    fclose(file);
}


int main() {

    const char* file_path = INPUT_FILE ; // Specify the file path here

    FILE* file = fopen(file_path, "r");

    if (file == NULL) {
        perror("Failed to open the file");
        return 1;
    }

    int charCount = 0;
    int charFrequency[256] = {0}; // Assuming ASCII characters

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ferror(file)) {
            perror("Error reading from the file");
            fclose(file);
            return 1;
        }

        charCount++;
        charFrequency[ch]++;
    }

    if (fclose(file) == EOF) {
        perror("Error closing the file");
        return 1;
    }

    printf("Total characters: %d\n", charCount);
    printf("Character frequencies:\n");

    for (int i = 0; i < 256; i++) {
        if (charFrequency[i] > 0) {
            printf("Character '%c' (ASCII %d): %d\n", (char)i, i, charFrequency[i]);
        }
    }


    printf("Huffman Compression\n");

    compressHuffman(INPUT_FILE, OUTPUT_FILE);

    printf("The file has been compressed and saved as %s\n", OUTPUT_FILE);

    printFileSize(INPUT_FILE);
    printFileSize(OUTPUT_FILE);
    return 0;
}
