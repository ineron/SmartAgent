#ifndef TMPL_REPORTS_H
#define TMPL_REPORTS_H
#include <cppcms/view.h>
#include <cppcms/http_context.h>
#include <cppcms/json.h>

namespace Data {
    
    


    typedef std::vector<DataRowItem> ReportRows;

    //-------------------------------------------------------------------------------------
    // Dsc: Новостной контент
    //-------------------------------------------------------------------------------------
    struct Reports :public cppcms::base_content{
        //-------------------------------------------------------------------------------------
        // Dsc: Главная новость
        //-------------------------------------------------------------------------------------
        int user_level;
        bool is_DateRange;
        std::string reportName;
        std::vector<DataRowItem>reportDataTable;
        std::vector<ParamItem>paramList;
        std::vector<std::string>dataBindList;
        std::vector<std::string>headerItemsList;
        std::vector<std::string>ajaxParamList;
        std::vector<std::string>dataRenderList;
        std::vector<ParamItem>userList;
        
        
        //-------------------------------------------------------------------------------------
        // Dsc: Конструктор страницы
        //-------------------------------------------------------------------------------------
        Reports():user_level(0),is_DateRange(false) //:
        //Master()
        {}
        //-------------------------------------------------------------------------------------
        // Dsc: Ленивый деструктор
        //-------------------------------------------------------------------------------------
        ~Reports(){}
    };
    
}





#endif // TMPL_REPORTS_H
