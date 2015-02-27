//
//  DataEventModelBase.h
//  SmartAgent
//
//  Created by Евгений Илюшин on 06.02.15.
//  Copyright (c) 2015 Евгений Илюшин. All rights reserved.
//

#ifndef __SmartAgent__DataEventModelBase__
#define __SmartAgent__DataEventModelBase__

#include "../../data/datastruct.h"

#include <cppdb/frontend.h>

namespace Apps {
    
    
    namespace Model {
        class DataEventModelBase{
        public:
            DataEventModelBase();
            ~DataEventModelBase();
            virtual Data::ErrorItemList addValuesByCode(const Data::ImportItemList &il);
            virtual Data::ErrorItemList addValuesByRef(const Data::ImportDoc &doc);
            virtual int getEventID();
        private:
            
            
            cppdb::session &m_session;
            int m_id;
        };
    }
}

#endif /* defined(__SmartAgent__DataEventModelBase__) */
