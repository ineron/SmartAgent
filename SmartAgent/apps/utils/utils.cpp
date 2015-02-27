//
//  utils.cpp
//  SmartAgent
//
//  Created by Евгений Илюшин on 20.01.15.
//  Copyright (c) 2015 Евгений Илюшин. All rights reserved.
//

#include "utils.h"
#include "rapidxml/rapidxml.hpp"
#include <locale>
#include <codecvt>
#include <boost/lexical_cast.hpp>

namespace {
    static const std::size_t stack_buffer_size = 512;
}

namespace Utils {

    std::wstring s2ws(const std::string& str)
    {
        typedef std::codecvt_utf8<wchar_t> convert_typeX;
        std::wstring_convert<convert_typeX, wchar_t> converterX;
        
        return converterX.from_bytes(str);
    }
    
    std::string ws2s(const std::wstring& wstr)
    {
        typedef std::codecvt_utf8<wchar_t> convert_typeX;
        std::wstring_convert<convert_typeX, wchar_t> converterX;
        
        return converterX.to_bytes(wstr);
    }
    
    std::wstring widen(const std::string& s, const std::locale &loc)
    {
        typedef std::codecvt<wchar_t, char, std::mbstate_t> Cvt;
        
        std::wstring result;
        result.reserve(s.length());
        
        const Cvt& myfacet = std::use_facet<Cvt>(loc);
        Cvt::result myresult;
        std::mbstate_t mystate = std::mbstate_t();
        
        wchar_t stack_buffer[stack_buffer_size + 1];
        const char* next_to_convert = s.c_str();
        const char* const to_convert_end = s.c_str() + s.length();
        
        bool error = false;
        
        while (next_to_convert != to_convert_end) {
            wchar_t* converted_end = stack_buffer;
            myresult = myfacet.in(mystate, next_to_convert, to_convert_end,
                                  next_to_convert,
                                  stack_buffer, stack_buffer + stack_buffer_size,
                                  converted_end);
            
            result.append(stack_buffer, converted_end);
            
            if (myresult == Cvt::error) {
                result += L'?';
                ++ next_to_convert;
                error = true;
            }
        }
        
        if (error)
            std::cout<<"widen(): could not widen string: " << s<<std::endl;
        
        return result;
    }

    
    std::string narrow(const std::wstring& s, const std::locale &loc)
    {
        typedef std::codecvt<wchar_t, char, std::mbstate_t> Cvt;
        
        const Cvt& myfacet = std::use_facet<Cvt>(loc);
        
        Cvt::result myresult;
        
        const wchar_t *pwstr = s.c_str();
        const wchar_t *pwend = s.c_str() + s.length();
        const wchar_t *pwc = pwstr;
        
        size_t size = s.length() + 1;
        
        char *pstr = new char [size];
        char *pc = pstr;
        
        std::mbstate_t mystate = std::mbstate_t();
        bool error = false;
        
        for (;;) {
            myresult = myfacet.out(mystate, pwc, pwend, pwc, pc, pc + size, pc);
            
            if (myresult == Cvt::ok) {
                break;
            } else {
                if (myresult == Cvt::partial || pc >= pstr + size) {
                    size += s.length();
                    std::size_t sofar = pc - pstr;
                    pstr = (char *)std::realloc(pstr, size);
                    pc = pstr + sofar;
                }
                
                if (myresult == Cvt::error) {
                    *pc++ = '?';
                    pwc++;
                    error = true;
                }
            }
        }
        
        std::string result(pstr, pc - pstr);
        
        if (error)
            std::cout <<"narrow(): loss of detail: " << result<<std::endl;
        
        delete[] pstr;
        
        return result;
    }

    
    std::wstring fromUTF8(const std::string& s)
    {
        std::wstring result;
        result.reserve(s.length());
        
        for (unsigned i = 0; i < s.length(); ++i) {
            bool legal = false;
            if ((unsigned char)s[i] <= 0x7F) {
                unsigned char c = s[i];
                if (c == 0x09 || c == 0x0A || c == 0x0D || c >= 0x20) {
                    result += (wchar_t)(c);
                    legal = true;
                }
            } else if ((unsigned char)s[i] >= 0xF0) {
                if (i + 3 < s.length()) {
                    if ((
                         // F0 90-BF 80-BF 80-BF
                         (                                    (unsigned char)s[i] == 0xF0)
                         && (0x90 <= (unsigned char)s[i+1] && (unsigned char)s[i+1] <= 0xBF)
                         && (0x80 <= (unsigned char)s[i+2] && (unsigned char)s[i+2] <= 0xBF)
                         && (0x80 <= (unsigned char)s[i+3] && (unsigned char)s[i+3] <= 0xBF)
                         ) ||
                        (
                         // F1-F3 80-BF 80-BF 80-BF
                         (   0xF1 <= (unsigned char)s[i]   && (unsigned char)s[i] <= 0xF3)
                         && (0x80 <= (unsigned char)s[i+1] && (unsigned char)s[i+1] <= 0xBF)
                         && (0x80 <= (unsigned char)s[i+2] && (unsigned char)s[i+2] <= 0xBF)
                         && (0x80 <= (unsigned char)s[i+3] && (unsigned char)s[i+3] <= 0xBF)
                         )) {
                            legal = true;
                            
                            uint32_t cp = ((unsigned char)s[i]) & 0x0F;
                            for (unsigned j = 1; j < 4; ++j) {
                                cp <<= 6;
                                cp |= ((unsigned char)s[i+j]) & 0x3F;
                            }
                            
                            wchar_t wc = cp;
                            if ((uint32_t)wc == cp)
                                result += wc;
                            else
                                legal = false;
                        }
                }
                i += 3;
            } else if ((unsigned char)s[i] >= 0xE0) {
                if (i + 2 < s.length()) {
                    if ((
                         // E0 A0*-BF 80-BF
                         (                                    (unsigned char)s[i] == 0xE0)
                         && (0xA0 <= (unsigned char)s[i+1] && (unsigned char)s[i+1] <= 0xBF)
                         && (0x80 <= (unsigned char)s[i+2] && (unsigned char)s[i+2] <= 0xBF)
                         ) ||
                        (
                         // E1-EF 80-BF 80-BF
                         (   0xE1 <= (unsigned char)s[i]   && (unsigned char)s[i] <= 0xF1)
                         && (0x80 <= (unsigned char)s[i+1] && (unsigned char)s[i+1] <= 0xBF)
                         && (0x80 <= (unsigned char)s[i+2] && (unsigned char)s[i+2] <= 0xBF)
                         )) {
                            legal = true;
                            
                            wchar_t cp = ((unsigned char)s[i]) & 0x1F;
                            for (unsigned j = 1; j < 3; ++j) {
                                cp <<= 6;
                                cp |= ((unsigned char)s[i+j]) & 0x3F;
                            }
                            
                            wchar_t wc = cp;
                            if (wc == cp)
                                result += wc;
                            else
                                legal = false;
                        }
                }
                i += 2;
            } else if ((unsigned char)s[i] >= 0xC0) {
                if (i + 1 < s.length()) {
                    if (
                        // C2-DF 80-BF
                        (   0xC2 <= (unsigned char)s[i]   && (unsigned char)s[i] <= 0xDF)
                        && (0x80 <= (unsigned char)s[i+1] && (unsigned char)s[i+1] <= 0xBF)
                        ) {
                        legal = true;
                        
                        wchar_t cp = ((unsigned char)s[i]) & 0x3F;
                        for (unsigned j = 1; j < 2; ++j) {
                            cp <<= 6;
                            cp |= ((unsigned char)s[i+j]) & 0x3F;
                        }
                        
                        wchar_t wc = cp;
                        if (wc == cp)
                            result += wc;
                        else
                            legal = false;
                    }
                }
                i += 1;
            }
            
            if (!legal)
                result += (wchar_t)0xFFFD;
        }
        
        return result;
    };
    std::string fromUTF8(const std::string& s, const std::locale &loc)
    {
        return narrow(fromUTF8(s), loc);
    };
    
    std::string toUTF8(const std::wstring& s)
    {
        std::string result;
        result.reserve(s.length() * 3);
        
        char buf[4];
        for (std::wstring::const_iterator i = s.begin(); i != s.end(); ++i) {
            char *end = buf;
            try {
                Utils::rapidxml::xml_document<>::insert_coded_character<0>(end, *i);
                for (char *b = buf; b != end; ++b)
                    result += *b;
            } catch (Utils::rapidxml::parse_error& e) {
                std::cout<<"toUTF8(): " << e.what()<<std::endl;
            }
        }
        
        return result;
    };
    std::string toUTF8(const std::string& s, const std::locale &loc)
    {
        return toUTF8(widen(s, loc));
    };
    
    std::string toString(int value)
    {
        return boost::lexical_cast<std::string>(value);
    };
    
    std::string toString(unsigned value)
    {
        return boost::lexical_cast<std::string>(value);
    };
    
    std::string toString(::int64_t value)
    {
        return boost::lexical_cast<std::string>(value);
    };
    
    std::string toString(::uint64_t value)
    {
        return boost::lexical_cast<std::string>(value);
    };
    
    std::string toString(double value)
    {
        return boost::lexical_cast<std::string>(value);
    };
    
    std::string& replace(std::string& s, char c, const std::string& r)
    {
        std::string::size_type p = 0;
        
        while ((p = s.find(c, p)) != std::string::npos) {
            s.replace(p, 1, r);
            p += r.length();
        }
        
        return s;
    };
    
    std::string& replace(std::string& s, const std::string& k, const std::string& r)
    {
        std::string::size_type p = 0;
        
        while ((p = s.find(k, p)) != std::string::npos) {
            s.replace(p, k.length(), r);
            p += r.length();
        }
        
        return s;
    }
}