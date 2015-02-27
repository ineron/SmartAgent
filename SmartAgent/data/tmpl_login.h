#ifndef TMPL_LOGIN_H
#define TMPL_LOGIN_H
#include <cppcms/view.h>
#include <cppcms/form.h>

namespace Data {
    
    
    struct LoginForm : public cppcms::form {
        cppcms::widgets::text email;
        cppcms::widgets::hidden formId;
        cppcms::widgets::password password;
        cppcms::widgets::checkbox remember;
        cppcms::widgets::submit submit;
        //cppcms::widgets::button cancel;
        
        LoginForm()
        {
            formId.value("1234");
            email.message("email");
            email.value("email");
            password.message("Пароль");
            password.value("password");
            remember.message("Запомнить меня");
            remember.value("remember");

            submit.message(booster::locale::gettext("Войти"));
            submit.value("send");
            //cancel.message(booster::locale::gettext("Отмена"));
            //cancel.value("cancel");
            //*this & email & password & remember & submit;
        
        }
        virtual bool validate()
        {
            if(!form::validate())
                return false;
            return true;
        }
    };
    
    /*struct LoginForm{
     void load(cppcms::http::context &cont){};
     bool validate(){ return true; };
     };*/
    //-------------------------------------------------------------------------------------
    // Dsc: Форма авторизации
    //-------------------------------------------------------------------------------------
    struct Login :public cppcms::base_content{
        LoginForm loginForm;
        //-------------------------------------------------------------------------------------
        // Dsc:
        //-------------------------------------------------------------------------------------
        bool is_signin;
        std::string email,password;
        bool remember;
        //-------------------------------------------------------------------------------------
        // Dsc: Конструктор страницы
        //-------------------------------------------------------------------------------------
        Login():is_signin(false) ,loginForm()
        
        {}
        //-------------------------------------------------------------------------------------
        // Dsc: Ленивый деструктор
        //-------------------------------------------------------------------------------------
        ~Login(){}
    };
    
}


#endif // TMPL_LOGIN_H
