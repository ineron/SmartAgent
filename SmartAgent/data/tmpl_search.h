#ifndef TMPL_SEARCH_H
#define TMPL_SEARCH_H
#include <cppcms/view.h>
#include <cppcms/http_context.h>
#include <cppcms/json.h>

namespace Data {
    
    
    typedef struct _SEARCHITEM {
        
        std::string ref;
        std::string articul;
        std::string code;
        std::string name;
        std::string description;
        std::string brief;
        std::string file;
        std::string pack;
        int level=0;
        int owner=0;
        double price=0;
        int quantity=0;
        int order_id=0;
    }SearchItem;
    
    //-------------------------------------------------------------------------------------
    // Dsc: Новостной контент
    //-------------------------------------------------------------------------------------
    struct Search :public cppcms::base_content{
        //-------------------------------------------------------------------------------------
        // Dsc: Главная новость
        //-------------------------------------------------------------------------------------
        int user_level=0;
        std::string user_ref;
        std::string author_ref;
        std::string order_id;
        std::string order_ref;
        std::string address_ref;
        std::string comments;
        std::string searchString;
        std::vector<SearchItem>searchItems;
        
        
        //-------------------------------------------------------------------------------------
        // Dsc: Конструктор страницы
        //-------------------------------------------------------------------------------------
        Search() //:
        //Master()
        {}
        //-------------------------------------------------------------------------------------
        // Dsc: Ленивый деструктор
        //-------------------------------------------------------------------------------------
        ~Search(){}
    };
    
}


namespace cppcms {
    namespace json {
        template<>
        struct traits<Data::SearchItem> {
            static Data::SearchItem get(value const &v)
            {
                Data::SearchItem c;
                if(v.type()!=is_object)
                    throw bad_value_cast();
                
                c.ref           = v.get<std::string>("ref");
                c.articul       = v.get<std::string>("articul");
                c.code          = v.get<std::string>("code");
                c.name          = v.get<std::string>("name");
                c.description   = v.get<std::string>("description");
                c.brief         = v.get<std::string>("brief");
                c.file          = v.get<std::string>("file");
                c.pack          = v.get<std::string>("pack");
                c.level         = v.get<int>("level");
                c.owner         = v.get<int>("owner");
                c.price         = v.get<double>("price");
                c.quantity      = v.get<int>("quantity");
                c.order_id      = v.get<int>("order_id");
                return c;
            }
            
            static void set(value &v,Data::SearchItem const &inv){
                v.set("ref",inv.ref);
                v.set("articul",inv.articul);
                v.set("code",inv.code);
                v.set("name",inv.name);
                v.set("description",inv.description);
                v.set("brief",inv.brief);
                v.set("file",inv.file);
                v.set("pack",inv.pack);
                v.set("level",inv.level);
                v.set("owner",inv.owner);
                v.set("price",inv.price);
                v.set("quantity",inv.quantity);
                v.set("order_id",inv.order_id);
                
            }
        };
    } // json
} // cppcms


#endif // TMPL_SEARCH_H
