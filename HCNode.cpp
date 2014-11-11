#include "HCNode.hpp"

/*
 * this is the implementation of the HCNode class
 */

/*
 * Make the less than " < " operator work in std::priority_queue. Example
 * of overloading an operator.
 */

bool HCNode::operator<(const HCNode& other)
{
  if(this->count < other.count)
  {
    return false;
  }
  return true;
}
