//
//  DataEventModelBase.cpp
//  SmartAgent
//
//  Created by Евгений Илюшин on 06.02.15.
//  Copyright (c) 2015 Евгений Илюшин. All rights reserved.
//

#include "../BaseApp.h"
#include "DataEventModelBase.h"
#include "ObjectPropery.h"

#include <boost/algorithm/string/replace.hpp>
#include <boost/any.hpp>


namespace Apps {
    
    
    namespace Model {
        
        DataEventModelBase::DataEventModelBase():m_session(Apps::BaseApp::instance()->dbsession()),m_id(0)
        {
            
        }
        
        DataEventModelBase::~DataEventModelBase()
        {
            
        }
        Data::ErrorItemList DataEventModelBase::addValuesByCode(const Data::ImportItemList &il)
        {
 
            Data::ErrorItemList errors;

            if (!Apps::BaseApp::instance()->loginLevel()) {
                return errors;
            }
            std::string userRef = BaseApp::instance()->currentUserRef();
            std::string usrCode=boost::any_cast<std::string>(BaseApp::instance()->getUser()->Property("Код 1С",userRef));
;
            std::string codeList="insert into tmppdoc_"+usrCode+" values \n";
            std::string masterQuery = "CREATE TEMPORARY TABLE  tmppdoc_"+usrCode+"  "
            "(code text, "
            "quantity numeric(15)) "
            "ON COMMIT DROP; \n";
            
            std::string query = "";
            for (auto i=il.begin(); i!=il.end(); ++i)
            {
                std::string code = i->code;
                std::string count = i->value;
                boost::replace_all( code, "\"", "" );
                boost::replace_all( code, "'", "" );
                boost::replace_all( code, """", "" );
                if(code.empty())
                    continue;
                boost::replace_all( count, ",", "." );
                boost::replace_all( count, " ", "" );
                boost::replace_all( count, " ", "" );
                if(!count.find("#"))
                    continue;
                if(count.empty())
                    count = "0";
                query += "[E'"+code+"''::text,'"+count+"'::text],";
                codeList += "('"+code+"',"+count+"),";
                
            }
            query.erase(query.end()-1,query.end());
            codeList.erase(codeList.end()-1,codeList.end());
            
            cppdb::transaction tr(m_session);
            cppdb::result result;
            m_session<<masterQuery<<cppdb::exec;;
            m_session<<codeList<<cppdb::exec;;
            
            
            masterQuery = "select code,errors from add_order_records(?) as (code text ,errors int)";
            result = m_session << masterQuery << userRef;
            
            
            while (result.next())
            {
                Data::ErrorItem item;
                result>>item.code>>item.errors;
                errors.push_back(item);
            }
            result = m_session<<"SELECT CURRVAL(pg_get_serial_sequence('orders_header','order_id'))"<<cppdb::row;
            if(!result.empty())
                m_id=result.get<int>(0);
            
            tr.commit();
            
            return errors;
        }

        Data::ErrorItemList DataEventModelBase::addValuesByRef(const Data::ImportDoc &doc)
        {
            
            Data::ErrorItemList errors;
            
            if (!Apps::BaseApp::instance()->loginLevel()) {
                return errors;
            }
            std::string userRef = BaseApp::instance()->currentUserRef();
            std::string usrCode=boost::any_cast<std::string>(BaseApp::instance()->getUser()->Property("Код 1С",doc.author));
            ;
            std::string codeList="insert into tmppdoc_"+usrCode+" values \n";
            std::string masterQuery = "CREATE TEMPORARY TABLE  tmppdoc_"+usrCode+"  "
            "(code text, "
            "ref uuid,"
            "quantity numeric(15)) "
            "ON COMMIT DROP; \n";
            
            //std::string query = "";
            for (auto i=doc.itemList.begin(); i!=doc.itemList.end(); ++i)
            {
                std::string ref = i->ref;
                std::string count = i->value;
                //query += "[E'"+ref+"''::text,'"+count+"'::text],";
                codeList += "(null,'"+ref+"',"+count+"),";
                
            }
            //query.erase(query.end()-1,query.end());$('meta[name=useref]').attr("rel")
            codeList.erase(codeList.end()-1,codeList.end());
            
            cppdb::transaction tr(m_session);
            cppdb::result result;
            m_session<<masterQuery<<cppdb::exec;;
            m_session<<codeList<<cppdb::exec;;
            
            std::cout<<__LINE__<<std::endl;
            
            masterQuery = "select code,errors from add_order_records(?,?) as (code text ,errors int)";
            result = m_session << masterQuery << userRef<< "ref";
            
            std::cout<<__LINE__<<std::endl;
            
            while (result.next())
            {
                Data::ErrorItem item;
                result>>item.code>>item.errors;
                errors.push_back(item);
            }
            result = m_session<<"SELECT CURRVAL(pg_get_serial_sequence('orders_header','order_id'));"<<cppdb::row;
            std::cout<<__LINE__<<std::endl;
            if(!result.empty())
                m_id=result.get<int>(0);
            
            std::cout<<__LINE__<<std::endl;
            if(m_id)
            {
                result = m_session<<"update orders_header set address_ref = ? , user_ref =  ? where order_id = ? returning ref"<<doc.addres<<doc.user<<m_id<<cppdb::row;
            }
            if(!doc.comment.empty())
            {
                if(!result.empty())
                {
                    m_session<<"insert into comments(orders_header_ref,comment)  values(?,?)"<<result.get<std::string>(0)<<doc.comment<<cppdb::exec;
                }
            }
            
            std::cout<<__LINE__<<std::endl;
            
            tr.commit();
            
            return errors;
        }
        
        
        int DataEventModelBase::getEventID()
        {
            return m_id;
        }
    }
}