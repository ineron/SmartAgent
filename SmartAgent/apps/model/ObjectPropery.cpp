
#include "../BaseApp.h"
#include "ObjectPropery.h"
//#include <uuid/uuid.h>


namespace Apps {
    namespace Model {
        
        
        ObjectPropery::ObjectPropery(std::string objectName):m_objectName(objectName) {}
        
        boost::any ObjectPropery::Property(const std::string &propertyName,const std::string &objRef)
        {
#ifdef DEBUG
            std::cout<<"******** userProperty("<< propertyName<<") *********"<<__LINE__<<std::endl;
#endif
            
            if (m_property.empty() || objRef.compare(m_ref))
            {
                //log("SESSION")<<"Init property "<<propertyName;
                init(objRef);
            }
            //return const_cast<boost::any>(m_userProperty[propertyName]);
            return m_property[propertyName];
        }
        
        void ObjectPropery::init(const std::string &objRef) const
        {
            std::cout<<"Init "<<objRef<<std::endl;
            //if(!BaseApp::instance()->loginLevel())
                //return;
            //cppdb::transaction transaction(BaseApp::instance()->dbsession());
            cppdb::result result;
            std::string query = std::string("select f.id,f.ref,f.field_name,type_id,\"value\",simple_value from " + m_objectName + "_property_fields f left join "+ m_objectName +"_property p on f.ref = p.field_ref  where p."+ m_objectName +"_ref = ? order by f.id");
            //UserPropertyFieldList prtList = session_.find<UserPropertyFields>();
            
            std::string ref;
            result = BaseApp::instance()->dbsession()<<query<<objRef;
            boost::any value;
            m_property.clear();
            while (result.next())
            {
                if(ref.empty())
                    ref = std::string(result.get<std::string>("ref"));
                
                switch (result.get<int>("type_id"))
                {
                        //std::cout<<__LINE__<<std::endl;
                    case ObjectPropery::Boolean:
                        value = boost::any((bool)result.get<int>("value") != 0);break;
                    case ObjectPropery::Numeric:
                        value = boost::any(result.get<int>("value"));break;
                    case ObjectPropery::String:
                        //std::cout<<result.get<std::string>("simple_value")<<__LINE__<<std::endl;
                        value = boost::any(result.get<std::string>("simple_value"));break;
                    case ObjectPropery::Date:
                        value = boost::any(result.get<time_t>("value"));break;
                    default:
                        value = boost::any(result.get<std::string>("simple_value"));break;
                }
                m_property.insert(std::pair<std::string,boost::any>( result.get<std::string>("field_name"),value));
                
            }
            m_ref = ref;
        }
        const std::string &ObjectPropery::id() const
        {
            return m_ref;
        }
        
    }
}