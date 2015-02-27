//
//  utils.h
//  SmartAgent
//
//  Created by Евгений Илюшин on 20.01.15.
//  Copyright (c) 2015 Евгений Илюшин. All rights reserved.
//

#ifndef __SmartAgent__utils__
#define __SmartAgent__utils__

#include <stdio.h>
#include <string>
#include <locale>

#define INLINE_WQUERY(...)  L ## #__VA_ARGS__

namespace Utils {
    extern std::wstring s2ws(const std::string& str);
    extern std::string ws2s(const std::wstring& str);
    
    extern std::wstring fromUTF8(const std::string& s);
    extern std::string fromUTF8(const std::string& s,const std::locale &loc);
    extern std::string toUTF8(const std::wstring& s);
    extern std::string toUTF8(const std::string& s,const std::locale &loc = std::locale());
    extern std::string toString(int value);
    extern std::string toString(unsigned value);
    extern std::string toString(::int64_t value);
    extern std::string toString(::uint64_t value);
    extern std::string toString(double value);
    extern std::string& replace(std::string& s, char c, const std::string& r);
    extern std::string& replace(std::string& s, const std::string& c, const std::string& r);
}

#endif /* defined(__SmartAgent__utils__) */
