#ifndef _TMPL_CATALOG_H
#define _TMPL_CATALOG_H
#include <cppcms/view.h>
#include <cppcms/http_context.h>
#include <cppcms/json.h>





namespace Data {
    
    
    typedef struct _CATITEMS {
        std::string ref;
        std::string parent_ref;
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
        int is_active=0;
        int is_group=0;
        int is_hidden=0;
        int remains=0;
        int emptySlot=0;
        
    } CatalogItem;
    
    typedef std::vector<CatalogItem> CatalogPage;
    //-------------------------------------------------------------------------------------
    // Dsc: Форма авторизации
    //-------------------------------------------------------------------------------------
    struct Catalog :public cppcms::base_content{
        
        //-------------------------------------------------------------------------------------
        // Dsc:
        //-------------------------------------------------------------------------------------
        //std::vector<CatalogItem> catalog;
        std::vector<std::vector<CatalogItem> > catalog;
        //-------------------------------------------------------------------------------------
        // Dsc:
        //-------------------------------------------------------------------------------------
        bool is_signed=false;
        std::string paragraph="";
        std::string catalogImagePath="";
        int widgetCount=0;
        int lineCount=0;
        int colSize=0;
        int header=0;
        int itemCount=0;
        //-------------------------------------------------------------------------------------
        // Dsc: Конструктор страницы
        //-------------------------------------------------------------------------------------
        Catalog():is_signed(false),widgetCount(0),lineCount(0),colSize(0)
        
        {}
        //-------------------------------------------------------------------------------------
        // Dsc: Ленивый деструктор
        //-------------------------------------------------------------------------------------
        ~Catalog(){}
    };
    
}


namespace cppcms {
    namespace json {
        template<>
        struct traits<Data::CatalogItem> {
            static Data::CatalogItem get(value const &v)
            {
                Data::CatalogItem c;
                if(v.type()!=is_object)
                    throw bad_value_cast();
                
                c.ref           = v.get<std::string>("ref");
                c.parent_ref    = v.get<std::string>("parent_ref");
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
                c.is_active     = v.get<int>("is_active");
                c.is_group      = v.get<int>("is_group");
                c.is_hidden     = v.get<int>("is_hidden");
                c.remains       = v.get<int>("remains");
                return c;
            }
            
            static void set(value &v,Data::CatalogItem const &inv){
                v.set("ref",inv.ref);
                v.set("parent_ref",inv.parent_ref);
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
                v.set("is_active",inv.is_active);
                v.set("is_group",inv.is_group);
                v.set("is_hidden",inv.is_hidden);
                v.set("remains",inv.remains);
                
            }
        };
    } // json
} // cppcms


#endif // _TMPL_CATALOG_H
