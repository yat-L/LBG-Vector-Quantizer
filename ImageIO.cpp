/*****************************************************
COMP4160 Assignment 3
Student Name: Ho Yat Lui
Student Number: 7780269
 *****************************************************/
#include "ImageIO.hpp"
#include <cstdint>
#include <cstdlib>
#include <stdio.h>
#include <vector>
using block = std::vector<std::vector<int>>;

// read a block from the image file
vector<block> ImageIO::readBlock(string fileName) {
  vector<block> result;  // whole file in block form in order
  FILE *input = fopen(fileName.c_str(), "rb"); // open output file
  if(input == NULL){
    cout << "Cannot Open file: " << fileName << endl;
    exit(1);
  }
  uint8_t fourRows[IMAGE_WIDTH * 4]; // memory to store 4 rows of byte
  while (fread(&fourRows, sizeof(uint8_t), IMAGE_WIDTH * 4, input) == IMAGE_WIDTH*4) {  // read bytes
    for (int i = 0; i < IMAGE_WIDTH; i += 4) { // for each block in 4 rows
      block currentBlock;  // current block we are working on
      vector<int> firstRow;
      vector<int> secRow;
      vector<int> thirdRow;
      vector<int> fourthRow;
      firstRow.insert(firstRow.end(), &fourRows[i + 0], &fourRows[i + 4]);  // read rows from fourRows
      secRow.insert(secRow.end(), &fourRows[(i + IMAGE_WIDTH) + 0],
                    &fourRows[(i + IMAGE_WIDTH) + 4]);
      thirdRow.insert(thirdRow.end(), &fourRows[(i + 2 * IMAGE_WIDTH) + 0],
                      &fourRows[(i + 2 * IMAGE_WIDTH) + 4]);
      fourthRow.insert(fourthRow.end(), &fourRows[(i + 3 * IMAGE_WIDTH) + 0],
                       &fourRows[(i + 3 * IMAGE_WIDTH) + 4]);
      currentBlock.push_back(firstRow); // add first row to currentBlock
      currentBlock.push_back(secRow); // add second row to curretnBlock etc...
      currentBlock.push_back(thirdRow);
      currentBlock.push_back(fourthRow);
      result.push_back(currentBlock); // add currentBlock to the result 
    }
  }
  fclose(input); // close the file
  return result;
}

// write a block to the output image file
void ImageIO::writeBlock(block oBlock){
  buffer.push_back(oBlock); // add blokc into buffer
  if(buffer.size() == IMAGE_WIDTH/4){ // if buffer contain 4 rows
    for(int i = 0 ; i < 4 ; i++){ // for each row
      vector<int> oneRow;
      for(int j = 0 ; j < buffer.size() ; j++){ // for each block the ith row
        vector<int> fourByte = buffer[j][i]; // four byte of ith row
        oneRow.insert(oneRow.end(),fourByte.begin(),fourByte.end()); // flatten to oneRow
      }
      writeRow(oneRow); // write one row when all block have written their ith row
    }
    buffer.clear(); // clear the buffer
  }
}

// write one row of byte to the output image
void ImageIO::writeRow(vector<int> out){
  for (int i = 0; i < out.size(); i++) { // for each byte in the vector
    uint8_t temp = out[i]; // cast it to 1 byte
    int result = fwrite(&temp, sizeof(uint8_t), 1, outputFP); // output it
    if (result != 1) {
      cout << "Writing Error" << endl;
    }
  }
}


// contructor to initiate the file pointer
ImageIO::ImageIO(string fileName){
      outputFP = fopen(fileName.c_str(), "wb"); // open output file
      if (outputFP == NULL) {
        cout << "Cannot Open file: " << fileName << endl;
        exit(1);
      }
}
// descructor to close the file
ImageIO::~ImageIO(){
  fclose(outputFP);
}
