#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <locale>

#include "apps/SmartAgent.h"

//-------------------------------------------------------------------------------------
// Dsc: Наш класс отрисовки страниц, при запросе некоторого адреса пользователем
//      В первую очередь он попадет сюда
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------


static cppcms::service* srv;
static bool got_sighup;

void handler()
{
    got_sighup = true;
    srv->shutdown();
}


int main(int argc,char **argv)
{
    //setlocale(LC_ALL,"");
    while(1) {
        try {
            // создаем сервис
            cppcms::service srv(argc,argv);
            // задаем корень
            srv.applications_pool().mount(cppcms::applications_factory<Apps::SmartAgent>());
            // запускаем
            srv.run();
        }
        catch(std::exception const &e) {
            std::cerr << "Failed: " << e.what() << std::endl;
            std::cerr << booster::trace(e) << std::endl;
            return 1;
        }
        if(got_sighup) {
            got_sighup = false;
            continue;
        }
        break;
    }
    
    
    return 0;
}



