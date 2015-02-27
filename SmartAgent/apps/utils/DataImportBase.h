//
//  DataImportBase.h
//  SmartAgent
//
//  Created by Евгений Илюшин on 05.02.15.
//  Copyright (c) 2015 Евгений Илюшин. All rights reserved.
//

#ifndef __SmartAgent__DataImportBase__
#define __SmartAgent__DataImportBase__

#include "../../data/datastruct.h"
#include <iostream>
#include <sstream>

namespace Utils {
    class DataImportBase
    {
    public:
        DataImportBase();
        ~DataImportBase();
        
        virtual void loadFromText( std::istream &stream);
        virtual void loadFromCSV(const std::string &filename);
        virtual void loadFromText(const std::string &text );
        virtual Data::ImportItemList getItemList() const {return m_importItemList;};
        DataImportBase& splitter(const int &spl) {m_splitter = spl; return *this;};
        DataImportBase& codepage(const int &cp) {m_codepage = cp;return *this;};
    private:
        Data::ImportItemList m_importItemList;
        int m_splitter, m_codepage;
        
        friend std::ostream & operator << (DataImportBase  const & DIB, std::ostream & outs);
        friend std::istream & operator >> (std::istream &is, DataImportBase  const & DIB);
    };
}

#endif /* defined(__SmartAgent__DataImportBase__) */
