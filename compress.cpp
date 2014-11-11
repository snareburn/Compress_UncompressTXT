#include "HCTree.hpp"
#include "HCNode.hpp"
 
#include <iostream>//stream to allow cout, cin, and endl
#include <iterator> //allow to write to a file using vector
#include <fstream> //steam class to both read and write from/to files
#include <vector> 
  
 //bypasses having to type std:: before cout and endl
  using namespace std;
 
 
 
/* Name: Anthony Grisaff
 * Date: July 8, 2014
 * P2-compress.cpp
 */
 
int main(int argc, char *argv[])
{
  //local variables
  unsigned char x;
  int counterForSize = 0x0;
  int numOfNonZero = 0x0;
 
  //check to make sure that there are 3 arguments
  if(argc != 3)
  {
    cout<<"Usage: ./compress inputFile outputFile"<<endl;
    return -1;
  }
 
  //Create a tree and a vector to hold the frequencies of each char
  HCTree* treezy = new HCTree();
  vector<int> freqVector;
  freqVector = vector<int>(256,0);
 
  //create an object for the input and output files
  ifstream inputFile;
  ofstream outputFile;//check to see if this exists already
 
  //open the file to be compressed. Step 1
  inputFile.open(argv[1], ios::binary);
 
  /*
   * loop through the file getting each byte, then increment
   * the charVector index when that specific byte occurs.
   * Step 2
   */
  cout<<"Reading from file " << argv[1] << endl;
  while(true)
  {
    
    x = inputFile.get();
    if(!inputFile.good()) break;
    //inputFile.read((unsigned char*)&x,sizeof(unsigned char));
    freqVector[(int)x]++;
    //freqVector[(int)x] = freqVector[(int)x] + 1;
    //cout<<"x = "<< freqVector[x] << endl;
  }
  //printf("freqVector = %d\n", freqVector[x]);
  //cout<<"freqVector = "<< freqVector[x]<<endl;
  //cout<<"size of vect = " << freqVector.size()<<endl;
 
  //find the number of non zero characters
  for(int w = 0; w < (int)freqVector.size(); w++)
  {
    if(freqVector[w] > 0)
    {
      numOfNonZero++;
    }
  }
 
 
  cout<<"Finished reading"<<endl;
  //close the file. Step2 cont.
  inputFile.close();
   
  cout<<"Building Huffman code tree" << endl;
 
  //build the HCTree. Step 3
  if(numOfNonZero != 0)
  {
    treezy->build(freqVector);
  }
 
  else
  {
    cout<<"No characters in "<<argv[1]<<" to build a tree."<<endl;
  }
 
  cout<<"Finished"<<endl;
 
  //open the output file for writing. Step 4
  outputFile.open(argv[2]);
 
  /* Write enough information (a "file header") to the ouput file to enable
   * the coding tree to be reconstruted when the file is read by your
   * uncompress program.
   */
 
  cout<<"num = "<<numOfNonZero<<endl;
  cout<<"Writing Header file"<<endl;

  //write the number of non zero characters to the ouput file first
  outputFile.write((char*)&numOfNonZero, sizeof(int));
   
  /*for(int i = 0; i < (int)freqVector.size(); i++) {
    cout << freqVector[i] << " ";
  }*/
 
 
  while(counterForSize < (int)freqVector.size())
  {
    if(freqVector[counterForSize] != 0)
    {
      //write the idex of the freqVectore pertaining to the ASCII character
      //as 1 byte.
 
      outputFile.write((char*)&counterForSize,sizeof(char));
 
      //next write the frequecy of that character as 4bytes
      outputFile.write((char*)&freqVector[counterForSize],sizeof(int));
   
      counterForSize++;
    }
    else
    {
      counterForSize++;
    }
  }
  cout<<"Finished writing Header file."<<endl;
  //Step 6: reopen  the input file for reading again.
  inputFile.open(argv[1]);
 
  /* 
   * Step 7: using the huffman coding tree translate each byte from the
   * input file into its code, and append these codes as a sequence of bits
   * to the output file, after the header.
   */
                      
  BitOutputStream* outStream = new BitOutputStream(outputFile);
  cout<<"Encoding the file .."<<endl;
  while(true) // this may segfault because of the order.
  {
    byte q = inputFile.get();
    if(! inputFile.good()) break;// check to see if there is another char
    //call encode on the current char
    treezy->encode(q,*outStream);
    
     
  }
 
  cout<<"File is now compressed."<<endl;
 
  //flush out remaining bits
  outStream->flushBits();
  //Step 8: close the input and output files
  inputFile.close();
  outputFile.close();
 
  if(numOfNonZero != 0)
  {
    delete treezy;
  }
  delete outStream;
   
  return 0;
}
