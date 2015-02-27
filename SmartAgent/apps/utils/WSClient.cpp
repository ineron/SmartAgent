//
//  WSClient.cpp
//  SmartAgent
//
//  Created by Евгений Илюшин on 20.01.15.
//  Copyright (c) 2015 Евгений Илюшин. All rights reserved.
//

#include "WSClient.h"
#include "BaseApp.h"
#include "gSoap/soapInfoBaseSoapBindingProxy.h"
#include "gSoap/InfoBaseSoapBinding.nsmap"

#include <cppcms/json.h>

namespace Utils {
    SoapClient::ConnectionMap SoapClient::m_soapConnectionMap = {
        {"spb","http://192.168.11.17/FIT-SPB/ws/InfoBase"},
        {"kzt","http://192.168.10.26/FIT-KZ/ws/InfoBase"},
        {"ukr","http://192.168.10.26/FIT-UA/ws/InfoBase"},
        {"nsk","http://192.168.10.26/FIT-NSK/ws/InfoBase"},
        {"krd","http://192.168.10.26/FIT-KRD/ws/InfoBase"},
        {"kzn","http://192.168.10.26/FIT-KZN/ws/InfoBase"},
        {"ekt","http://192.168.10.26/FIT-EKB/ws/InfoBase"},
        {"spb_1","http://192.168.11.17/ONLINE-SPB/ws/InfoBase"},
        {"test","http://192.168.0.142:8080/FIT-SPB/ws/InfoBase"}
    };
    
    SoapClient::SoapClient(const std::string &endPoint)
    {
        m_client = new InfoBaseSoapBindingProxy();
        std::string value = m_soapConnectionMap[endPoint];

        soapUserName = Apps::BaseApp::instance()->settings().get<std::string>("app.soap.user");
        soapUserPasswd = Apps::BaseApp::instance()->settings().get<std::string>("app.soap.passwd");
        std::cout<<"Passwd "<<soapUserPasswd<<std::endl;
        std::cout<<"User "<<soapUserName<<std::endl;
        std::cout<<"endpoint "<<value<<std::endl;
        
        endpoint = value.c_str();
        m_client->soap_endpoint = value.c_str();//m_soapConnectionMap[endPoint].c_str();
        m_client->recv_timeout = 30;
        
        
    }
    
    SoapClient::SoapResult SoapClient::executeQuery(const std::wstring &query)
    {
        
        InfoBaseSoapBindingProxy client;
        
        SoapResult result;
        
        _ns1__QueryUniversalResponse respone;
        _ns1__QueryUniversal request;
        
        std::wstring wquery = query;
        //std::wcout<<wquery<<std::endl;
        request.SystemExpression = &wquery;
        std::wcout<<*request.SystemExpression<<std::endl;
        client.soap_endpoint = endpoint.c_str();
        client.userid="Администратор";//soapUserName.c_str();
        client.passwd=soapUserPasswd.c_str();
        client.QueryUniversal(&request,&respone);
        if(respone.return_)
            for (auto i = respone.return_->row.begin(); i != respone.return_->row.end(); ++i)
            {
                auto values = *i;
                //for (auto k = 0;i<(*i)->Value.size();k++)
                std::vector<std::wstring> columns;
                for (auto k = values->Value.begin(); k != values->Value.end(); ++k)
                {
                    //std::cout<<__LINE__<<std::endl;
                    if(*k)
                        columns.push_back(*k);
                    else
                        columns.push_back(L"");
                }
                
                result.push_back(columns);
            }
        std::cout<<result.size()<<std::endl;
        return result;
    }
    
    SoapClient::~SoapClient()
    {
        delete m_client;
    }
}