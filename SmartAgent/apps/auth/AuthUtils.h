// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2011 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#ifndef _AUTH_UTILS_H_
#define _AUTH_UTILS_H_

#include <string>

namespace Apps {
  namespace Auth {
    namespace Utils {
      extern std::string createSalt(unsigned int length);

      // decodeAscii(encodeAscii(a)) == a only if
      // its length multiple of 3 bytes
      /*WT_API extern std::string encodeAscii(const std::string& a);
      WT_API extern std::string decodeAscii(const std::string& a);*/
    }
  }
}

#endif // _AUTH_UTILS_H_
