#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <bitset>
#include <vector>

using namespace std;

class HuffmanCoding {
private:
    struct HeapNode {
        char ch;
        int freq;
        HeapNode* left;
        HeapNode* right;

        HeapNode(char ch, int freq) {
            this->ch = ch;
            this->freq = freq;
            left = right = nullptr;
        }
    };

    struct compare {
        bool operator()(HeapNode* l, HeapNode* r) {
            return l->freq > r->freq;
        }
    };

    string path;
    unordered_map<char, string> codes;
    unordered_map<string, char> reverseMapping;
    priority_queue<HeapNode*, vector<HeapNode*>, compare> minHeap;

    // Function to generate frequency map
    unordered_map<char, int> makeFrequencyDict(const string& text) {
        unordered_map<char, int> frequency;
        for (char ch : text) {
            frequency[ch]++;
        }
        return frequency;
    }

    // Function to build the heap
    void makeHeap(const unordered_map<char, int>& frequency) {
        for (auto pair : frequency) {
            HeapNode* node = new HeapNode(pair.first, pair.second);
            minHeap.push(node);
        }
    }

    // Merge nodes to build the Huffman Tree
    void mergeNodes() {
        while (minHeap.size() > 1) {
            HeapNode* left = minHeap.top();
            minHeap.pop();
            HeapNode* right = minHeap.top();
            minHeap.pop();

            HeapNode* merged = new HeapNode('\0', left->freq + right->freq);
            merged->left = left;
            merged->right = right;
            minHeap.push(merged);
        }
    }

    // Recursive function to generate Huffman codes
    void makeCodesHelper(HeapNode* root, string currentCode) {
        if (!root) return;

        if (root->ch != '\0') {
            codes[root->ch] = currentCode;
            reverseMapping[currentCode] = root->ch;
        }

        makeCodesHelper(root->left, currentCode + "0");
        makeCodesHelper(root->right, currentCode + "1");
    }

    // Wrapper to generate codes
    void makeCodes() {
        HeapNode* root = minHeap.top();
        makeCodesHelper(root, "");
    }

    // Get encoded text using Huffman codes
    string getEncodedText(const string& text) {
        string encodedText = "";
        for (char ch : text) {
            encodedText += codes[ch];
        }
        return encodedText;
    }

    // Add padding to encoded text
    string padEncodedText(string encodedText) {
        int extraPadding = 8 - encodedText.length() % 8;
        for (int i = 0; i < extraPadding; ++i) {
            encodedText += "0";
        }

        string paddedInfo = bitset<8>(extraPadding).to_string();
        encodedText = paddedInfo + encodedText;
        return encodedText;
    }

    // Convert the padded encoded text to byte array
    vector<unsigned char> getByteArray(const string& paddedEncodedText) {
        vector<unsigned char> byteArray;
        for (size_t i = 0; i < paddedEncodedText.length(); i += 8) {
            byteArray.push_back(static_cast<unsigned char>(bitset<8>(paddedEncodedText.substr(i, 8)).to_ulong()));
        }
        return byteArray;
    }

    // Remove padding from encoded text
    string removePadding(const string& paddedEncodedText) {
        string paddedInfo = paddedEncodedText.substr(0, 8);
        int extraPadding = bitset<8>(paddedInfo).to_ulong();

        string encodedText = paddedEncodedText.substr(8, paddedEncodedText.size() - 8);
        encodedText = encodedText.substr(0, encodedText.size() - extraPadding);
        return encodedText;
    }

    // Decode the encoded text
    string decodeText(const string& encodedText) {
        string currentCode = "";
        string decodedText = "";

        for (char bit : encodedText) {
            currentCode += bit;
            if (reverseMapping.find(currentCode) != reverseMapping.end()) {
                decodedText += reverseMapping[currentCode];
                currentCode = "";
            }
        }

        return decodedText;
    }

public:
    HuffmanCoding(string path) {
        this->path = path;
    }

    // Compress the file
    string compress() {
        ifstream inputFile(path, ios::in);
        if (!inputFile.is_open()) {
            cerr << "Unable to open file: " << path << endl;
            return "";
        }

        string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
        inputFile.close();

        unordered_map<char, int> frequency = makeFrequencyDict(text);
        makeHeap(frequency);
        mergeNodes();
        makeCodes();

        string encodedText = getEncodedText(text);
        string paddedEncodedText = padEncodedText(encodedText);

        vector<unsigned char> byteArray = getByteArray(paddedEncodedText);

        string outputPath = path.substr(0, path.find_last_of('.')) + ".bin";
        ofstream outputFile(outputPath, ios::out | ios::binary);
        for (unsigned char byte : byteArray) {
            outputFile.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
        }
        outputFile.close();

        cout << "Compressed file saved at: " << outputPath << endl;
        return outputPath;
    }

    // Decompress the file
    string decompress(string inputPath) {
        ifstream inputFile(inputPath, ios::in | ios::binary);
        if (!inputFile.is_open()) {
            cerr << "Unable to open file: " << inputPath << endl;
            return "";
        }

        string bitString = "";
        unsigned char byte;
        while (inputFile.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
            bitString += bitset<8>(byte).to_string();
        }
        inputFile.close();

        string encodedText = removePadding(bitString);
        string decompressedText = decodeText(encodedText);

        string outputPath = path.substr(0, path.find_last_of('.')) + "_decompressed.txt";
        ofstream outputFile(outputPath, ios::out);
        outputFile << decompressedText;
        outputFile.close();

        cout << "Decompressed file saved at: " << outputPath << endl;
        return outputPath;
    }
};

int main() {
    string filePath = "input.txt"; // Specify your file path here
    HuffmanCoding huffman(filePath);

    string compressedFilePath = huffman.compress();
    huffman.decompress(compressedFilePath);

    return 0;
}
