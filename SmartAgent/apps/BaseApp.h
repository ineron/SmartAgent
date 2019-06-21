//
//  BaseApp.h
//  SmartAgent
//
//  Created by Евгений Илюшин on 22.12.10.
//  Copyright (c) 2010 Евгений Илюшин. All rights reserved.
//

#ifndef __BaseApp__
#define __BaseApp__

#define INLINE_JAVASCRIPT(...) #__VA_ARGS__
#define INLINE_SQLSCRIPT(...) #__VA_ARGS__
#define INLINE_WQUERY(...)  L ## #__VA_ARGS__

#include "model/ObjectPropery.h"
#include <cppcms/application.h>
#include <cppdb/frontend.h>

namespace Model {
    class ObjectPropery;
}

namespace Apps {
    class BaseApp : public cppcms::application
    {
    public:
        
        BaseApp(cppcms::service &s);
        virtual ~BaseApp();
        static BaseApp* instance();
        
        inline cppdb::session &dbsession()
            {return m_session;};
        inline const short loginLevel() const
            {return m_loginLevel;};
        void loginLevel(const short &l)
            {m_loginLevel = static_cast<short>(l);};
        virtual void setUser(Model::ObjectPropery *user){m_user = user;};
        virtual Model::ObjectPropery *getUser(){return m_user;};
        virtual std::string currentUserRef() {return m_currentUser;};
        virtual void currentUserRef(std::string userRef) {m_currentUser=userRef;};
    protected:
        cppdb::session m_session;
        std::string m_currentUser;
        virtual void initApp()=0;
        
    private:
        BaseApp(cppcms::service &s,int param);
        static BaseApp * m_pInstance;
        static short m_loginLevel;
        mutable Model::ObjectPropery *m_user;
        
    };
    

}

#endif /* defined(__BaseApp__) */
