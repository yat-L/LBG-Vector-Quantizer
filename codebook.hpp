/*****************************************************
COMP4160 Assignment 3
Student Name: Ho Yat Lui
Student Number: 7780269
 *****************************************************/
#include <iostream>
#include <time.h>
#include <vector>
#define PIXEL_MAX 256
using block = std::vector<std::vector<int>>; // block is a 4x4 vector
using namespace std;
class codebook {
  vector<block> codeword; // store the trained codeword
  vector<vector<block>>
      region; // contains regions, list of blocks that belongs to that region
  vector<block> training; // all training blocks
  double epsilon; // provided by command line, for LBG algorithm
  int maxSize; // privided by command line, max size of codebook

public:
  void printCodeBook(); 
  block replaceBlock(block);
  codebook(vector<block>, double, int);

private:
  block centroid(vector<block>);
  void printBlock(block);
  block randomBlock(block);
  double distance(block, block);
  double distortion();
  void assignRegion();
  void reAssignCode();
  void LindeBuzoGray();
  void initCodebook();
  void doubleCodebook();
};
