//
//  WSClient.h
//  SmartAgent
//
//  Created by Евгений Илюшин on 20.01.15.
//  Copyright (c) 2015 Евгений Илюшин. All rights reserved.
//

#ifndef __SmartAgent__WSClient__
#define __SmartAgent__WSClient__

// wsdl2h -o wsclient.h InfoBase.wsdl
// soapcpp2 -i  -a -dgSoap -I/usr/local/share/gsoap/import  wsclient.h

#include <stdio.h>
#include <string>
#include <map>
#include <vector>
class InfoBaseSoapBindingProxy;

namespace Utils {
    class SoapClient
    {
    public:
        SoapClient(const std::string &endPoint);
        virtual ~SoapClient();
/*
        std::vector<WString> getUserDept(const std::string userCode) const;
        std::vector<std::vector<WString> >getUnbackedDocs(std::string userCode) const;
        std::vector<std::vector<WString> >getLastOrder(std::string userCode) const;
        std::vector<std::vector<WString> >getLastShipment(std::string userCode) const;
        std::vector<std::vector<WString> >getDeptStucture(std::string userCode) const;
        std::vector<std::vector<WString> >getOrdersShipping(const WDate &beginDate,const WDate &endDate,const std::string userCode) const;
        std::vector<std::vector<WString> >getGoodsSales(const WDate &beginDate,const WDate &endDate,const std::string userCode) const;
        std::vector<std::vector<WString> >getMutualItog(const WDate &beginDate,const WDate &endDate,const std::string userCode) const;
        std::vector<std::vector<WString> >getMutual(const WDate &beginDate,const WDate &endDate,const std::string userCode) const;
        std::vector<std::vector<WString> >getDocTablel(const std::string docRef) const;
*/
        typedef std::vector<std::vector<std::wstring> > SoapResult;
        SoapResult executeQuery(const std::wstring &query);
        //SoapResult executeQuery(std::string query);
        
        typedef std::map<std::string, std::string> ConnectionMap;
        static ConnectionMap m_soapConnectionMap;
    protected:
        InfoBaseSoapBindingProxy *m_client;
        std::string soapUserName;
        std::string soapUserPasswd;
        std::string endpoint;
    private:
    };
}

#endif /* defined(__SmartAgent__WSClient__) */
