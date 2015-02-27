//
//  SmartAgent.cpp
//  SmartAgent
//
//  Created by Евгений Илюшин on 16.10.14.
//  Copyright (c) 2014 Евгений Илюшин. All rights reserved.
//

#include <algorithm>
#include <fstream>
#include <time.h>
#include <map>

#include "../data/tmpl.h"


#include "model/ObjectPropery.h"
#include "model/DataEventModelBase.h"

#include "utils/utils.h"
#include "utils/DataImportBase.h"

#include "SmartAgent.h"
#include "auth/HashFunction.h"
#include "utils/WSClient.h"
#include "utils/String.h"

#include <cppcms/json.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/http_file.h>
#include <cppcms/url_mapper.h>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
// TODO: dsdsd


namespace Apps {
    
    
    
    //-------------------------------------------------------------------------------------
    // Dsc: Конструктор, который будет запушен во время старта программы
    //-------------------------------------------------------------------------------------
    SmartAgent::SmartAgent(cppcms::service &s) : BaseApp(s)
    {
        // TODO: 4444
        // !!!: fdf
        // Открываем соединение с базой данных
        //url.add("^/page/(\\d+)$",boost::bind(&app::page,this,_1));
        
        setUser(new Model::ObjectPropery("user"));
        initApp();
    }
    
    void SmartAgent::initApp()
    {
        
        
        dispatcher().assign("/dashboard(.*)",&SmartAgent::dashboard,this,1);
        mapper().assign("dashboard","/");
        
        dispatcher().assign("/login",&SmartAgent::login,this,1);
        
        dispatcher().assign("/logout",&SmartAgent::logout,this);
        
        dispatcher().assign("/news",&SmartAgent::news,this,1);
        mapper().assign("news","/");
        
        dispatcher().assign("/reports/(\\d+|\\D+)",&SmartAgent::reports,this,1);
        mapper().assign("reports","/reports/{1}");
        
        dispatcher().assign("/doc/(\\d+|\\D+)",&SmartAgent::docs,this,1);
        mapper().assign("doc","/doc/{1}");
        
        
        //dispatcher().assign("^/search/?(=*?)/?$",&SmartAgent::search,this,1,2);
        dispatcher().assign("/search/?(=*?)/?",&SmartAgent::search,this,1,2);
        mapper().assign("search","/search(1)");
        //dispatcher().mount("/news((/.*)?)",&SmartAgent::search,1);
        //(\\d+|\\D++)$
        
        dispatcher().assign("/invoicelist",&SmartAgent::invoiceList,this,1);
        mapper().assign("invoicelist","/");
        
        dispatcher().assign("/invoice",&SmartAgent::invoce,this,1);
        mapper().assign("invoice","/");
        
        dispatcher().assign("/profile",&SmartAgent::profile,this,1);
        mapper().assign("profile","/");
        
        dispatcher().assign("/catalog/(\\d+|\\D+)",&SmartAgent::catalog,this,1,2,3,4);
        mapper().assign("catalog","/catalog/{1}/");
        
        dispatcher().assign("/catalog-data/?(=*?)/?",&SmartAgent::catalog,this,1);
        mapper().assign("catalog-data","/catalog-data");
        
        dispatcher().assign("/shopcart-data/?(=*?)/?",&SmartAgent::catalogData,this,1);
        //mapper().assign("shopcart-data","/shopcart-data");
        
        
        dispatcher().assign("(/?)",&SmartAgent::master,this,1);
        mapper().assign("master","/");
        
        dispatcher().assign("/search-data/?(=*?)/?",&SmartAgent::searchData,this);
        dispatcher().assign("/shopcart",&SmartAgent::shopcart,this);
        dispatcher().assign("/calendar",&SmartAgent::calendar,this);
        dispatcher().assign("/report-data",&SmartAgent::reportsData,this);
        dispatcher().assign("/journal-data",&SmartAgent::docsData,this);
        dispatcher().assign("/journal-data-table",&SmartAgent::docsDataTable,this);
        
        dispatcher().assign("/service/import",&SmartAgent::service,this);
        dispatcher().assign("/service/settings",&SmartAgent::settings,this);
        //dispatcher().assign("/service-data",&SmartAgent::docsData,this);
        
        dispatcher().assign("/upload",&SmartAgent::upload,this);
        dispatcher().assign("/textimport",&SmartAgent::textImport,this);

        dispatcher().assign("/messages",&SmartAgent::messages,this);
        dispatcher().assign("/messages-data",&SmartAgent::messagesData,this);
        dispatcher().assign("/messages/list",&SmartAgent::messageList,this);
        dispatcher().assign("/messages/compose",&SmartAgent::messageCompose,this);
        dispatcher().assign("/messages/read",&SmartAgent::messageRead,this);
        dispatcher().assign("/messages/reply",&SmartAgent::messageReply,this);
        dispatcher().assign("/messages/read/attachment",&SmartAgent::attachmentDownload,this);
        dispatcher().assign("/send-order",&SmartAgent::sendOrder,this);
        dispatcher().assign("/event-data",&SmartAgent::events,this);

        
        
        
    }
    //-------------------------------------------------------------------------------------
    // Dsc: Деструктор
    //-------------------------------------------------------------------------------------
    SmartAgent::~SmartAgent()
    {
        
    };
    //-------------------------------------------------------------------------------------
    // Dsc: Функция в которую мы попадем, если иного не указано в конструкторе
    //      ( об этом позже )
    //-------------------------------------------------------------------------------------
    void SmartAgent::main(std::string path)
    {
        cppcms::application::main(path);
    }
    //-------------------------------------------------------------------------------------
    // Dsc: Рендеринг базового контента
    //-------------------------------------------------------------------------------------
    void SmartAgent::accordion(Data::Master &tmpl)
    {
        
        if(loginLevel()){
            std::string query;
            query = INLINE_SQLSCRIPT
            (
             select sum(inbox),sum(allmsg) from
             (
              select count(1) inbox,0 allmsg from messages where state = 0 and author_ref = ?
              union all
              select 0 inbox,count(1) allmsg from messages where author_ref = ?
              )a
             );
            //cppdb::transaction tr(m_session);
            cppdb::result result;
            result = m_session << query << m_currentUser<<m_currentUser<<cppdb::row;
            if (!result.empty()) {
                tmpl.newMsgCount = result.get<int>(0);
            }
        }
        // TODO: 123
        {
            Data::Master::MenuStruct item;
            item.url = "/dashboard.html";
            item.title = "Главная";
            item.style = "menu-item-parent";
            item.icon = "fa fa-lg fa-fw fa-home";
            tmpl.menuList.push_back(item);
        }
        
        if(BaseApp::loginLevel())
        {
            Data::Master::MenuStruct item;
            item.url = "/messages";
            item.title = "Сообщения";
            item.style = "menu-item-parent";
            item.icon = "fa fa-lg fa-fw fa-inbox";
            item.badge = boost::lexical_cast<std::string>(tmpl.newMsgCount);
            tmpl.menuList.push_back(item);
        }
        
        if(BaseApp::loginLevel())
        {
            Data::Master::MenuStruct item;
            item.url = "#";
            item.title = "Отчеты";
            item.style = "menu-item-parent";
            item.icon = "fa fa-lg fa-fw fa-table";
            
            
            {
                Data::Master::MenuStruct subitem;
                subitem.url = "/reports/mutual";
                subitem.title = "Взаиморасчеты";
                subitem.style = "menu-item-parent";
                //subitem.icon = "ffa fa-lg fa-fw fa-table";
                item.menu.push_back(subitem);
            }
            {
                Data::Master::MenuStruct subitem;
                subitem.url = "/reports/dept-structure";
                subitem.title = "Структура долга";
                subitem.style = "menu-item-parent";
                //subitem.icon = "fa fa-lg fa-fw fa-table";
                item.menu.push_back(subitem);
            }
            {
                Data::Master::MenuStruct subitem;
                subitem.url = "/reports/orders-shipping";
                subitem.title = "Заказы отгрузки";
                subitem.style = "menu-item-parent";
                //subitem.icon = "fa fa-lg fa-fw fa-inbox";
                item.menu.push_back(subitem);
            }
            
            
            tmpl.menuList.push_back(item);
            
            
        }
        if(BaseApp::loginLevel())
        {
            Data::Master::MenuStruct item;
            item.url = "#";
            item.title = "Документы";
            item.style = "menu-item-parent";
            item.icon = "fa fa-lg fa-fw fa-book txt-color-green";
            
            
            {
                Data::Master::MenuStruct subitem;
                subitem.url = "/doc/invoice";
                subitem.title = "Заказы покупателя";
                subitem.style = "menu-item-parent";
                subitem.icon = "fa fa-lg fa-fw fa-shopping-cart";
                item.menu.push_back(subitem);
            }
            {
                Data::Master::MenuStruct subitem;
                subitem.url = "doc/sale";
                subitem.title = "Расходные наладные";
                subitem.style = "menu-item-parent";
                subitem.icon = "fa fa-lg fa-fw fa-truck";
                //item.menu.push_back(subitem);
            }
            {
                Data::Master::MenuStruct subitem;
                subitem.url = "/doc/payment";
                subitem.title = "Платежные док-ты";
                subitem.style = "menu-item-parent";
                subitem.icon = "fa fa-lg fa-fw fa-rouble";
                //item.menu.push_back(subitem);
            }
            
            
            tmpl.menuList.push_back(item);
            
            
        }
        {   /* КАТАЛОГ */
            Data::Master::MenuStruct item;
            item.url = "#";
            item.title = "Каталог";
            item.style = "menu-item-parent";
            item.icon = "fa fa-lg fa-fw fa-cube txt-color-blue";
            
            
            {
                Data::Master::MenuStruct subitem;
                subitem.url = "/catalog/FIT";
                subitem.title = "FIT";
                subitem.style = "menu-item-parent";
                //subitem.icon = "fa fa-lg fa-fw fa-inbox";
                item.menu.push_back(subitem);
            }
            {
                Data::Master::MenuStruct subitem;
                subitem.url = "/catalog/DWT";
                subitem.title = "DWT";
                subitem.style = "menu-item-parent";
                //subitem.icon = "fa fa-lg fa-fw fa-inbox";
                item.menu.push_back(subitem);
            }
            {
                Data::Master::MenuStruct subitem;
                subitem.url = "/catalog/KF";
                subitem.title = "КФ";
                subitem.style = "menu-item-parent";
                //subitem.icon = "fa fa-lg fa-fw fa-inbox";
                item.menu.push_back(subitem);
            }
            {
                Data::Master::MenuStruct subitem;
                subitem.url = "/catalog/SALE";
                subitem.title = "Распрадажа";
                subitem.style = "menu-item-parent";
                //subitem.icon = "fa fa-lg fa-fw fa-inbox";
                item.menu.push_back(subitem);
            }
            
            
            tmpl.menuList.push_back(item);
            
        }
        /*if(BaseApp::loginLevel())
        {
            Data::Master::MenuStruct item;
            item.url = "/calendar";
            item.title = "Календарь";
            item.style = "menu-item-parent";
            item.icon = "fa fa-lg fa-fw fa-calendar";
            tmpl.menuList.push_back(item);
        }*/
        {
            Data::Master::MenuStruct item;
            item.url = "/news";
            item.title = "Новости";
            item.style = "menu-item-parent";
            item.icon = "fa fa-lg fa-fw fa-eye";
            tmpl.menuList.push_back(item);
        }
        if(BaseApp::loginLevel())
        {
            Data::Master::MenuStruct item;
            item.url = "#";
            item.title = "Сервис";
            item.style = "menu-item-parent";
            item.icon = "fa fa-lg fa-fw fa-cog";
            {
                Data::Master::MenuStruct subitem;
                subitem.url = "/service/import";
                subitem.title = "Импорт";
                subitem.style = "menu-item-parent";
                subitem.icon = "fa fa-lg fa-fw fa-cloud-upload";
                item.menu.push_back(subitem);
            }/*
            {
                Data::Master::MenuStruct subitem;
                subitem.url = "/service/settings";
                subitem.title = "Настройки";
                subitem.style = "menu-item-parent";
                subitem.icon = "fa fa-lg fa-fw fa-wrench";
                item.menu.push_back(subitem);
            }*/
            tmpl.menuList.push_back(item);

        }
        if(BaseApp::loginLevel())
        {
            Data::Master::MenuStruct item;
            item.url = "/profile";
            item.title = "Мой профиль";
            item.style = "menu-item-parent";
            item.icon = "fa fa-lg fa-fw fa-user";
            tmpl.menuList.push_back(item);
        }
        
    }
    
    void SmartAgent::master(std::string path)
    {
        Data::Master tmpl;
        tmpl.page.title = path;
        tmpl.page.description = "description";
        tmpl.page.keywords = "keywords";
        tmpl.assetsUrl = path;

        std::cout<<path<<"Current user "<<m_currentUser<<" "<<__LINE__<<std::endl;
        cppdb::transaction tr(m_session);
        cppdb::result result;
        
        if(request().request_method() == "POST" ) {
            std::cout<<"POST"<<std::endl;
            if (request().post("id") ==  "login")
            {
                std::cout<<request().post("email")<<std::endl;
                std::cout<<request().post("password")<<std::endl;
                std::cout<<request().post("remember")<<std::endl;
                std::cout<<request().post("id")<<std::endl;
                result = m_session << "select password_hash,password_method,password_salt,user_ref, (crypt(?,'$1$'||password_salt)=password_hash)::int passwd_valid from auth_identity  inner join auth_info on auth_identity.auth_info_id = auth_info.id where \"identity\" = ? "<<request().post("password")<<request().post("email")<<cppdb::row;;
                if(!result.empty())
                {
                    std::cout<<result.get<std::string>("password_method")<<std::endl;
                    
                    Auth::HashFunction *crypt;
                    if(result.get<std::string>("password_method") == "crypt") //md5
                    {
                        if(result.get<int>("passwd_valid"))
                        {
                            //std::cout<<request().post("user_ref")<<" "<<__LINE__<<std::endl;
                            BaseApp::loginLevel(1);
                            
                        }
                    }
                    else //bcrypt
                    {
                        crypt = new Auth::BCryptHashFunction();
                        if(crypt->verify(request().post("password"), result.get<std::string>("password_hash"), result.get<std::string>("password_salt")))
                        {
                            //std::cout<<result.get<std::string>("user_ref")<<" "<<__LINE__<<std::endl;
                            //std::cout<<request().post("email")<<" "<<__LINE__<<std::endl;
                            BaseApp::loginLevel(1);
                        }
                    }
                    if (loginLevel())
                    {
                        std::cout<<result.get<std::string>("user_ref")<<" "<<__LINE__<<std::endl;
                        m_currentUser = result.get<std::string>("user_ref");
                        //getUser()->Property("",m_currentUser);
                    }
                    
                    
                }
            }else if(request().post("id") ==  "searchdata"){
                searchData();
                return;
            }
        }
        
        //Model::ObjectPropery *user = new Model::ObjectPropery("user");
        
        if(BaseApp::loginLevel())
        {
            std::cout<<m_currentUser<<" "<<__LINE__<<std::endl;
            boost::any param = getUser()->Property("Наименование", m_currentUser);
            tmpl.page.userName = boost::any_cast<std::string>(param);
            
            std::string query;
            query = INLINE_SQLSCRIPT
            (
                select sum(inbox),sum(allmsg) from(
                    select count(1) inbox,0 allmsg from messages where state = 0 and author_ref = ?
                    union all
                    select 0 inbox,count(1) allmsg from messages where author_ref = ?
                    )a
             );
            //cppdb::transaction tr(m_session);
            cppdb::result result;
            int newMsgCount=0;
            result = m_session << query << m_currentUser<<m_currentUser<<cppdb::row;
            if (!result.empty()) {
                newMsgCount = result.get<int>(0);
            }
            param = getUser()->Property("Код 1С", m_currentUser);
            tmpl.user_code= boost::any_cast<std::string>(param);
            tmpl.user_ref= m_currentUser;
            
            
        }else{
            m_currentUser = "00000000-0000-0000-0000-000000000000";
            tmpl.user_code= "";
            tmpl.user_ref= m_currentUser;
            tmpl.page.userName = "";
        }
        std::cout<<__LINE__<<std::endl;
        tmpl.page.is_signed = BaseApp::loginLevel() != 0;
        accordion(tmpl);
        tr.commit();
        render("Master",tmpl);
    }
    
    
    void SmartAgent::post(){
        std::cout<<request().request_method()<<" "<<__LINE__<<std::endl;
        if(request().request_method() == "POST" ) {
            std::cout<<request().post("email")<<std::endl;
            std::cout<<request().post("password")<<std::endl;
            std::cout<<request().post("remember")<<std::endl;
        }
        master("/");
        
    }
    
    //-------------------------------------------------------------------------------------
    // Dsc: Рендеринг новостей
    //-------------------------------------------------------------------------------------
    void SmartAgent::dashboard(std::string path)
    {
        Data::Dashboard tmpl;
        
        std::cout<<request().request_method()<<" "<<__LINE__<<std::endl;
        
        
        //tmpl.page.title = path;
        //tmpl.page.description = "description";
        //tmpl.page.keywords = "keywords";
        //tmpl.page.menuList.insert(std::pair<std::string,std::string>("/","MASTER"));
        //tmpl.page.menuList.insert(std::pair<std::string,std::string>("/news","NEWS"));
        //tmpl.mainNews = "Сенсация! У нас на сайте ничего не произошло!";
        
        
        
        render("Dashboard",tmpl);
    }
    
    void SmartAgent::login(std::string path)
    {
        Data::Login tmpl;
        std::cout<<request().request_method()<<" "<<__LINE__<<std::endl;
        //int session_id=0;
       
        if(request().request_method() == "POST" ) {
            std::cout<<" "<<__LINE__<<std::endl;
            tmpl.loginForm.load(context());
            if(tmpl.loginForm.validate() /*&& correct_login(tmpl.loginForm)*/) { // perform form validation and then user validation
                tmpl.email = tmpl.loginForm.email.value();
                tmpl.password = tmpl.loginForm.password.value();
                tmpl.remember = tmpl.loginForm.remember.value();
                
                std::cout<<"eMail "<<tmpl.email<<" pass "<<tmpl.password<<std::endl;
                //session().reset_session();
                //session().erase("prelogin");
                //session().set("id",session_id); // or some other login id stored in the session
                //response().set_redirect_header("/some/path");
                //return;
            }
        }else{
            std::cout<<" "<<__LINE__<<std::endl;
            
            render("Login",tmpl);
        }
    }
    
    
    void SmartAgent::logout()
    {
        auto query = request().get();
        for (auto i = query.begin(); i != query.end();i++)
        {
            std::cout<<"params "<<i->first<<" "<<i->second<<std::endl;
        }
        BaseApp::loginLevel(0);
        m_currentUser = "";
        master("/");
        
    }
    
    void SmartAgent::inbox(std::string path)
    {
        
    }
    
    void SmartAgent::events()
    {
        if(request().request_method() == "POST" ) {
            
            cppdb::transaction tr(m_session);
            cppdb::result sqlResult;
            Utils::String query;
            
            std::cout<<request().post("event")<<" "<<__LINE__<<std::endl;
            std::cout<<request().post("object")<<" "<<__LINE__<<std::endl;
            std::cout<<request().post("value")<<" "<<__LINE__<<std::endl;
            std::string event = request().post("event");
            std::string object = request().post("object");
            std::string value = request().post("value");
            
            sqlResult = m_session << "select name,value from events where ref=?" <<event <<cppdb::row;
            if(!sqlResult.empty()){
                query = sqlResult.get<std::string>("query_string");
                sqlResult = m_session << query.toUTF8() <<m_currentUser<< "";
            }
            
        }
    }
    
#pragma mark -
    // mark 1
#pragma mark Fetch File List
    /*******************************************/
    /* Отчеты */
    /*******************************************/
    
    void SmartAgent::reportsData()
    {
        cppdb::transaction tr(m_session);
        cppdb::result sqlResult;
        Utils::String query;
        
        auto httpquery = request().get();
        //for (auto i = httpquery.begin(); i != httpquery.end();i++)
        //    std::cout<<"params "<<i->first<<" "<<i->second<<std::endl;
        
        struct tm begindate;
        struct tm enddate;
        memset(&begindate, 0, sizeof(struct tm));
        memset(&enddate, 0, sizeof(struct tm));
        strptime(httpquery.find("begindate")->second.c_str(), "%Od.%Om.%Y", &begindate);
        strptime(httpquery.find("enddate")->second.c_str(), "%d.%m.%Y", &enddate);
        
        std::cout << httpquery.find("begindate")->second.c_str()<<std::endl;
        std::cout<<begindate.tm_mday<<" "<<begindate.tm_mon+1<<" "<<begindate.tm_year+1900<<std::endl;
        
        sqlResult = m_session << "select query_string from reports_query where query_path = ?"<<httpquery.find("report")->second<<cppdb::row;
        
        
        if(!sqlResult.empty()){
            query = sqlResult.get<std::string>("query_string");
            
            // Установить параметры запроса
            std::string userRef = httpquery.find("user")->second;
            //std::cout<<userRef<<std::endl<<" "<<__LINE__;
            std::string code = boost::any_cast<std::string>(getUser()->Property("Код 1С",userRef));
            std::string region = boost::any_cast<std::string>(getUser()->Property("Регион",userRef));
            std::string name = boost::any_cast<std::string>(getUser()->Property("Наименование",userRef));
            
            query.arg(code).arg(begindate.tm_year+1900).arg(begindate.tm_mon+1).arg(begindate.tm_mday).arg(enddate.tm_year+1900).arg(enddate.tm_mon+1).arg(enddate.tm_mday);
            
            //std::cout<<query<<std::endl;
            // Установить регион клиента
            Utils::SoapClient wsClient(region);
            
            // Выпронить запрос к WS
            Utils::SoapClient::SoapResult soapRsult = wsClient.executeQuery(Utils::s2ws(query.narrow()));
            //std::cout<<soapRsult.size()<<std::endl;
            
            cppcms::json::value data_object;
            
            Data::ReportRows tableRows;
            int rowCount = 1;
            int subRowCount = 1;
            for(auto i = soapRsult.begin(); i != soapRsult.end(); i++)
            {
                std::cout<<Utils::ws2s((*i)[4])<<std::endl;
                Data::DataRowItem row;
                Data::DataRowItem subrow;
                int colCounter = 1;
                int itemState = atoi(Utils::ws2s((*i)[0]).c_str());
                for(auto k = i->begin();k!= i->end();k++)
                {
                    
                    Data::DataCellItem item;
                    item.value = Utils::ws2s(*k);
                    item.col_id = colCounter++;
                    
                    row.row.push_back(item);
                    row.row_id = rowCount;
                    
                    subrow.row.push_back(item);
                    subrow.row_id = subRowCount;
                    
                    
                }
                
                
                if (itemState==2) {
                    subRowCount++;
                    tableRows[tableRows.size()-1].subrow.push_back(subrow);
                }else
                {
                    subRowCount = 1;
                    row.row_id = rowCount++;
                    tableRows.push_back(row);
                    
                }
                
            }
            data_object["data"] = tableRows;
            data_object["recordsTotal"] = rowCount;
            data_object["recordsFiltered"] = rowCount;
            data_object["draw"] = 1;
            //data_object["length"] = 10;
            //std::cout<<data_object<<std::endl;
            response().out()<<data_object;
            
            
        }
        tr.commit();
    }
    
    void SmartAgent::reports(std::string path)
    { // MARK: -dsds
        
        
        if(m_currentUser.empty())
            response().out()<<"Вы не авторзированы !";
        Data::Reports tmpl;
        
        //tmpl.searchString = searchStr;
        cppdb::transaction tr(m_session);
        cppdb::result sqlResult;
        Utils::String query;
        sqlResult = m_session << "select name,params from reports_query where query_path = ?"<<path<<cppdb::row;
        cppcms::json::value object;
        
        std::fstream setting;
        setting.open("settings.json",std::fstream::in);
        object.load(setting,true);
        setting.close();
        
        auto reportSettings = object["reports"][path];
        for (auto i=reportSettings["ajax"].array().begin();i != reportSettings["ajax"].array().end();i++)
        {
            std::cout<<(*i).str()<<std::endl;
            tmpl.ajaxParamList.push_back((*i).str());
        }
        for (auto i=reportSettings["databind"].array().begin();i != reportSettings["databind"].array().end();i++)
        {
            std::cout<<(*i).str()<<std::endl;
            tmpl.dataBindList.push_back((*i).str());
        }
        for (auto i=reportSettings["headers"].array().begin();i != reportSettings["headers"].array().end();i++)
        {
            std::cout<<(*i).str()<<std::endl;
            tmpl.headerItemsList.push_back((*i).str());
        }
        
        for (auto i=reportSettings["render"].array().begin();i != reportSettings["render"].array().end();i++)
        {
            std::cout<<(*i).str()<<std::endl;
            tmpl.dataRenderList.push_back((*i).str());
        }
        
        tmpl.is_DateRange = reportSettings["hasdaterange"].boolean();
        
        
        if(!sqlResult.empty()){
            tmpl.user_level = BaseApp::loginLevel();
            tmpl.reportName = sqlResult.get<std::string>("name");
            
        }
        
        
        
        sqlResult = m_session << "select query_string,name from reports_query where query_path = ?"<<"user-list"<<cppdb::row;
        
        if(!sqlResult.empty()){
            query = sqlResult.get<std::string>("query_string");
        }
        
        sqlResult = m_session << query.toUTF8() << m_currentUser<<m_currentUser;
        while (sqlResult.next()) {
            Data::ParamItem item;
            sqlResult>>item.ref>>item.name;
            tmpl.userList.push_back(item);
        }
        
        
        render("Reports",tmpl);
        
        //render("Search",tmpl);
        //response().out()<<"Reports "<<path<<" "<<request().remote_addr()<<" | "<<request().request::http_accept_language	()<<" | " <<request().http_user_agent();
        //std::cout<<request().path_info()<<std::endl;
        setting.close();
        tr.commit();
    }
    
    /*******************************************/
    /* Документы */
    /*******************************************/
    
    void SmartAgent::docsDataTable()
    {
        cppdb::transaction tr(m_session);
        cppdb::result sqlResult;
        Utils::String wsquery;
        std::string sqlquery;
        cppcms::json::value data_object;
        Data::ReportRows tableRows;
        int rowCount = 1;
        
        auto httpquery = request().get();
        
        
        sqlResult = m_session << "select query_string from reports_query where query_path = ?" << httpquery.find("recordtype")->second << cppdb::row;
        
        if(!sqlResult.empty()){
            sqlquery = sqlResult.get<std::string>("query_string");
            
            std::string recordRef = httpquery.find("record")->second;
            if (!recordRef.empty()) {
                
                std::cout<<"recordRef ="<<recordRef<<std::endl;
                
                sqlResult = m_session << sqlquery << recordRef;
                while (sqlResult.next())
                {
                    Data::DataRowItem row;
                    int colCounter = 1;
                    //int itemState = 0;//atoi(Utils::ws2s((*i)[0]).c_str());
                    for(int k = 0;k < sqlResult.cols();k++)
                    {
                        //std::cout<<k<<std::endl;
                        Data::DataCellItem item;
                        if(sqlResult.is_null(k))
                            item.value = "";
                        else
                            item.value = sqlResult.get<std::string>(k);
                        item.col_id = colCounter++;
                        
                        row.row.push_back(item);
                        row.row_id = rowCount;
                        
                    }
                    row.row_id = rowCount++;
                    tableRows.push_back(row);
                    
                }
            }
        }
        data_object["data"] = tableRows;
        data_object["recordsTotal"] = rowCount;
        data_object["recordsFiltered"] = rowCount;
        data_object["draw"] = 1;
        //data_object["length"] = 10;
        //std::cout<<data_object<<std::endl;
        tr.commit();
        response().out()<<data_object;
        
    }

    
    void SmartAgent::docsData()
    {
        cppdb::transaction tr(m_session);
        cppdb::result sqlResult;
        Utils::String wsquery;
        std::string sqlquery;
        
        auto httpquery = request().get();
        //for (auto i = httpquery.begin(); i != httpquery.end();i++)
        //    std::cout<<"params "<<i->first<<" "<<i->second<<std::endl;
        
        struct tm begindate;
        struct tm enddate;
        memset(&begindate, 0, sizeof(struct tm));
        memset(&enddate, 0, sizeof(struct tm));
        strptime(httpquery.find("begindate")->second.c_str(), "%Od.%Om.%Y", &begindate);
        strptime(httpquery.find("enddate")->second.c_str(), "%d.%m.%Y", &enddate);
        
        std::cout << httpquery.find("begindate")->second.c_str()<<std::endl;
        std::cout << begindate.tm_mday<<" "<<begindate.tm_mon+1<<" "<<begindate.tm_year+1900<<std::endl;
        
        sqlResult = m_session << "select query_string from reports_query where query_path = ?"<<httpquery.find("report")->second<<cppdb::row;
        
        
        if(!sqlResult.empty()){
            sqlquery = sqlResult.get<std::string>("query_string");
            
            // Установить параметры запроса
            std::string userRef = httpquery.find("user")->second;
            //std::cout<<userRef<<std::endl<<" "<<__LINE__;
            std::string code = boost::any_cast<std::string>(getUser()->Property("Код 1С",userRef));
            std::string region = boost::any_cast<std::string>(getUser()->Property("Регион",userRef));
            std::string name = boost::any_cast<std::string>(getUser()->Property("Наименование",userRef));
            
            //query.arg(userRef).arg(httpquery.find("begindate")->second.c_str()).arg(httpquery.find("enddate")->second.c_str());
            
            // Установить регион клиента
            //Utils::SoapClient wsClient(region);
            
            // Выпронить запрос к WS
            //Utils::SoapClient::SoapResult soapRsult = wsClient.executeQuery(Utils::s2ws(query.narrow()));
            //std::cout<<soapRsult.size()<<std::endl;
            sqlResult = m_session << sqlquery << userRef << httpquery.find("begindate")->second << httpquery.find("enddate")->second;
            
            
            cppcms::json::value data_object;
            
            Data::ReportRows tableRows;
            int rowCount = 1;
            int subRowCount = 1;
            
            //if (!sqlResult.empty())
            while (sqlResult.next())
            {
                Data::DataRowItem row;
                Data::DataRowItem subrow;
                int colCounter = 1;
                int itemState = 0;//atoi(Utils::ws2s((*i)[0]).c_str());
                for(int k = 0;k < sqlResult.cols();k++)
                {
                    //std::cout<<k<<std::endl;
                    Data::DataCellItem item;
                    if(sqlResult.is_null(k))
                        item.value = "";
                    else
                        item.value = sqlResult.get<std::string>(k);
                    item.col_id = colCounter++;
                    
                    row.row.push_back(item);
                    row.row_id = rowCount;
                    
                    subrow.row.push_back(item);
                    subrow.row_id = subRowCount;
                    
                    
                }
                
                
                if (itemState==2) {
                    subRowCount++;
                    tableRows[tableRows.size()-1].subrow.push_back(subrow);
                }else
                {
                    subRowCount = 1;
                    row.row_id = rowCount++;
                    tableRows.push_back(row);
                    
                }
                
            }
            data_object["data"] = tableRows;
            data_object["recordsTotal"] = rowCount;
            data_object["recordsFiltered"] = rowCount;
            data_object["draw"] = 1;
            //data_object["length"] = 10;
            //std::cout<<data_object<<std::endl;
            response().out()<<data_object;
            
            
        }
        
        tr.commit();
    }
    
    void SmartAgent::docs(std::string path)
    { // MARK: -dsds
        
        if(m_currentUser.empty())
            response().out()<<"Вы не авторзированы !";
        Data::Journals tmpl;
        
        //tmpl.searchString = searchStr;
        cppdb::transaction tr(m_session);
        cppdb::result sqlResult;
        Utils::String query;
        sqlResult = m_session << "select name,params from reports_query where query_path = ?"<<path<<cppdb::row;
        cppcms::json::value object;
        
        std::fstream setting;
        setting.open("settings.json",std::fstream::in);
        object.load(setting,true);
        setting.close();
        
        std::cout<<__LINE__<<std::endl;
        auto reportSettings = object["journals"][path];
        for (auto i=reportSettings["ajax"].array().begin();i != reportSettings["ajax"].array().end();i++)
        {
            //std::cout<<(*i).str()<<std::endl;
            tmpl.ajaxParamList.push_back((*i).str());
        }

        for (auto i=reportSettings["databind"].array().begin();i != reportSettings["databind"].array().end();i++)
        {
            //std::cout<<(*i).str()<<std::endl;
            tmpl.dataBindList.push_back((*i).str());
        }

        for (auto i=reportSettings["headers"].array().begin();i != reportSettings["headers"].array().end();i++)
        {
            //std::cout<<(*i).str()<<std::endl;
            tmpl.headerItemsList.push_back((*i).str());
        }
        
        for (auto i=reportSettings["render"].array().begin();i != reportSettings["render"].array().end();i++)
        {
            //std::cout<<(*i).str()<<std::endl;
            tmpl.dataRenderList.push_back((*i).str());
        }
        
        tmpl.is_DateRange = reportSettings["hasdaterange"].boolean();
        //boost::any param = getUser()->Property("Код 1С", m_currentUser);
        //tmpl.user_code= boost::any_cast<std::string>(param);
        //tmpl.user_ref= m_currentUser;
        
        
        if(!sqlResult.empty()){
            tmpl.user_level = BaseApp::loginLevel();
            tmpl.journalName = sqlResult.get<std::string>("name");
            
        }

        
        query = "select p.address_ref ref ,p.simple_value  ||' ('||p2.simple_value||')' from address a inner join address_property p on a.ref = p.address_ref inner join address_property p2 on a.ref = p2.address_ref where user_ref = ? and p2.field_ref = '08b535eb-d5be-4460-37ba-62c1a5b7cb94' and p.field_ref = '3e0af5dc-9d4c-2ba3-e5a8-6217aa880eb0' ";
        
        sqlResult = m_session << query.toUTF8() << m_currentUser;
        while (sqlResult.next()) {
            
            Data::ParamItem item;
            sqlResult>>item.ref>>item.name;
            tmpl.addressList.push_back(item);
        }

        sqlResult = m_session << "select query_string,name from reports_query where query_path = ?"<<"user-list"<<cppdb::row;
        
        if(!sqlResult.empty()){
            query = sqlResult.get<std::string>("query_string");
        }
        
        sqlResult = m_session << query.toUTF8() << m_currentUser<<m_currentUser;
        while (sqlResult.next()) {
            Data::ParamItem item;
            sqlResult>>item.ref>>item.name;
            tmpl.userList.push_back(item);
        }
        
        
        render("Journals",tmpl);
        
        
        setting.close();
        tr.commit();
        /*response().out()<<"Reports "<<path<<" "<<request().remote_addr()<<" | "<<request().request::http_accept_language	()<<" | " <<request().http_user_agent();
         std::cout<<request().path_info()<<std::endl;*/
    }
   
   
    
   
    
    
    void SmartAgent::invoiceList(std::string path)
    {
        response().out()<<"InvoiceList";
    }
    void SmartAgent::invoce(std::string path)
    {
        response().out()<<"Invoice";
    }
    void SmartAgent::profile(std::string path)
    {
        Data::Profile tmpl;
        render("Profile",tmpl);
    }
    
    void SmartAgent::news(std::string path)
    {
        Data::News tmpl;
        render("News",tmpl);
        
    }
    /*******************************************/
    /* Товарный каталог */
    /*******************************************/
    void SmartAgent::catalog(std::string brand)
    {
        std::cout<<" ---> "<< brand<< " "<<std::endl;
        auto query = request().get();
        for (auto i = query.begin(); i != query.end();i++)
        {
            std::cout<<"params "<<i->first<<" "<<i->second<<std::endl;
        }
        
        
        
        Data::Catalog tmpl;
        cppdb::transaction tr(m_session);
        cppdb::result result;
        
        tmpl.widgetCount = atoi(query.find("column")->second.c_str());
        tmpl.lineCount = atoi(query.find("length")->second.c_str());
        tmpl.catalogImagePath = query.find("brand")->second;
        tmpl.header = 0;
        tmpl.is_signed = BaseApp::loginLevel() != 0;
        
        std::string sqlQuery = INLINE_SQLSCRIPT
        (
            SELECT "ref", parent_ref, is_group, name, articul, not_visible::int,  short_description, description, file_name, code,upakovka,price ,order_id from (
            WITH RECURSIVE tree ( "ref",parent_ref) AS (
            SELECT t1.ref ,t1.parent_ref
            FROM goods_simple t1 WHERE t1.ref = (select CASE WHEN not is_group THEN "parent_ref" else goods."ref" END from goods_simple goods inner join catalogs on catalogs.id = owner where catalogs.name = ? and state = 0 order by order_id limit 1 offset ?) and state = 0
            union
            select t2.ref , t2.parent_ref
            FROM goods_simple t2 INNER JOIN tree ON  tree.parent_ref = T2.ref where t2.is_group )
            select goods.ref,goods.parent_ref, goods.is_group::int , goods.name,''::text articul ,goods.not_visible::int not_visible,''::text short_description, goods.description, goods.file_name, ''::text code,''::text upakovka,0::numeric(12,2) price,order_id
            from goods_simple goods inner JOIN tree on goods.ref = tree.ref where goods.parent_ref != '00000000-0000-0000-0000-000000000000' order by level desc limit 2) a
                                                 
            UNION ALL
                                                 
            SELECT ref, parent_ref, is_group::int is_group, name, articul, not_visible::int,   regexp_replace(short_description, '<br/>', ' '::text) short_description, description, file_name, code,upakovka,price ,order_id
            FROM (
                select  goods.ref, parent_ref, is_group::int, goods.name, articul, not_visible::int,   short_description, description, file_name, goods.code,upakovka,price::numeric(12,2) price,order_id
                from goods_simple goods
                left join price on price.goods_ref = goods.ref and price_type_ref = 'b29e8011-280b-bd8a-241b-a4e52a6bb95b'
                inner join catalogs on catalogs.id = owner
                where catalogs.name = ? and state = 0  order by order_id  limit ? offset ?) b
                where parent_ref != '00000000-0000-0000-0000-000000000000' order by order_id
        );
        
        result = m_session << sqlQuery << tmpl.catalogImagePath << query.find("start")->second << tmpl.catalogImagePath << query.find("length")->second << query.find("start")->second;
        
        
        
        switch (tmpl.widgetCount) {
            case 1:
                tmpl.colSize = 12;
                break;
            case 2:
                tmpl.colSize = 6;
                break;
            case 3:
                tmpl.colSize = 4;
                break;
            case 4:
                tmpl.colSize = 3;
                break;
            default:
                tmpl.colSize = 12;
                break;
        }
        
        Data::CatalogItem lastItem;
        Data::CatalogItem lastGroup;
        for (int i=0; i<tmpl.widgetCount;i++)
        {
            Data::CatalogPage items;
            int counter=1;
            
            if(lastItem.is_group)
            {
                items.push_back(lastItem);
                counter--;
            }
            
            while (result.next()) {
                
                
                Data::CatalogItem item;
                std::string description;
                result>>item.ref>>item.parent_ref>>item.is_group>>item.name>>item.articul>>item.is_hidden>>item.brief>>item.description>>item.file>>item.code>>item.pack>>item.price;
                
                if(!(items.size() || item.is_group))
                {
                    tmpl.paragraph = lastGroup.name;
                    items.push_back(lastGroup);
                    counter--;
                }
                if(item.is_group)
                {
                    lastGroup = item;
                }
                lastItem = item;
                items.push_back(item);
                if(++counter > tmpl.lineCount/tmpl.widgetCount and i < tmpl.widgetCount-1 )
                    break;
            }
            
            tmpl.catalog.push_back(items);
            std::cout<<"part "<<" "<< i <<" counter " << counter<<std::endl;
        }
        int maxRow=0;
        for (auto i=tmpl.catalog.begin();i!=tmpl.catalog.end();i++)
        {
            if(i->begin()!= i->end())
                if(i->end()->is_group)
                    i->pop_back();
            maxRow = std::max<int>(maxRow,(int)i->size());
        }
        
        for (auto items = tmpl.catalog.begin();items != tmpl.catalog.end();items++)
        {
            for (int i = (int)items->size();i < maxRow;i++)
            {
                Data::CatalogItem item;
                item.emptySlot = 1;
                items->push_back(item);
            }
            
        }
        tr.commit();
        render("Catalog",tmpl);
        
    }
    
    void SmartAgent::catalog(std::string brand,std::string tables,std::string strings,std::string page)
    {//TODO: Сделать карточки товаров для разных видов товара например для товара с признаком 1 таблица а для товара с признаком 2 только карточка
        // TODO: Добавить в таблицу каталога ячеку с количеством предыдущего заказа
#pragma mark - fdfdfdf
        std::cout<<brand<<" --- "<< tables<<" " << strings<< " "<<std::endl;
        
        auto query = request().get();
        for (auto i = query.begin(); i != query.end();i++)
        {
            std::cout<<"params "<<i->first<<" "<<i->second<<std::endl;
        }
        
        
        Data::Catalog tmpl;
        cppdb::transaction tr(m_session);
        cppdb::result result;
        //int ipage = atoi(page.c_str());
        //int itbl = atoi(tables.c_str());
        //int istr = atoi(strings.c_str());
        
        //tmpl.widgetCount = itbl;
        //tmpl.lineCount = istr;
        tmpl.catalogImagePath = brand;
        tmpl.header = 1;
        
        
        result = m_session << "select count(1) catRows from goods_simple inner join catalogs on catalogs.id = owner where parent_ref != '00000000-0000-0000-0000-000000000000' and state = 0 and catalogs.name = ?"<<brand<<cppdb::row;
        
        
        if(!result.empty()){
            tmpl.itemCount = result.get<int>("catRows");
            std::cout<<brand<<" --- "<< tmpl.itemCount<<std::endl;
        }
        
        render("Catalog",tmpl);
        
    }
    
    void SmartAgent::catalogData(std::string params)
    {
        auto query = request().get();
        cppcms::json::value data_object;
        //std::cout<<"length "<<query.find("length")->second<<std::endl;
        std::string uidlist;
        for (auto i = query.begin(); i != query.end();i++)
        {
            std::cout<<"params "<<i->first<<" "<<i->second<<std::endl;
            uidlist += "'"+i->first+"',";
        }
        std::cout<<"|"<<uidlist<<"|"<<std::endl;
        
        if(uidlist.empty())
        {
            Data::CatalogItem item;
            data_object["data"][0] = item;
            data_object["recordsTotal"] = 1;
            data_object["recordsFiltered"] = 1;
            data_object["draw"] = 1;
            response().out()<<data_object;
            return;
        }
        if(uidlist.begin() != uidlist.end())
        {
            uidlist.erase(uidlist.end()-1);
        }
        cppdb::transaction tr(m_session);
        cppdb::result result;
        
        //query.find("length")->second
        //query.find("start")->second
        
        result = m_session << "select  goods.ref, parent_ref, goods.name, articul,  short_description, description, catalogs.name||'/'||file_name file_name, goods.code,upakovka,price::numeric(12,2) from goods_simple goods left join price on price.goods_ref = goods.ref and price_type_ref = 'b29e8011-280b-bd8a-241b-a4e52a6bb95b' left join catalogs on catalogs.id = goods.owner where goods.ref in ("+uidlist+") order by order_id";
        
        int i=0;
        while (result.next()) {
            
            Data::CatalogItem item;
            result>>item.ref>>item.parent_ref>>item.name>>item.articul>>item.brief>>item.description>>item.file>>item.code>>item.pack>>item.price;
            data_object["data"][i++] = item;
        }
        data_object["recordsTotal"] = i;
        data_object["recordsFiltered"] = i;
        data_object["draw"] = 1;
        //data_object["length"] = 10;
        //std::cout<<data_object<<std::endl;
        tr.commit();
        response().out()<<data_object;
    }
    
    /*******************************************/
    /* Процедуры поиска по каталогу */
    /*******************************************/
    
    void SmartAgent::searchData()
    {
        auto query = request().get();
        cppcms::json::value data_object;
        //std::string searchStr = request().post("search");
        std::string searchStr = query.find("search")->second;
        std::cout<<"params "<<searchStr<<std::endl;
        
        if(searchStr.empty())
        {
            Data::CatalogItem item;
            data_object["data"][0] = item;
            data_object["recordsTotal"] = 0;
            data_object["recordsFiltered"] = 0;
            data_object["draw"] = 0;
            response().out()<<data_object;
        }
        
        cppdb::transaction tr(m_session);
        cppdb::result result;
        
        //query.find("length")->second
        //query.find("start")->second
        
        result = m_session << "SELECT ref,name,articul,brief,description,file,code,pack,price from search_goods(?, ?, ?) as ( ref uuid,code text ,is_group bool ,name text  ,articul text  ,not_visible bool ,description text  ,brief text  ,file text  ,pack text  ,level int  ,owner int  , price numeric ,quantity int  ,order_id int);"<<searchStr<<"00000000-0000-0000-0000-000000000000"<<300;
        
        int i=0;
        while (result.next()) {
            
            Data::SearchItem item;
            result>>item.ref>>item.name>>item.articul>>item.brief>>item.description>>item.file>>item.code>>item.pack>>item.price;
            data_object["data"][i++] = item;
        }
        data_object["recordsTotal"] = i;
        data_object["recordsFiltered"] = i;
        data_object["draw"] = 1;
        //data_object["length"] = 10;
        //std::cout<<data_object<<std::endl;
        tr.commit();
        response().out()<<data_object;
        
    }
    
    void SmartAgent::search(std::string param,std::string str)
    {
        auto query = request().get();
        std::string searchStr;
        //std::cout<<"length "<<query.find("length")->second<<std::endl;
        for (auto i = query.begin(); i != query.end();i++)
        {
            std::cout<<"params "<<i->first<<" "<<i->second<<std::endl;
            searchStr = i->second;
        }
        
        Data::Search tmpl;
        
        tmpl.searchString = searchStr;
        tmpl.user_level = BaseApp::loginLevel();
        
        render("Search",tmpl);
    }
    
    /*******************************************/
    /* Корзина */
    /*******************************************/
    
    void SmartAgent::shopcart()
    {
        std::cout<<"shopcart "<<m_currentUser<<" "<<__LINE__<<std::endl;
        Data::ShopCart tmpl;
        
        std::string query = "select p.address_ref ref ,p.simple_value  ||' ('||p2.simple_value||')' from address a inner join address_property p on a.ref = p.address_ref inner join address_property p2 on a.ref = p2.address_ref where user_ref = ? and p2.field_ref = '08b535eb-d5be-4460-37ba-62c1a5b7cb94' and p.field_ref = '3e0af5dc-9d4c-2ba3-e5a8-6217aa880eb0' ";

        cppdb::result result;
        result = m_session << query << m_currentUser;
        while (result.next()) {
            
            Data::ParamItem item;
            result>>item.ref>>item.name;
            tmpl.addressList.push_back(item);
        }
        
        
        //cppdb::transaction tr(m_session);
        //cppdb::result result;
        //result = m_session << "";
        render("ShopCart",tmpl);
    }
    
    void SmartAgent::sendOrder()
    {
        if(request().request_method() == "POST" ) {
            std::cout<<request().post("address")<<std::endl;
            std::cout<<request().post("user")<<std::endl;
            //std::cout<<"-"<<request().post("records")<<"-"<<std::endl;
            cppcms::json::object object;
            cppcms::json::value value;
            
            if(std::strcmp(request().post("records").c_str(),"[]"))
            {
                
                
                std::stringstream strarray;
                strarray.str(request().post("records"));
                value.load(strarray, true);
                object = value.object();
                //std::cout<<object<<std::endl;
                cppcms::json::object::iterator it;

                Data::ImportDoc doc;
                Model::DataEventModelBase dm;

                for (it = object.begin(); it!=object.end();it++) {
                    std::cout<<it->first.str()<<"="<<it->second.str()<<std::endl;
                    doc.itemList.push_back({"",it->first.str(),it->second.str()});
                }
                doc.addres = request().post("address");
                if (request().post("user").empty()) {
                    doc.user = m_currentUser;
                }else{
                    doc.user = request().post("user");
                }
                doc.author = m_currentUser;
                doc.comment = request().post("comment");
                
                Data::ErrorItemList errors = dm.addValuesByRef(doc);
                for (auto i=errors.begin(); i!=errors.end(); ++i) {
                    std::cout<<i->code<<"--"<<i->errors<<std::endl;
                }
                
                char buf[12];
                std::sprintf(buf,"%010d",dm.getEventID());
                response().out()<<"Создано новый документ заказа №"<<buf;
                
                
            }
        }
    
        //response().out()<<"sucsess";
    }
    /*******************************************/
    /* Календарь */
    /*******************************************/
    
    void SmartAgent::calendar()
    {
        std::cout<<"Calendar "<<__LINE__<<std::endl;
        Data::Calendar tmpl;
        render("Calendar",tmpl);
    }
    

    /*******************************************/
    /* Служебные процедуры настройки и импорта */
    /*******************************************/
    void SmartAgent::service()
    {
        std::cout<<"Calendar "<<__LINE__<<std::endl;
        Data::Service tmpl;
        render("Service",tmpl);
        
    }
    
    void SmartAgent::settings()
    {
        response().out()<<"<div></div>";
    }
    
    void SmartAgent::upload()
    {
        std::cout<<"Upload "<<__LINE__<<std::endl;
        std::cout<<request().request_method()<<" "<<__LINE__<<std::endl;
        if(request().request_method() == "POST" ) {
            std::vector<booster::shared_ptr<cppcms::http::file> > files = context().request().files();
            
            Data::ErrorItemList errors;
            Utils::DataImportBase di;
            Model::DataEventModelBase dm;
            for (int i = 0; i < files.size(); ++i)
            {
                //const std::fstream file << &files[i]->data();
                std::cout<< files[i]->filename()<<std::endl;
                //setting.open("settings.json",std::fstream::in);
                //files[i]->filename()
                
                //boost::uuids::uuid uuid = boost::uuids::random_generator()();
                //files[i]->save_to("media/upload/" + boost::lexical_cast<std::string>(uuid));
                
                di.loadFromText(files[i]->data());
                
                errors = dm.addValuesByCode(di.getItemList());
                
                
                for (auto i=errors.begin(); i!=errors.end(); ++i) {
                    std::cout<<i->code<<"--"<<i->errors<<std::endl;
                    
                }
                
            }
            char buf[12];
            std::sprintf(buf,"%010d",dm.getEventID());
            response().out()<<"Импорт заказа №"<<buf<<" прошол успешно. Загруженно "<<di.getItemList().size()<< " строк. Ошибок "<<errors.size();
            
        }
        
        
    }
    
    void SmartAgent::textImport()
    {
        std::cout<<"Import text "<<__LINE__<<std::endl;
        std::cout<<request().request_method()<<" "<<__LINE__<<std::endl;
        if(request().request_method() == "POST" ) {
            //std::cout<<request().post("text")<<std::endl;
            Utils::DataImportBase di;
            di.loadFromText(request().post("text"));
            Model::DataEventModelBase dm;
            Data::ErrorItemList errors = dm.addValuesByCode(di.getItemList());
            
            for (auto i=errors.begin(); i!=errors.end(); ++i) {
                std::cout<<i->code<<"--"<<i->errors<<std::endl;
            }
            char buf[12];
            std::sprintf(buf,"%010d",dm.getEventID());
            
            response().out()<<"Импорт заказа №"<<buf<<" прошол успешно. Загруженно "<<di.getItemList().size()<< " строк. Ошибок "<<errors.size();
        }
    }
    
    /********************************/
    /* Обработка почтовых сообщений */
    /********************************/
    
    void SmartAgent::messagesData()
    {
        if(request().request_method() == "POST" ) {
            std::cout<<"messagesData"<<std::endl;
            std::cout<<"-"<<request().post("msglist")<<"-"<<std::endl;
            cppcms::json::value object;
            if(!std::strcmp(request().post("msglist").c_str(),"[]"))
                response().out()<<"empty";
            std::cout<<request().post("msgcount")<<" "<< request().post("mode")<<std::endl;

            std::stringstream strarray;
            strarray.str(request().post("msglist"));
            //a.str(strarray().post("msglist"));
            object.load(strarray, true);
            if (request().post("mode") ==  "checked")
            {
                int maxrows = boost::lexical_cast<int>(request().post("msgcount"));
                std::string msglist = "";
                for (int i = 0 ;i<maxrows;i++)
                {
                    msglist += "'"+object[i].str()+"',";
                }
                msglist.erase(msglist.end()-1);
                std::string query = "update messages set \"state\" = 1 where ref in ("+msglist+")";
                m_session << query <<cppdb::exec;
                response().out()<<"success";
            }else if(request().post("mode") ==  "remove"){
                int maxrows = boost::lexical_cast<int>(request().post("msgcount"));
                std::string msglist = "";
                for (int i = 0 ;i<maxrows;i++)
                {
                    msglist += "'"+object[i].str()+"',";
                }
                msglist.erase(msglist.end()-1);
                std::string query = "update messages set \"type\" = 9 where ref in ("+msglist+")";
                m_session << query <<cppdb::exec;
                response().out()<<"success";
            }else if(request().post("mode") ==  "impotent"){
                int maxrows = boost::lexical_cast<int>(request().post("msgcount"));
                std::string msglist = "";
                for (int i = 0 ;i<maxrows;i++)
                {
                    msglist += "'"+object[i].str()+"',";
                }
                msglist.erase(msglist.end()-1);
                std::string query = "update messages set \"state\" =  where ref in ("+msglist+")";
                m_session << query <<cppdb::exec;
                response().out()<<"success";
            
            }
        }
    }
    
    void SmartAgent::messages()
    {
        std::cout<<"Messages "<<__LINE__<<std::endl;
        if (m_currentUser.empty()) {
            response().out()<< "сессия устарела !";
            return;
        }
        Data::Messages tmpl;
        std::string query;
        query = INLINE_SQLSCRIPT
        (
          select sum(inbox),sum(allmsg) from(
              select count(1) inbox,0 allmsg from messages where state = 0 and author_ref = ?
              union all
              select 0 inbox,count(1) allmsg from messages where author_ref = ?
          )a
        );
        cppdb::transaction tr(m_session);
        cppdb::result result;
        result = m_session << query << m_currentUser<<m_currentUser<<cppdb::row;
        if (!result.empty()) {
            tmpl.newMsgCount = result.get<int>(0);
            tmpl.allMsgCount = result.get<int>(1);
        }
        tr.commit();
        render("Messages",tmpl);
    }
    
    void SmartAgent::messageList()
    {
        std::cout<<"Messages List "<<__LINE__<<std::endl;
        
        if (m_currentUser.empty()) {
            response().out()<< "сессия устарела !";
            return;
        }
        
        Data::Messages::MessageList tmpl;
        Data::Messages::MessageItemList itemList;
        
        auto httpquery = request().get();
        std::string searchStr;
        //std::cout<<"length "<<query.find("length")->second<<std::endl;
        for (auto i = httpquery.begin(); i != httpquery.end();i++)
        {
            std::cout<<"params "<<i->first<<" "<<i->second<<std::endl;
            //searchStr = i->second;
        }
        
        std::string items = httpquery.find("items")->second;
        std::string page = httpquery.find("page")->second;
        std::string folder = httpquery.find("folder")->second;
        
        std::string query;
        
        query = INLINE_SQLSCRIPT
        (
         
         select m.ref,m.id,uid,to_char(date, 'DD.MM.YYYY HH24:MI:SS'),file_name,state,
         CASE WHEN file_name is null  THEN 0 ELSE 1 END hasFile,
         CASE WHEN subject is null THEN substring(message,1,50)||'...' END subject,
         CASE WHEN (EXTRACT(epoch FROM now()::timestamp - "date"::timestamp)) <  60 THEN
         'Только что'
         WHEN  (EXTRACT(epoch FROM now()::timestamp - "date"::timestamp)/60) < 60 THEN
         (ROUND((EXTRACT(epoch FROM now()::timestamp - "date"::timestamp)/60))  || ' мин. назад')
         WHEN (EXTRACT(epoch FROM now()::timestamp - "date"::timestamp)/3600) < 2 THEN
         (ROUND((EXTRACT(epoch FROM now()::timestamp - "date"::timestamp)/3600))  || ' час. назад')
         WHEN  (DATE_PART('day', now()::timestamp - "date"::timestamp)) < 1 THEN
         to_char("date", 'Сегодня в HH24:MI')
         WHEN  (DATE_PART('day', now()::timestamp - "date"::timestamp)) < 2 THEN
         to_char("date", 'Вчера  в HH24:MI')
         ELSE
         to_char("date", 'DD MM YYYY | HH24:MI')
         END AS dateString
         from messages m inner join messages_type t on t.id = m.type where t.name = ? and author_ref = ? order by date desc limit ? offset ?
        );
        cppdb::transaction tr(m_session);
        cppdb::result result;
        result = m_session << query << folder << m_currentUser<<items<<(atoi(page.c_str())-1)*atoi(items.c_str());
        while (result.next()) {
            //std::cout<<m_currentUser<<std::endl;
            Data::Messages::MessageItem item;
            item.isAttachment = 0;
            item.isDraft = 0;
            item.isUnread = 1;
            item.isWarning = 0;
            item.senderName = "Оператор";
            result>>item.messageRef>>item.id>>item.uid>>item.messageDate>>item.attachmentList>>item.isUnread>>item.isAttachment>>item.subject;
            tmpl.messageItemList.push_back(item);
        }
        tr.commit();
        render("MessageList",tmpl);
    }

    void SmartAgent::messageCompose()
    {
        Data::Messages::Compose tmpl;
        render("Compose",tmpl);
        
    }
    
    void SmartAgent::messageRead()
    {
        Data::Messages::Read tmpl;
        auto httpget = request().get();
        
        std::string query = INLINE_SQLSCRIPT
        (
            select ref,id,uid,to_char(date, 'DD.MM.YYYY HH24:MI:SS'),file_name,state,
            CASE WHEN file_name is null  THEN 0 ELSE 1 END hasFile,
            CASE WHEN subject is null THEN substring(message,1,50)||'...' END subject,message
            from messages where ref = ?
        );
        
        cppdb::transaction tr(m_session);

        cppdb::result result;
        result = m_session << query << httpget.find("message")->second;
        while (result.next()) {
            
            Data::Messages::MessageItem item;
            item.isAttachment = 0;
            item.isDraft = 0;
            item.isUnread = 1;
            item.isWarning = 0;
            item.senderName = "Оператор";
            result>>item.messageRef>>item.id>>item.uid>>item.messageDate>>item.attachmentList>>item.isUnread>>item.isAttachment>>item.subject>>item.message;
            tmpl.currentMessage = item;
        }

        query = INLINE_SQLSCRIPT
        (
         select ref,original_name,system_name,size
         from message_attachment where ref = ?
        );
        
        
        result = m_session << query << httpget.find("message")->second;
        while (result.next()) {
            
            Data::Messages::AttachmentItem item;
            item.isImage = 0;
            item.size = 0;
            result>>item.ref>>item.originalName>>item.file>>item.size;
            tmpl.currentMessage.attachments.push_back(item);
        }
        std::string code = boost::any_cast<std::string>(getUser()->Property("Код 1С",m_currentUser));
        std::string region = boost::any_cast<std::string>(getUser()->Property("Регион",m_currentUser));
        
        std::ifstream file( "media/msg/"+region+"/"+code+"/"+tmpl.currentMessage.uid, std::ios::binary | std::ios::ate);
        std::ifstream::pos_type size = file.tellg();
        std::cout<<region<<"/"<<code<<" "<<size<<std::endl;
        if(!tmpl.currentMessage.attachments.size())
        {
            Data::Messages::AttachmentItem item;
            item.isImage = 0;
            item.size = (int)size;
            item.ref = httpget.find("message")->second;
            item.originalName = tmpl.currentMessage.attachmentList;
            //"media/msg/"+region+"/"+code+"/"+
            item.file = tmpl.currentMessage.uid;
            tmpl.currentMessage.attachments.push_back(item);
        }
        file.close();
        m_session << "update messages set \"state\" = 1 where ref = ?" << httpget.find("message")->second<<cppdb::exec;
        tr.commit();
        render("Read",tmpl);
        
    }
    
    void SmartAgent::attachmentDownload()
    {
        
        auto httpquery = request().get();
        
        //response().set_header("Set-Cookie", "fileDownload=true; path=/");
        //response().set_header("Cache-Control", "max-age=60, must-revalidate");
        //response().set_header("Content-type", "text/csv");
        //response().set_header("Content-Disposition", "filename=\"'.$title.'-' . $timestamp . '.csv\"'");
        
        std::string fileuid = httpquery.find("uid")->second;

        std::string code = boost::any_cast<std::string>(getUser()->Property("Код 1С",m_currentUser));
        std::string region = boost::any_cast<std::string>(getUser()->Property("Регион",m_currentUser));
        
        std::string filepath = "media/msg/"+region+"/"+code+"/"+fileuid;
        std::ifstream file;
        file.open(filepath , std::ios::binary | std::ios::ate);
        if (!file) {
            response().status(404);
            return;
        }

        std::filebuf* pbuf = file.rdbuf();
        std::size_t size = pbuf->pubseekoff (0,file.end,file.in);
        pbuf->pubseekpos (0,file.in);
        //std::ifstream::pos_type size = file.tellg();
        std::cout<<filepath<<" "<<size<<std::endl;
        
        //cppcms::impl::file_server fsrv;
        //mime_file.mime_type_by_name(filepath)Content-Type: application/octet-stream
        //response().content_type("application/vnd.ms-excel");
        //response().content_type("text/csv");
        ///< \note this is required otherwise data may be gzipped and content_length will be incorrect
        //response().io_mode(cppcms::http::response::nogzip);
        std::string filename = "Счет.xls";
        
        response().content_type("application/octet-stream");
        response().set_header("Pragma", "public");
        response().set_header("Content-Disposition", "attachment; filename=\"" + filename + "\"");
        response().set_header("X-Lighttpd-Send-File",filename);
        response().set_header("Content-Transfer-Encoding", "binary");
        response().set_header("Connection", "Keep-Alive");
        //response().content_length(size);
        //std::cout<<pbuf<<std::endl;
        
        //std::ostringstream os;
        //is = stream;
        //copy( std::istreambuf_iterator<char>(file),
        //     std::istreambuf_iterator<char>(),
        //     std::ostreambuf_iterator<char>(os));
        
        
        response().out() << pbuf;
        file.close();
 
    }
    
    void SmartAgent::messageReply()
    {
        Data::Messages::Reply tmpl;
        
        
        
        render("Reply",tmpl);
    
    }
}