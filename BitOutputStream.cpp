#include "BitOutputStream.hpp"

/*
 * Write the huffman code bit to a buffer. Once it reaches 8 bits or 1 byte,
 * "flush" it to the output file.
 */
void BitOutputStream::flushBits()
{
  buf = buf << (8-bufi);
  out.put(buf);
  out.flush();
}

void BitOutputStream::writeBit(int bit)
{
  //if the buf reaches 8 bits then flush out the new ASCII character
  if(bufi== 8)
  {
    out.put(buf);
    out.flush();

    //reasign the buf and nits to 0
    bufi = 0;
    buf = 0;
  }

  /*
   * Shift the current buf left by 1 and OR it with the current bit.
   * This will create the new bitcode for the compressed file.
   */
  buf = (buf << 1)|bit;

  //increment the counters
  bufi++;
}
