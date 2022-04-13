/*****************************************************
COMP4160 Assignment 3
Student Name: Ho Yat Lui
Student Number: 7780269
 *****************************************************/
#include <iostream>
#include <vector>
#define IMAGE_WIDTH 256

using block = std::vector<std::vector<int>>;
using namespace std;
class ImageIO {
  vector<block> buffer; // buffer to store blocks before it reaches 256 width
  FILE* outputFP; // file pointer for output

public:
  ImageIO(string fileName);
  ~ImageIO();
  vector<block> readBlock(string fileName);
  void writeBlock(block);
  void writeRow(vector<int>);
};
