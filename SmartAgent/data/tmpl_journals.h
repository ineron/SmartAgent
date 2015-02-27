#ifndef TMPL_JOURNALS_H
#define TMPL_JOURNALS_H

#include "datastruct.h"

#include <cppcms/view.h>
#include <cppcms/http_context.h>
#include <cppcms/json.h>

namespace Data {
    
    
    typedef std::vector<DataRowItem> JournalsRows;

    //-------------------------------------------------------------------------------------
    // Dsc: Новостной контент
    //-------------------------------------------------------------------------------------
    struct Journals :public cppcms::base_content{
        //-------------------------------------------------------------------------------------
        // Dsc: Главная новость
        //-------------------------------------------------------------------------------------
        int user_level;
        bool is_DateRange;
        bool hasNewDoc;
        std::string journalName;
        std::vector<DataRowItem>DataTable;
        std::vector<ParamItem>paramList;
        std::vector<ParamItem>userList;
        std::vector<ParamItem>addressList;
        std::vector<std::string>dataBindList;
        std::vector<std::string>headerItemsList;
        std::vector<std::string>ajaxParamList;
        std::vector<std::string>dataRenderList;
        
        //-------------------------------------------------------------------------------------
        // Dsc: Конструктор страницы
        //-------------------------------------------------------------------------------------
        Journals():user_level(0),is_DateRange(false),hasNewDoc(false) //:
        //Master()
        {}
        //-------------------------------------------------------------------------------------
        // Dsc: Ленивый деструктор
        //-------------------------------------------------------------------------------------
        ~Journals(){}
    };
    
}


#endif // TMPL_JOURNALS_H
