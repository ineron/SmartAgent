//
//  BaseApp.cpp
//  SmartAgent
//
//  Created by Евгений Илюшин on 22.12.10.
//  Copyright (c) 2010 Евгений Илюшин. All rights reserved.
//

#include "BaseApp.h"

#include <cppcms/json.h>


namespace Apps {
    
    BaseApp* BaseApp::m_pInstance = NULL;
    short BaseApp::m_loginLevel = 0;
   
    
    BaseApp::BaseApp(cppcms::service &s,int param) : cppcms::application(s),m_user(0)
    {
        std::cout<<__LINE__<<std::endl;
        m_pInstance = this;
        m_session.open(settings().get<std::string>("app.connection_string"));
        std::cout<<m_session.is_open()<<std::endl;
    }
    
    BaseApp::BaseApp(cppcms::service &s): BaseApp(s,0)
    {
        std::cout<<__LINE__<<std::endl;
        
    }
    
    BaseApp::~BaseApp()
    {
        m_session.close();
    }
    BaseApp* BaseApp::instance()
    {
        if(m_pInstance)
           return m_pInstance;
        return NULL;
    }
    
}
