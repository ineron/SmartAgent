//
//  String.cpp
//  SmartAgent
//
//  Created by Евгений Илюшин on 20.01.15.
//  Copyright (c) 2015 Евгений Илюшин. All rights reserved.
//

#include "String.h"
#include "utils.h"
#include "rapidxml/rapidxml.hpp"
#include <boost/algorithm/string/trim.hpp>

namespace Utils {
    std::vector<String> String::stArguments_;
    const String String::Empty;
    String::CharEncoding String::defaultEncoding_ = LocalEncoding;
    
    String::String()
    : impl_(0)
    { }
    
    String::String(const wchar_t *value)
    : impl_(0)
    {
        if (value)
            utf8_ = Utils::toUTF8(value);
    }
    String::String(const std::wstring& value)
    : impl_(0)
    {
        utf8_ = Utils::toUTF8(value);
    }
    
    String::String(const char *value, CharEncoding encoding)
    : impl_(0)
    {
        if (value) {
            if (realEncoding(encoding) == UTF8)
                utf8_ = value;
            else
                utf8_ = Utils::toUTF8(value);
        }
    }
    
    String::String(const std::string& value, CharEncoding encoding)
    : impl_(0)
    {
        if (realEncoding(encoding) == UTF8)
            utf8_ = value;
        else
            utf8_ = Utils::toUTF8(value);
    }
    
    String::String(const char *value, const std::locale& loc)
    : impl_(0)
    {
        utf8_ = Utils::toUTF8(value, loc);
    }
    String::String(const std::string& value, const std::locale& loc)
    : impl_(0)
    {
        utf8_ = Utils::toUTF8(value, loc);
    }
    
    String::String(const String& other)
    : utf8_(other.utf8_),
    impl_(0)
    {
        if (other.impl_)
            impl_ = new Impl(*other.impl_);
    }
    
    String::Impl::Impl()
    : n_(-1)
    { }
    
    String::~String()
    {
        delete impl_;
    }
    
    bool String::operator== (const String& rhs) const
    {
        return toUTF8() == rhs.toUTF8();
    }
    
    bool String::operator< (const String& rhs) const
    {
        return toUTF8() < rhs.toUTF8();
    }
    
    bool String::operator> (const String& rhs) const
    {
        return toUTF8() > rhs.toUTF8();
    }
    
    String& String::operator= (const String& rhs)
    {
        if (this != &rhs) {
            this->String::~String();
            new (this) String(rhs);
        }
        
        return *this;
    }
    
    String& String::operator+= (const String& rhs)
    {
        makeLiteral();
        utf8_ += rhs.toUTF8();
        
        return *this;
    }
    
    String& String::operator+= (const std::wstring& rhs)
    {
        makeLiteral();
        utf8_ += Utils::toUTF8(rhs);
        
        return *this;
    }
    String& String::operator+= (const wchar_t *rhs)
    {
        makeLiteral();
        utf8_ += Utils::toUTF8(rhs);
        
        return *this;
    }
    
    String& String::operator+= (const std::string& rhs)
    {
        makeLiteral();
        utf8_ += Utils::toUTF8(rhs);
        
        return *this;
    }
    
    String& String::operator+= (const char *rhs)
    {
        makeLiteral();
        utf8_ += Utils::toUTF8(rhs);
        
        return *this;
    }
    
    bool String::empty() const
    {
        if (literal())
            return utf8_.empty();
        else
            return toUTF8().empty();
    }
    
    String String::trim() const
    {
        std::string u8 = toUTF8();
        
        boost::trim(u8);
        
        return String::fromUTF8(u8, false);
    }
    
    String String::fromUTF8(const std::string& value, bool checkValid)
    {
        String result(value, UTF8);
        if (checkValid)
            checkUTF8Encoding(result.utf8_);
        return result;
    }
    
    String String::fromUTF8(const char *value, bool checkValid)
    {
        String result(value, UTF8);
        if (checkValid)
            checkUTF8Encoding(result.utf8_);
        return result;
    }
    
    void String::checkUTF8Encoding(std::string& value)
    {
        unsigned pos = 0;
        for (; pos < value.length();) {
            unsigned at = pos;
            const char *c_start = value.c_str() + pos;
            const char *c = c_start;
            try {
                char *dest = 0;
                Utils::rapidxml::xml_document<>::copy_check_utf8(c, dest);
                pos += c - c_start;
            } catch (Utils::rapidxml::parse_error& e) {
                pos += c - c_start;
                for (unsigned i = at; i < pos && i < value.length();
                     ++i)
                    value[i] = '?';
            }
        }
    }
    
    std::string String::resolveKey() const
    {
        //std::string result;
        return "??" + impl_->key_ + "??";
    }
    
    std::string String::toUTF8() const
    {
        if (impl_) {
            std::string result = utf8_;
            
            if (!impl_->key_.empty())
                result = resolveKey();
            
            for (unsigned i = 0; i < impl_->arguments_.size(); ++i) {
                std::string key = '{' + boost::lexical_cast<std::string>(i+1) + '}';
                Utils::replace(result, key, impl_->arguments_[i].toUTF8());
            }
            
            return result;
        } else
            return utf8_;
    }
    
    String String::tr(const char *key)
    {
        return String(key, true);
    }
    
    String String::tr(const std::string& key)
    {
        return String(key.c_str(), true);
    }
    
    String String::trn(const char *key, ::uint64_t n)
    {
        return String(key, true, n);
    }
    
    String String::trn(const std::string& key, ::uint64_t n)
    {
        return String(key.c_str(), true, n);
    }
    
    String::String(const char *key, bool, ::uint64_t n)
    {
        impl_ = new Impl;
        impl_->key_ = key;
        impl_->n_ = n;
    }
    
    std::wstring String::value() const
    {
        return Utils::fromUTF8(toUTF8());
    }
    
    std::string String::narrow(const std::locale &loc) const
    {
        return Utils::fromUTF8(toUTF8(), loc);
    }
    
    String::operator std::wstring() const
    {
        return value();
    }
    
    const std::string String::key() const
    {
        if (impl_)
            return impl_->key_;
        else
            return std::string();
    }
    
    void String::createImpl()
    {
        if (!impl_)
            impl_ = new Impl();
    }
    
    String& String::arg(const std::string& value, CharEncoding encoding)
    {
        createImpl();
        
        if (realEncoding(encoding) == UTF8)
            impl_->arguments_.push_back(String::fromUTF8(value, true));
        else {
            String s;
            s.utf8_ = Utils::toUTF8(value);
            impl_->arguments_.push_back(s);
        }
        
        return *this;
    }
    
    String& String::arg(const char *value, CharEncoding encoding)
    {
        return arg(std::string(value), encoding);
    }
    
    String& String::arg(const std::wstring& value)
    {
        createImpl();
        
        String s;
        s.utf8_ = Utils::toUTF8(value);
        impl_->arguments_.push_back(s);
        
        return *this;
    }
    
    String& String::arg(const String& value)
    {
        createImpl();
        
        impl_->arguments_.push_back(value);
        
        return *this;
    }
    
    String& String::arg(int value)
    {
        return arg(Utils::toString(value));
    }
    
    String& String::arg(unsigned value)
    {
        return arg(Utils::toString(value));
    }
    
    String& String::arg(::int64_t value)
    {
        return arg(Utils::toString(value));
    }
    
    String& String::arg(::uint64_t value)
    {
        return arg(Utils::toString(value));
    }
    
    String& String::arg(double value)
    {
        return arg(Utils::toString(value));
    }
    
    bool String::refresh()
    {
        if (literal())
            return false;
        else
            return true;
    }
    
    const std::vector<String>& String::args() const
    {
        if (impl_)
            return impl_->arguments_;
        else
            return stArguments_;
    }
    
    String utf8(const char *value)
    {
        return String(value, String::UTF8);
    }
    
    String utf8(const std::string& value)
    {
        return String(value, String::UTF8);
    }
    
    
    void String::setDefaultEncoding(CharEncoding encoding)
    {
        if (encoding == DefaultEncoding)
            defaultEncoding_ = LocalEncoding;
        else
            defaultEncoding_ = encoding;
    }
    
    String::CharEncoding String::realEncoding(CharEncoding encoding)
    {
        return encoding == DefaultEncoding ? defaultEncoding_ : encoding;
    }
    
    String operator+ (const String& lhs, const String& rhs)
    {
        String result = lhs;
        return result += rhs;
    }
    
    String operator+ (const String& lhs, const std::wstring& rhs)
    {
        String result = lhs;
        return result += rhs;
    }

    String operator+ (const String& lhs, const wchar_t *rhs)
    {
        String result = lhs;
        return result += rhs;
    }
    
    String operator+ (const String& lhs, const std::string& rhs)
    {
        String result = lhs;
        return result += rhs;
    }
    
    String operator+ (const String& lhs, const char *rhs)
    {
        String result = lhs;
        return result += rhs;
    }
    
    String operator+ (const std::wstring& lhs, const String& rhs)
    {
        String result = lhs;
        return result += rhs;
    }

    String operator+ (const wchar_t *lhs, const String& rhs)
    {
        String result = lhs;
        return result += rhs;
    }
    
    String operator+ (const std::string& lhs, const String& rhs)
    {
        String result = lhs;
        return result += rhs;
    }
    
    String operator+ (const char *lhs, const String& rhs)
    {
        String result = lhs;
        return result += rhs;
    }
    
    bool operator== (const char *lhs, const String& rhs)
    {
        return rhs == lhs;
    }
    
    bool operator== (const std::string& lhs, const String& rhs)
    {
        return rhs == lhs;
    }
    
    bool operator== (const std::wstring& lhs, const String& rhs)
    {
        return rhs == lhs;
    }
    bool operator== (const wchar_t *lhs, const String& rhs)
    {
        return rhs == lhs;
    }
    
    bool operator!= (const char *lhs, const String& rhs)
    {
        return !(rhs == lhs);
    }
    
    bool operator!= (const std::string& lhs, const String& rhs)
    {
        return !(rhs == lhs);
    }
    
    bool operator!= (const std::wstring& lhs, const String& rhs)
    {
        return !(rhs == lhs);
    }

    bool operator!= (const wchar_t *lhs, const String& rhs)
    {
        return !(rhs == lhs);
    }
    
    void String::makeLiteral()
    {
        if (!literal()) {
            utf8_ = resolveKey();
            impl_->key_ = std::string();
        }
    }
    
    std::wostream& operator<< (std::wostream& lhs, const String& rhs)
    {
        return lhs << rhs.value();
    }
    
    std::ostream& operator<< (std::ostream& lhs, const String& rhs)
    {
        return lhs << rhs.narrow();
    }
    
}
