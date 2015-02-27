#ifndef TMPL_DATASTRUCT_H
#define TMPL_DATASTRUCT_H
#include <cppcms/view.h>
#include <cppcms/http_context.h>
#include <cppcms/json.h>

namespace Data {
    
    
    typedef struct _DATACELL {
        int 		col_id;
        int 		col_type;
        std::string 	col_name;
        std::string 	value;
    }DataCellItem;
    
    typedef struct _DATAROW {
        int row_id;
        std::vector<DataCellItem> row;
        std::vector<_DATAROW> subrow;
    } DataRowItem;
    
    typedef struct _PARAMITEM{
	std::string name;
	std::string ref;
	std::string value;
    }ParamItem;


    typedef struct _IMPORTDOC
    {
	std::string author;
	std::string user;
	std::string addres;
	std::string comment;
	std::vector<ParamItem>itemList;
    }ImportDoc;

    typedef struct _JSONARRAY{
        std::vector<std::string> array;
    }JsonArray;
    
    typedef struct _IMPORTITEM{
	std::string code;
	std::string value;
    }ImportItem;
    
    typedef std::vector<ImportItem> ImportItemList;

    typedef struct _ERRORITEM{
	std::string code;
	int errors;
    }ErrorItem;
    
    typedef std::vector<ErrorItem> ErrorItemList;
    
}

namespace cppcms {
    namespace json {
        template<>
        struct traits<Data::DataCellItem> {
            static Data::DataCellItem get(value const &v)
            {
                Data::DataCellItem c;
                if(v.type()!=is_object)
                    throw bad_value_cast();
                
                c.col_id        = v.get<int>("col_id");
                c.col_type      = v.get<int>("col_type");
                c.col_name      = v.get<std::string>("col_name");
                c.value         = v.get<std::string>("value");
                return c;
            }
            
            static void set(value &v,Data::DataCellItem const &inv){
                v.set("col_id",inv.col_id);
                v.set("col_type",inv.col_type);
                v.set("col_name",inv.col_name);
                v.set("value",inv.value);
                
            }
        };


        template<>
        struct traits<Data::DataRowItem> {
            static Data::DataRowItem get(value const &v)
            {
                Data::DataRowItem c;
                if(v.type()!=is_object)
                    throw bad_value_cast();
                c.row_id     = v.get<int>("row_id");
                c.row        = v.get<std::vector<Data::DataCellItem> >("row");
                c.subrow     = v.get<std::vector<Data::DataRowItem> >("subrow");
                return c;
            }
            
            static void set(value &v,Data::DataRowItem const &inv){
                v.set("row_id",inv.row_id);
                v.set("row",inv.row);
                v.set("subrow",inv.subrow);
                
            }
        };

        template<>
        struct traits<Data::JsonArray> {
            static Data::JsonArray get(value const &v)
            {
                Data::JsonArray c;
                if(v.type()!=is_object)
                    throw bad_value_cast();
                c.array        = v.get<std::vector<std::string> >("array");
                return c;
            }
            
            static void set(value &v,Data::JsonArray const &inv){
                v.set("array",inv.array);
                
            }
        };
        
        

    } // json
} // cppcms
#endif // TMPL_DATASTRUCT_H