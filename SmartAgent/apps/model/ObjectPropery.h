//
//  SessionModel.h
//  SmartAgent
//
//  Created by Евгений Илюшин on 19.12.14.
//  Copyright (c) 2014 Евгений Илюшин. All rights reserved.
//

#ifndef __SmartAgent__ObjectPropery__
#define __SmartAgent__ObjectPropery__

#include <iostream>
#include <string>
#include <map>
#include <boost/any.hpp>

namespace Apps {

    class BaseApp;

    namespace Model {
        
        typedef std::map<std::string, boost::any> PropertyMap;
        
        class ObjectPropery
        {
        public:
            enum PropertyType
            {
                String = 1,
                Numeric = 2,
                Boolean = 3,
                Date = 4,
                Dictionary = 5, // Ссылка на элемент справочника
                DictionaryList = 6 // По этой ссылки есть несколько записей в справчнике
            };
            
            ObjectPropery(std::string onjectName);
            boost::any Property(const std::string &propertyName,const std::string &
                                objectRef);
            virtual inline const std::string &id() const;
        protected:
            std::string m_objectName;
            
        private:
            void init(const std::string &objRef) const;
            mutable PropertyMap m_property;
            mutable std::string m_ref;
            
        };
        
    }
}
#endif /* defined(__SmartAgent__ObjectPropery__) */
