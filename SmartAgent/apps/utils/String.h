//
//  String.h
//  SmartAgent
//
//  Created by Евгений Илюшин on 20.01.15.
//  Copyright (c) 2015 Евгений Илюшин. All rights reserved.
//

#ifndef __SmartAgent__String__
#define __SmartAgent__String__

#include <stdio.h>
#include <string>
#include <vector>
#include <iosfwd>
#include <locale>

namespace Utils {
    class String
    {
    public:
        
        enum CharEncoding{
            DefaultEncoding,
            LocalEncoding,
            UTF8
        };
        /*! \brief Sets the encoding for
         *         \link Wt::DefaultEncoding DefaultEncoding\endlink
         *
         * String::setDefaultEncoding() controls the behaviour of
         * \link Wt::DefaultEncoding DefaultEncoding\endlink. If not modified, the
         * default encoding will be the encoding dictated by the C++ global locale.
         * Often UTF-8 is desired, in which case you can use this method to change
         * the system-wide how Wt should interpret C style strings.
         *
         * Since this is a system-wide setting, and not a per-session setting,
         * you should call this function before any session is created, e.g. in
         * main() before calling WRun().
         *
         * Note: in the future, the default encoding for DefaultEncoded strings may
         * change to UTF-8 instead of the C++ global locale.
         */
        static void setDefaultEncoding(CharEncoding encoding);
        
        /*! \brief Creates an empty string
         *
         * Create a literal string with empty contents ("").
         */
        String();
        
        /*! \brief Creates a %String from a wide C string
         *
         * The wide string is implicitly converted to proper unicode. Note
         * that there are known issues with the portability of wchar_t since
         * its width and encoding are platform dependent.
         */
        String(const wchar_t *value);
        
        /*! \brief Copy constructor
         */
        String(const String& other);
        
        /*! \brief Creates a %String from a wide C++ string
         *
         * The wide string is implicitly converted to proper unicode. Note
         * that there are known issues with the portability of wchar_t since
         * its width and encoding are platform dependent.
         */
        String(const std::wstring& value);
        
        /*! \brief Creates a %String from a C string.
         *
         * The C string is implicitly converted to unicode. When
         * \p encoding is \link Wt::LocalEncoding LocalEncoding\endlink,
         * the current locale is used to interpret the C string. When
         * encoding is \link Wt::UTF8 UTF8\endlink, the C string is
         * interpreted as a UTF8 encoded unicode string.
         *
         * String::setDefaultEncoding() controls the behaviour of
         * \link Wt::DefaultEncoding DefaultEncoding\endlink. Use it to set a
         * system-wide default format for C style strings (e.g. to UTF-8).
         */
        String(const char *value, CharEncoding encoding = DefaultEncoding);
        
        /*! \brief Creates a %String from a C string.
         *
         * The C string is implicitly converted to unicode. The
         * string is interpreted within the character set of the given locale.
         */
        String(const char *value, const std::locale &loc);
        
        
        /*! \brief Creates a %String from a C++ string.
         *
         * The C++ string is implicitly converted to unicode. When
         * \p encoding is \link Wt::LocalEncoding LocalEncoding\endlink,
         * the current locale is used to interpret the C++ string. When
         * encoding is \link Wt::UTF8 UTF8\endlink, the C++ string is
         * interpreted as a UTF8 encoded unicode string.
         *
         * String::setDefaultEncoding() controls the behaviour of
         * \link Wt::DefaultEncoding DefaultEncoding\endlink. Use it to set a
         * system-wide default format for C style strings (e.g. to UTF-8).
         
         */
        String(const std::string& value, CharEncoding encoding = DefaultEncoding);
        
        /*! \brief Creates a %String from a C++ string.
         *
         * The C++ string is implicitly converted to unicode. The
         * string is interpreted within the character set of the given locale.
         */
        String(const std::string& value, const std::locale &loc);
        
        /*! \brief Destructor
         */
        ~String();
        
        String trim() const;
        
        /*! \brief Assignment operator
         *
         * Copy another string into this string.
         */
        String& operator= (const String& rhs);
        
        /*! \brief Comparison operator
         *
         * Compares two strings and returns \c true if the strings are exactly
         * the same. This may require evaluating a localized string in the
         * current locale.
         */
        bool operator== (const String& rhs) const;
        
        /*! \brief Comparison operator
         *
         * Compares to strings lexicographically. This may require
         * evaluating a localized string in the current locale. The unicode
         * representation of the strings are compared.
         */
        bool operator< (const String& rhs) const;
        
        /*! \brief Comparison operator
         *
         * Compares to strings lexicographically. This may require
         * evaluating a localized string in the current locale. The unicode
         * representation of the strings are compared.
         */
        bool operator> (const String& rhs) const;
        
        int compareTo(const String& rhs) const;
        
        /*! \brief Self-concatenation operator
         *
         * Appends a string to the current value. If the string was localized,
         * this automatically converts it to a literal string, by evaluating the
         * string using the current locale.
         */
        String& operator+= (const String& rhs);
        
        /*! \brief Self-concatenation operator
         *
         * Appends a string to the current value. If the string was localized,
         * this automatically converts it to a literal string, by evaluating the
         * string using the current locale.
         */
        String& operator+= (const std::wstring& rhs);
        
        /*! \brief Self-concatenation operator
         *
         * Appends a string to the current value. If the string was localized,
         * this automatically converts it to a literal string, by evaluating the
         * string using the current locale.
         */
        String& operator+= (const wchar_t *rhs);
        
        /*! \brief Self-concatenation operator
         *
         * Appends a string to the current value. The right hand side is
         * interpreted in the server locale and converted to unicode. If the
         * string was localized, this automatically converts it to a literal
         * string, by evaluating the string using the current locale.
         */
        String& operator+= (const std::string& rhs);
        
        /*! \brief Self-concatenation operator
         *
         * Appends a string to the current value. The right hand side is
         * interpreted in the server locale and converted to unicode. If the
         * string was localized, this automatically converts it to a literal
         * string, by evaluating the string using the current locale.
         */
        String& operator+= (const char *rhs);
        
        /*! \brief Returns whether the string is empty.
         */
        bool empty() const;
        
        /*! \brief Creates a %String from a UTF8 unicode encoded string.
         *
         * This is equivalent to using the constructor String(\p value,
         * UTF8).
         *
         * When \p checkValid is \c true, the UTF8 encoding is validated. You
         * should enable this only if you cannot trust the origin of the string.
         * The library uses this internally whenever it receives data from the
         * browser (in UTF-8 format).
         */
        static String fromUTF8(const std::string& value, bool checkValid = false);
        
        /*! \brief Creates a %String from a UTF8 unicode encoded string.
         *
         * This is equivalent to using the constructor String(\p value,
         * UTF8).
         *
         * When \p checkValid is \c true, the UTF8 encoding is validated. You
         * should enable this only if you cannot trust the origin of the string.
         * The library uses this internally whenever it receives data from the
         * browser (in UTF-8 format).
         */
        static String fromUTF8(const char *value, bool checkValid = false);
        
        /*! \brief Returns the value as a UTF8 unicode encoded string.
         *
         * For a localized string, returns the current localized value.
         *
         * \sa fromUTF8()
         */
        std::string toUTF8() const;
        
        /*! \brief Creates a localized string from a key.
         *
         * Whenever the value of the string is needed, the key is used for a
         * lookup in the application message resource bundles taking into
         * account the current application locale. If the key cannot be
         * resolved, its value is set to '??key??'.
         *
         * \sa WApplication::locale(), WApplication::localizedStrings()
         */
        static String tr(const char *key);
        
        /*! \brief Creates a localized string with the specified key.
         *
         * \sa tr(const char *)
         */
        static String tr(const std::string& key);
        
        /*! \brief Creates a localized string from a key for a number \p n.
         *
         * Whenever the value of the string is needed, the \p key is used
         * for a lookup in the application message resource bundles taking
         * into account the current application locale. This function
         * fetches the appropriate plural case for the translation
         * corresponding to the quantity \p n. Note that usually, your
         * string will have a place-holder for the value of \p n, and thus
         * you will also need to bind \p as an argument.
         *
         * For example, consider a string "quantity.cars" with two plural cases:
         * - n == 1: "{1} car"
         * - n != 1: "{1} cars"
         *
         * You would use the following to use the string:
         * \if cpp
         * \code
         * Wt::String::trn("quantity.cars", cars).arg(cars);
         * \endcode
         * \else
         * \code
         * String.trn("quantity.cars", cars).arg(cars);
         * \endcode
         * \endif
         *
         * If the \p key cannot be resolved, its value is set to '??key??'.
         *
         * \sa tr()
         */
        static String trn(const char *key, ::uint64_t n);
        
        /*! \brief Creates a localized string with the specified key for a
         number \c n.
         *
         * \sa trn(const char *)
         */
        static String trn(const std::string& key, ::uint64_t n);
        
        /*! \brief Returns the value as a wide C++ string.
         *
         * A localized string is resolved using the WApplication::localizedStrings().
         *
         * Argument place holders are substitued with actual arguments.
         */
        std::wstring value() const;
        
        /*! \brief Returns the value as a narrow C++ string.
         *
         * A localized string is resolved using the WApplication::localizedStrings().
         *
         * Argument place holders are substitued with actual arguments.
         *
         * Any wide character is narrowed using the provided locale, possibly
         * losing information. If you wish to keep all information, use toUTF8()
         * instead, which encodes wide characters in the string.
         *
         * \sa toUTF8()
         */
        std::string narrow(const std::locale &loc = std::locale()) const;
        
        /*! \brief Returns the value as a wide C++ string.
         *
         * A localized string is resolved using the WApplication::localizedStrings().
         *
         * Argument place holders are substitued with actual arguments.
         */
        operator std::wstring() const;
        
        /*! \brief Retuns whether the string is literal or localized.
         *
         * \sa tr()
         */
        bool literal() const { return !impl_ || impl_->key_.empty(); }
        
        /*! \brief Returns the key for a localized string.
         *
         * When the string is literal, the result is undefined.
         */
        const std::string key() const;
        
        /*! \brief Substitutes the next positional argument with a string value.
         *
         * In the string, the \p n-th argument is referred to as using
         * {\p n}.
         *
         * For example: the string "<tt>{1} bought {2} apples in the
         * shop.</tt>" with first argument value "<tt>Bart</tt>" and second
         * argument value <tt>5</tt> becomes: "<tt>Bart bought 5 apples in
         * the shop.</tt>"
         */
        String& arg(const std::wstring& value);
        
        /*! \brief Substitutes the next positional argument with a string value.
         *
         * In the string, the \p n-th argument is referred to as using
         * {\p n}.
         *
         * For example: the string "<tt>{1} bought {2} apples in the
         * shop.</tt>" with first argument value "<tt>Bart</tt>" and second
         * argument value <tt>5</tt> becomes: "<tt>Bart bought 5 apples in
         * the shop.</tt>"
         */
        String& arg(const std::string& value, CharEncoding = DefaultEncoding);
        
        String& arg(const char *value, CharEncoding = DefaultEncoding);
        
        /*! \brief Substitutes the next positional argument with a string value.
         *
         * In the string, the \p n-th argument is referred to as using
         * {\p n}.
         *
         * For example: the string "<tt>{1} bought {2} apples in the
         * shop.</tt>" with first argument value "<tt>Bart</tt>" and second
         * argument value <tt>5</tt> becomes: "<tt>Bart bought 5 apples in
         * the shop.</tt>"
         */
        String& arg(const String& value);
        
        /*! \brief Substitutes the next positional argument with an integer value.
         *
         * In the string, the \p n-th argument is reffered to as using
         * {\p n}.
         *
         * For example: the string "<tt>{1} bought {2} apples in the
         * shop.</tt>" with first argument value "<tt>Bart</tt>" and second
         * argument value <tt>5</tt> becomes: "<tt>Bart bought 5 apples in
         * the shop.</tt>"
         */
        String& arg(int value);
        
        /*! \brief Substitutes the next positional argument with an unsigned value.
         *
         * \sa arg()
         */
        String& arg(unsigned value);
        
        /*! \brief Substitutes the next positional argument with an integer value.
         *
         * In the string, the \p n-th argument is reffered to as using
         * {\p n}.
         *
         * For example: the string "<tt>{1} bought {2} apples in the
         * shop.</tt>" with first argument value "<tt>Bart</tt>" and second
         * argument value <tt>5</tt> becomes: "<tt>Bart bought 5 apples in
         * the shop.</tt>"
         */
        String& arg(::int64_t value);
        
        /*! \brief Substitutes the next positional argument with an unsigned value.
         *
         * \sa arg()
         */
        String& arg(::uint64_t value);
        
        /*! \brief Substitutes the next positional argument with a double value.
         *
         * In the string, the \p n-th argument is reffered to as using
         * {\p n}.
         *
         * For example: the string "<tt>{1} bought {2} apples in the
         * shop.</tt>" with first argument value "<tt>Bart</tt>" and second
         * argument value <tt>5</tt> becomes: "<tt>Bart bought 5 apples in
         * the shop.</tt>"
         */
        String& arg(double value);
        
        /*! \brief Returns the list of arguments
         */
        const std::vector<String>& args() const;
        
        /*! \brief Refreshes the string.
         *
         * For a localized string, its value is resolved again.
         *
         * Returns whether the value has (potentially) changed.
         */
        bool refresh();
        
        /*! \brief Returns the string as a JavaScript literal
         *
         * The \p delimiter may be a single or double quote.
         *
         * \sa WWebWidget::jsStringLiteral()
         */
        std::string jsStringLiteral(char delimiter = '\'') const;
        
#ifdef WT_CNOR
        String& operator+(const char *);
        String& operator+(const String &);
#endif
        
        /*! \brief Comparison operator
         *
         * Compares two strings and returns \c true if the strings are not exactly
         * the same. This may require evaluating a localized string in the
         * current locale.
         */
        bool operator!= (const String& rhs) const { return !(*this == rhs); }
        
        /** \brief An empty string.
         */
        static const String Empty;
        
        static void checkUTF8Encoding(std::string& value);
        
    private:
        String(const char *key, bool, ::uint64_t n = -1);
        
        std::string utf8_;
        
        std::string resolveKey() const;
        
        void makeLiteral();
        
        struct Impl {
            std::string key_;
            std::vector<String> arguments_;
            ::int64_t n_;
            
            Impl();
        };
        
        static std::vector<String> stArguments_;
        
        void createImpl();
        
        Impl *impl_;
        static CharEncoding defaultEncoding_;
        static CharEncoding realEncoding(CharEncoding encoding);
    };
    
    
    /* \brief Short hand for String(const char * value, UTF8)
     *
     * \relates String
     */
    extern String utf8(const char *value);
    
    /* \brief Short hand for String(const std::string& value, UTF8)
     *
     * \relates String
     */
    extern String utf8(const std::string& value);
    
    /*! \brief Concatenate two Strings
     *
     * \relates String
     */
    extern String operator+ (const String& lhs, const String& rhs);
    
    /*! \brief Conatenate a String with a C++ wide string
     *
     * \relates String
     */
    extern String operator+ (const String& lhs, const std::wstring& rhs);
    
    /*! \brief Conatenate a String with a C wide string
     *
     * \relates String
     */
    extern String operator+ (const String& lhs, const wchar_t *rhs);
    
    /*! \brief Conatenate a WStrin with a C++ string
     *
     * \relates String
     */
    extern String operator+ (const String& lhs, const std::string& rhs);
    
    /*! \brief Conatenate a String with a C string
     *
     * \relates String
     */
    extern String operator+ (const String& lhs, const char *rhs);
    
    /*! \brief Conatenate a C++ wide string with a String
     *
     * \relates String
     */
    extern String operator+ (const std::wstring& lhs, const String& rhs);
    
    /*! \brief Conatenate a C wide string with a String
     *
     * \relates String
     */
    extern String operator+ (const wchar_t *lhs, const String& rhs);
    
    /*! \brief Conatenate a C++ string with a String
     *
     * \relates String
     */
    extern String operator+ (const std::string& lhs, const String& rhs);
    
    /*! \brief Conatenate a C string with a String
     *
     * \relates String
     */
    extern String operator+ (const char *lhs, const String& rhs);
    
    /*! \brief Compare a C string with a String
     *
     * \relates String
     */
    extern bool operator== (const char *lhs, const String& rhs);
    
    /*! \brief Compare a C wide string with a String
     *
     * \relates String
     */
    extern bool operator== (const wchar_t *lhs, const String& rhs);
    
    /*! \brief Compare a C++ string with a String
     *
     * \relates String
     */
    extern bool operator== (const std::string& lhs, const String& rhs);
    
    /*! \brief Compare a C++ wide string with a String
     *
     * \relates String
     */
    extern bool operator== (const std::wstring& lhs, const String& rhs);
    
    /*! \brief Compare a C string with a String
     *
     * \relates String
     */
    extern bool operator!= (const char *lhs, const String& rhs);
    
    /*! \brief Compare a C wide string with a String
     *
     * \relates String
     */
    extern bool operator!= (const wchar_t *lhs, const String& rhs);
    
    /*! \brief Compare a C++ string with a String
     *
     * \relates String
     */
    extern bool operator!= (const std::string& lhs, const String& rhs);
    
    /*! \brief Compare a C++ wide string with a String
     *
     * \relates String
     */
    extern bool operator!= (const std::wstring& lhs, const String& rhs);
    
    /*! \brief Output a String to a C++ wide stream
     *
     * \relates String
     */
    extern std::wostream& operator<< (std::wostream& lhs, const String& rhs);
    
    /*! \brief Output a String to a C++ stream
     *
     * The string is narrowed using the currently global C++ locale, possibly
     * losing information.
     * \relates String
     */
    extern std::ostream& operator<< (std::ostream& lhs, const String& rhs);
}

#endif /* defined(__SmartAgent__String__) */
