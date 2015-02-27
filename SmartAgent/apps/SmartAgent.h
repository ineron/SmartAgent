//
//  SmartAgent.h
//  SmartAgent
//
//  Created by Евгений Илюшин on 16.10.14.
//  Copyright (c) 2014 Евгений Илюшин. All rights reserved.
//

#ifndef __SmartAgent__SmartAgent__
#define __SmartAgent__SmartAgent__

#include "BaseApp.h"
#include "../data/tmpl_master.h"



#pragma mark - dffdf

namespace Apps {
    class SmartAgent : public BaseApp{
        // TODO: midhan
        // FIXME: media
#pragma mark - Navigation        
    public:
        SmartAgent(cppcms::service &s);
        ~SmartAgent();
        virtual void main(std::string path);
        virtual void accordion(Data::Master &tmpl);
        virtual void master(std::string path);
        virtual void dashboard(std::string path);
        virtual void login(std::string path);
        virtual void logout();
        virtual void inbox(std::string path);
        virtual void catalog(std::string brand,std::string tables,std::string strings,std::string page);
        virtual void catalog(std::string brand);
        virtual void reports(std::string path);
        virtual void docs(std::string path);
        virtual void invoiceList(std::string path);
        virtual void invoce(std::string path);
        virtual void profile(std::string path);
        virtual void news(std::string path);
        virtual void search(std::string param,std::string str);
        virtual void shopcart();
        virtual void catalogData(std::string params);
        virtual void reportsData();
        virtual void searchData();
        virtual void docsData();
        virtual void docsDataTable();
        virtual void post();
        virtual void messages();
        virtual void calendar();
        virtual void messageList();
        virtual void service();
        virtual void settings();
        virtual void upload();
        virtual void textImport();
        virtual void messageCompose();
        virtual void messageRead();
        virtual void messageReply();
        virtual void messagesData();
        virtual void attachmentDownload();
        virtual void sendOrder();
        virtual void events();

    protected:
        virtual void initApp();
    private:
        //bool is_logon;
        //std::string m_currentUser;
    };
}

#endif /* defined(__SmartAgent__SmartAgent__) */
