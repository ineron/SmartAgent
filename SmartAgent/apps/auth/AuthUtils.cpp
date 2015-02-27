/*
 * Copyright (C) 2011 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <vector>
#include <iterator>
#include <cstring>
#include <stdlib.h>

#include "AuthUtils.h"
//#include "base64.h"

//#include "Wt/WRandom"

namespace Apps {
  namespace Auth {
    namespace Utils {

std::string createSalt(unsigned int length)
{
  unsigned char *saltBuf = new unsigned char[length];
  for (unsigned i = 0; i < length; i += 3) {
      unsigned r = rand() % 32768;
    std::memcpy(saltBuf + i, &r, 3);
  }

  std::string s(saltBuf, saltBuf + length);

  delete[] saltBuf;

  return s;
}


    }
  }
}
