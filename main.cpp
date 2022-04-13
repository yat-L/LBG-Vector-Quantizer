/*****************************************************
COMP4160 Assignment 3
Student Name: Ho Yat Lui
Student Number: 7780269
 *****************************************************/
#include "ImageIO.hpp"
#include "codebook.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  double eps = atof(argv[3]);
  int size = atoi(argv[4]);
  if (argc < 5) { // if too little arguments
    cout << "Please provide command." << endl;
    return 1;
  }
  if (size % 2 != 0) { // if codebook size not power of 2
    cout << "Codebook Size have to be a power of 2." << endl;
    return 1;
  }
  if (!(eps <= 1 && eps >= 0)) { // if epsilon not between 0 - 1
    cout << "Epsilon have to be between 0 to 1." << endl;
    return 1;
  }

  ImageIO I(argv[2]);                          // init image IO
  vector<block> result = I.readBlock(argv[1]); // read the input image
  codebook c(result, eps, size);               // init codebook
  c.printCodeBook(); // print the whole codebook after training
  for (int i = 0; i < result.size(); i++) {
    I.writeBlock(c.replaceBlock(result[i])); // replace each block
  }
}
