#include "BitInputStream.hpp"

/*
 * when all of the bits have been read in the buffer, refill it 
 * with the next character
 */
void BitInputStream::fill()
{
  in.read((char*)&buf, sizeof(unsigned char));
  bufi = 8;
}


/*
 * Read each bit in the current buffer.
 */
int BitInputStream::readBit()
{
  char bitMask = 0x80; //bit mask = 10000000
  int bitNum;

  //If the buffer is empty then refill it.
  if(bufi == 0)
  {
    //make cases for integer and char
    fill();
  }

  //If the bit is a one, then bitNum will equal one and will return 
  //one. 
  bitNum = buf & bitMask;

  //shift the remaining bits to the left by one.
  buf = buf << 1;
  bufi--;
  return bitNum;
}
