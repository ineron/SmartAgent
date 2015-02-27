//
//  DataImportBase.cpp
//  SmartAgent
//
//  Created by Евгений Илюшин on 05.02.15.
//  Copyright (c) 2015 Евгений Илюшин. All rights reserved.
//

#include "DataImportBase.h"

#include <boost/tokenizer.hpp>
#include <htmlcxx/html/CharsetConverter.h>
#include <boost/algorithm/string/replace.hpp>
#include <boost/any.hpp>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>


namespace Utils {
    
    DataImportBase::DataImportBase():m_splitter(0) ,m_codepage(0)
    {
    
    }
    DataImportBase::~DataImportBase()
    {
    };
    
    void DataImportBase::loadFromText(std::istream &stream )
    {
        
        int csvRow = 0;
        
        m_importItemList.clear();

        
        char chSepparator = '\t';
        
        switch(m_splitter)
        {
            case 0: chSepparator = '\t';
                break;
            case 1: chSepparator = ';';
                break;
            case 2: chSepparator = ',';
                break;
            case 3: chSepparator = '`';
                break;
            case 4: chSepparator = '~';
                break;
        }
        
        std::istringstream is;
        //std::ostringstream os;
        
        
        //is = stream;
        
        //copy( std::istreambuf_iterator<char>(stream),
        //     std::istreambuf_iterator<char>(),
        //     std::istreambuf_iterator<char>(is));
             
        std::string line;
        while (std::getline(stream, line))
        {

            //std::cout << line <<std::endl;
            if(m_codepage)
            {
                htmlcxx::CharsetConverter cc("WINDOWS-1251","UTF-8");
                boost::replace_all( line, "\r", "" );
                line = cc.convert(line);
            }
            
            boost::escaped_list_separator<char> separator('\\',chSepparator,'\"');
            
            typedef boost::tokenizer<boost::escaped_list_separator<char> > CsvTokenizer;
            
            CsvTokenizer tok(line,separator);
            int col = 0;
            Data::ImportItem item;
            for (CsvTokenizer::iterator i = tok.begin();
                 i != tok.end(); ++i, ++col)
            {
                if(col)
                    item.value = *i;
                else
                    item.code = *i;
            }
            m_importItemList.push_back(item);
        }
        
    }
    void DataImportBase::loadFromText(const std::string &text )
    {
        std::istringstream striam(text);
        loadFromText( striam );
    }
    void DataImportBase::loadFromCSV(const std::string &filename )
    {
        std::ifstream fin(filename);
        loadFromText(fin);
        fin.close();
    }
    
    std::ostream & operator << (DataImportBase  const & DIB, std::ostream & outs)
    {
        return outs;
    }
    
    std::istream & operator >> (std::istream &is, DataImportBase  const & DIB)
    {
        return is;
    }
}