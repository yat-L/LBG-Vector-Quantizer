/*****************************************************
COMP4160 Assignment 3
Student Name: Ho Yat Lui
Student Number: 7780269
 *****************************************************/
#include "codebook.hpp"
#include <assert.h>
#include <cmath>
#include <cstdlib>
#include <iterator>
#include <limits>
#include <math.h>
#include <stdlib.h>
#include <vector>

// print all the blocks in the codebook
void codebook::printCodeBook() {
  cout << "Code Book Content: " << endl;
  for (int i = 0; i < codeword.size(); i++) {
    printBlock(codeword[i]);
  }
}

// output the block that is the closest to source
block codebook::replaceBlock(block source) {
  block estimate;
  double min = std::numeric_limits<double>::infinity(); // make sure everything
                                                        // is smaller than this
                                                        // at start of the loop
  int closestRegion = 0; // index number for region that source belongs to
  for (int i = 0; i < codeword.size(); i++) {
    double dist = distance(codeword[i], source); // calculate distance
    if (dist <= min) { // if distance is smaller than previous loop
      closestRegion = i;
      min = dist;
    }
  }
  estimate = codeword[closestRegion]; // output the block at the correct region
  return estimate;
}

// double the codebook entry, use after running LBG algorithm
void codebook::doubleCodebook() {
  vector<block> newBlocks; // store list of new blocks to add
  for (int i = 0; i < codeword.size(); i++) {
    newBlocks.push_back(randomBlock(codeword[i])); // get random blocks
  }
  codeword.insert(codeword.end(), newBlocks.begin(),
                  newBlocks.end()); // concat codeword with the new blocks.
  vector<block> empty;
  int oldRegionSize = region.size();
  for (int i = 0; i < oldRegionSize;
       i++) { // add entry to region so region and codeword have the same size
    region.push_back(empty);
  }
  assert(codeword.size() == region.size());
}

// for training the codebook
void codebook::initCodebook() {
  doubleCodebook();                   // now codebook have 2 entry
  while (codeword.size() < maxSize) { // until target size is met
    LindeBuzoGray();                  // run LBG
    doubleCodebook();                 // double codebook after run
  }
  assert(codeword.size() == maxSize);
}

// implement the LBG algorithm
void codebook::LindeBuzoGray() {
  double oldDistortion = 0; // Distortion from previous iteration
  bool stop = false;        // decide if LBG should stop
  do {
    assignRegion(); // assign each vector in training a region
    double currentDistort = distortion(); // distortion of current iteration
    double diff = abs(currentDistort - oldDistortion);
    double stopCondition = diff / currentDistort; // stop condition
    if (stopCondition < epsilon) {
      stop = true;
    } else {
      reAssignCode(); // if don't stop, assign codebook block to be centriod
    }
    oldDistortion = currentDistort; // save the distortion for next loop
  } while (!stop);                  // stop
}

// set each block in codeword to be the centriod of all the block
// in the region
void codebook::reAssignCode() {
  for (int i = 0; i < region.size(); i++) {
    if (region[i].size() == 0) { // if no block assigned to initial, pick random
      int maxPopu = -1;          // size of maximum poplated region
      int maxRegion = 0;         // index of maxumum populated region
      for (int j = 0; j < region.size(); j++) { // for each region
        if (region[j].size() >= maxPopu) {      // if this region is larger
          maxPopu = region[j].size();           // update variables
          maxRegion = j;
        }
      }
      int newIndex =
          rand() % region[maxRegion].size(); // index of the random block in the
                                             // maximum populated region
      codeword[i] = region[maxRegion][newIndex]; // assign new codeword
    } else {
      codeword[i] = centroid(region[i]);  // assign region codeword to be centroid
    }
  }
}

// assign each block in training to a region
void codebook::assignRegion() {
  for (int i = 0; i < training.size(); i++) {// for each block in training
    int minRegion = 0; // index of minimum region
    double min = std::numeric_limits<double>::infinity(); // make sure largest
    for (int j = 0; j < codeword.size(); j++) {
      double currDistance = distance(training[i], codeword[j]); // distance
      if (currDistance <= min) {
        min = currDistance; // update index and minimum
        minRegion = j;
      }
    }
    region[minRegion].push_back(training[i]); // assign region
  }
}

// calculate distortion of current codeword and region
double codebook::distortion() {
  double result = 0;
  for (int i = 0; i < codeword.size(); i++) { // for each codeword
    block currentRegion = codeword[i];
    for (int j = 0; j < region[i].size(); j++) { // for each region
      block currentBlock = region[i][j];
      double currDistance = distance(currentRegion, currentBlock);
      result += currDistance * currDistance;
    }
  }
  result /= (double)training.size(); // devide by number of training block

  return result;
}

// distance between 2 block
double codebook::distance(block b1, block b2) {
  double distanceR = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int diff = b1[i][j] - b2[i][j]; // for each block in the loop, difference
      distanceR += diff * diff; // squre them
    }
  }
  distanceR = sqrt(distanceR); // squear root

  return distanceR;
}

// calcualte centroid of a list
block codebook::centroid(vector<block> list) {
  block result(4, vector<int>(4));
  for (int i = 0; i < list.size(); i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        result[j][k] += list[i][j][k]; // add blocks in list by index
      }
    }
  }
  for (int j = 0; j < 4; j++) {
    for (int k = 0; k < 4; k++) {
      result[j][k] = result[j][k] / list.size(); // devide them with list size
    }
  }

  return result;
}

// print a block to output, stylized
void codebook::printBlock(block b) {

  for (int i = 0; i < 4; i++) {
    cout << "[";
    for (int j = 0; j < 4; j++) {
      cout << b[i][j] << " ";
    }
    cout << "]" << endl;
  }
  cout << endl;
}

// priovide a randomblick from a block
block codebook::randomBlock(block b) {
  block result(4, vector<int>(4));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int shift = rand() % PIXEL_MAX - b[i][j]; // shift number by random, make sure it's between 0 - 255
      result[i][j] = b[i][j] + shift; // shift the number
      assert(result[i][j] <= 255 && result[i][j] >= 0);
    }
  }
  return result;
}

// contructor of codebook
codebook::codebook(vector<block> train, double eps, int size) {
  srand(time(NULL)); // start with a random seed
  epsilon = eps; 
  maxSize = size;
  training = train;
  block initBlock = centroid(training); // start codebook with 1 block, which is the centroid of training blocks
  codeword = vector<block>(1, initBlock); // init codeword
  vector<block> empty;
  region.push_back(empty); // init region with a empty vector
  initCodebook(); // call initCodebook to double the codeword and run LBG
  return;
}
