#ifndef TMPL_NAV_H
#define TMPL_NAV_H
#include "tmpl_master.h"

namespace Data {
  //-------------------------------------------------------------------------------------
  // Dsc: Новостной контент
  //-------------------------------------------------------------------------------------
  struct Nav :public Master {
    //-------------------------------------------------------------------------------------
    // Dsc: Струтура меню
    //-------------------------------------------------------------------------------------
    typedef struct MENUSTRUCT{
	std::string url;
	std::string title;
	std::string style;
	std::string icon;
	std::vector<MENUSTRUCT>menu;
    }MenuItem;
    std::vector<MenuItem> menuList;   // список выводимых пунктов меню (url,desc)
    //-------------------------------------------------------------------------------------
    // Dsc: Конструктор страницы
    //-------------------------------------------------------------------------------------
    Nav() //:
      //Master()
    {}
    //-------------------------------------------------------------------------------------
    // Dsc: Ленивый деструктор
    //-------------------------------------------------------------------------------------
    ~Nav(){}
  };

}


#endif // TMPL_NAV_H
