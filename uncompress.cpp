#include "HCTree.hpp"
#include "HCNode.hpp"
 
#include <iostream>//stream to allow cout, cin, and endl
#include <iterator> //allow to write to a file using vector
#include <fstream> //steam class to both read and write from/to files
#include <vector> 
  
 //bypasses having to type std:: before cout and endl
  using namespace std;
 
 
 
/* Name: Anthony Grisaff
 * Date: February 6, 2014
 * P3-uncompress.cpp
 */
 
int main(int argc, char *argv[])
{
  vector<int> treeInfo = vector<int>(256,0);
  HCTree* treezy2 = new HCTree();
  int numOfNonZero = 0x0;
  unsigned char decodedLetter;
   
  //check to make sure that there are 3 arguments
  if(argc != 3)
  {
    cout<<"Usage: ./uncompress inputFile outputFile"<<endl;
    return -1;
  }
   
  //create an object for the input and output files
  ifstream inputFile;
  ofstream outputFile;
 
  cout<<"Opening file named "<<argv[1]<<endl;
  //Step 1: open the input file for reading
  inputFile.open(argv[1]);
 
  cout<<"File opened."<<endl;
  //create a BitInputStream to use in decode
  BitInputStream* instream = new BitInputStream(inputFile);
 
  //Step2: read the file header at the beginning of the input file, and 
  //reconstruct the Huffman coding tree.
 
  //read number of non zeros first
  cout<<"Reading the first integer"<<endl;
   
 
  inputFile.read((char*)&numOfNonZero,sizeof(int));
  cout<<"Done."<<endl;
 
  cout<<"numOfNonZero = "<<numOfNonZero<<endl;
 
  //vector<int> asciiChar(numOfNonZero);
  //vector<int> freqOfChar(numOfNonZero);
  int loopCounter = numOfNonZero-1;
 
  while(loopCounter >= 0)
  {
    byte asciiChar = inputFile.get();
    int freqOfChar = 0x0;
    //asciiChar[loopCounter] = inputFile.get();
    //inputFile.read((char*)&asciiChar[numOfNonZero],sizeof(char));
    inputFile.read((char*)&freqOfChar,sizeof(int));
    //combine the two vectors into one
    treeInfo[asciiChar] = freqOfChar;
        loopCounter--;
  }
 
  int TOTAL_SYMBOLS =0x0;
  for(int p = 0; p < 256; p++)
  {
    TOTAL_SYMBOLS += treeInfo[p];
    //cout<<"treeInfo["<<p<<"] = "<< treeInfo[p]<<endl;
  }
 
 
  cout<<"Re-Building tree."<<endl;
  //rebuild the Huffman tree
  if(numOfNonZero != 0)
  {
    treezy2->build(treeInfo);
  }
 
  cout<<"Tree built."<<endl;
  //Step 3: open the output file for writing
  outputFile.open(argv[2]);
 
  //Step 4: using the Huffman coding tree, decode the bits from the input
  //file into the appropriate sequence of bytes, writing them to the ouput
  //file.
  loopCounter = 0x0;//reset loopCounter to 0
 
  cout<<"Writing characters to file"<<endl;
  //loop through then number of non zeros and print each character
  while(loopCounter < TOTAL_SYMBOLS)
  {
    decodedLetter = treezy2->decode(*(instream));
    if(!inputFile.good()) break;
    outputFile.write((char*)&decodedLetter,sizeof(char));
    loopCounter++;
  }
 
  //close the file.
  outputFile.close();
 
  //deallocate the objects from memory
  if(numOfNonZero != 0)
  {
    delete treezy2;
  }
   
  delete instream;
 
  cout<<"ALL DONE!!!!"<<endl;
  return 0;
}
