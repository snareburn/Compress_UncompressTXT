#include "HCTree.hpp"
#include "HCNode.hpp"
#include <vector>
#include <queue>
#include <array>
#include <algorithm>
 
 
using namespace std;
 
 
/*
 * destructor implementation
 */
HCTree::~HCTree()
{
  HCTree::clear();
}
 
void HCTree::clear()
{
  HCTree::clearHelper(root);
}
 
void HCTree::clearHelper(HCNode* curr)
{
  if(!curr)
  {
   return;
  }
 
    if(curr->c0 != NULL)
    {
      HCTree::clearHelper(curr->c0);
    }
     
    if(curr->c1 != NULL)
    {
      HCTree::clearHelper(curr->c1);
    }
    delete curr;
}
 
 
/*
 * method to build the huffman coding tree.
 */
 
void HCTree::build(const vector<int>& freqs)
{
  HCNode* leftNode;
  HCNode* rightNode;
  HCNode* rad;
  int counter = 0x0; //for the while loop
   
  //create a priority queue with the frequency vector
  std::priority_queue<HCNode*,std::vector<HCNode*>,HCNodePtrComp> pq;
 
  //Local variables
  int tempSum = 0x0;
   
  /*
  * Loop through all 256 different ascii characters and add a HCNode
  * pointer for each character. Each node will hold the count or number
  * of occurances for each character
  */
  for(int i = 0; i < 256; i++)
  {
    //ignore all characters that have a count of 0 
    if(freqs.at(i) != 0)
    {
      leftNode = new HCNode(freqs.at(i),i,0,0,0);
      leaves[i] = leftNode;
      //add node to the priority_queue
      pq.push(leftNode);
    }
  }
 
  if(pq.size() == 1)
  {
    rad = pq.top();
    root = rad;
  }
  /*Loop through the priority_queue until there is one node left*/
  while(pq.size() > 1)
  {
    counter++;
    //get the top node of the priority_queue which will hold the smallest
    //count.
    leftNode = pq.top();
 
    //remove the top node;
    pq.pop();
     
    //get the node that is now at the top
    rightNode = pq.top();
 
    //remove that top node now
    pq.pop();
     
    //add the counts for both nodes that were removed and assign them to a temp
    tempSum = leftNode->count + rightNode->count;
 
    //create a new node to be the root that holds the sum of the two previous
    //nodes
    rad = new HCNode(tempSum,leftNode->symbol);
 
    //assign the new root node to have a c0 and a c1
    rad->c0 = leftNode;
    rad->c1 = rightNode;
 
    //update the pointers for the newly assign c0 and c1 to point back
    //at the root
    rad->c0->p = rad;
    rad->c1->p = rad;
 
    //add the node pack into the priority queue
    pq.push(rad);
  }
 // if(counter > 0)
  //{
    root=rad;
  //}
 
}
 
void HCTree::encode(byte symbol, BitOutputStream& out) const
{
  byte sym = symbol;
  HCNode* tempNode = leaves[sym];
  vector<int> bitCode;
  vector<int> reversedBitCode;
  /*
   * find the symbol in the leaves vector. Then find out if it is the 
   * left or right child of it's p
   */
 
  //if tempNode->p is null then you know that the tempNode is 
  //pointing to the root
 
  while(tempNode->p != NULL)
  {
 
    if(tempNode->p->c0 == tempNode) // tempNode is the left child
    {
      //since it is a left child, assign a 0 to the bitCode array
      bitCode.push_back(0);
      //z++;
      tempNode = tempNode->p; //traverse up the tree
 
      if(tempNode == root->c0)
      {
	bitCode.push_back(0);
	break;
      }
    }
 
    else if(tempNode->p->c1 == tempNode) //tempNode is the right child
    {
      //since it is a right child, assign a 1 to the bitCode array
      bitCode.push_back(1);
      //z++;
      tempNode = tempNode->p; //traverse up the tree
       
      if(tempNode == root->c1)
      {
	bitCode.push_back(1);
	break;
      }
    }
  }
 
  //reverse the array
  int bitCodeSize = bitCode.size();
  reverse(bitCode.begin(),bitCode.end());
 
  if(bitCodeSize > 0)
  {
    for(int r = 0; r <bitCodeSize; r++)
    {
      out.writeBit(bitCode[r]);
    }
  }
}
 
/*
 * Return symbol coded in the next sequence of bits from the steam.
 */
 
int HCTree::decode(BitInputStream& inny) const
{
  //integer to help direct down the huffman tree
  int bitRead;
  HCNode* tempNode = root;
 
  while((tempNode->c0 != NULL) | (tempNode->c1 != NULL))
  {
     
    bitRead = inny.readBit();
 
    //if readBit returns a 0 then traverse down the left child(c0)
    if(bitRead == 0)
    {
    //make sure that c0 isn't NULL, if so then it is at a leaf and
    //return the symbol
      tempNode = tempNode->c0; //need to save the 0 obtained and find the 
   //   bitRead = inny.readBit();
    }
 
 
    //if readBit returns a 1 then traverse down the right child(c1)
    else
    {
      tempNode = tempNode->c1;
    //  bitRead = inny.readBit();
   
    }
  }
  return tempNode->symbol;
}

