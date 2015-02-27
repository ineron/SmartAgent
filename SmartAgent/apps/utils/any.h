#include <memory>
#include <string>

namespace Apps {
    namespace Utils {

class any
{
public:

    template <class T>
    any(const T& t)
    {
        typedef type<T> assign_type;
        object = std::auto_ptr<assign_type>(new assign_type(t));
    
    }
    template <class T>
    any& operator = (T const& t)
    {
        typedef type<T> assign_type;
        object = std::auto_ptr<assign_type>(new assign_type(t));
        return *this;
    }

    template <class T>
    operator T ()
    {
        typedef type<T> assign_type;
        assign_type& type = dynamic_cast<assign_type&>(*object);
        return type.get();
    }

    
    
private:

    class base
    {
    public:

        virtual ~base() {}
    };

    typedef std::auto_ptr<base> base_ptr;

    template <class T>
    class type : public base
    {
    public:

        type(T const& t)
        : object(t)
        {
        }

        T get() const
        {
            return object;
        }

    private:

        T object;

    };

    base_ptr object;
};

    }
}