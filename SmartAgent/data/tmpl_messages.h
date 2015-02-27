#ifndef TMPL_MESSAGES_H
#define TMPL_MESSAGES_H
#include "tmpl_master.h"

namespace Data {
  //-------------------------------------------------------------------------------------
  // Dsc: Новостной контент
  //-------------------------------------------------------------------------------------
  struct Messages :public Master{
    //-------------------------------------------------------------------------------------
    // Dsc: Главная новость
    //-------------------------------------------------------------------------------------

	typedef struct _ATTACHMENTITEM{
	    int isImage;
	    int size;
	    std::string originalName;
	    std::string file;
	    std::string ref;
	}AttachmentItem;

	typedef struct _MESSAGEITEM{
	    int id;
	    std::string uid;
	    std::string messageRef;
	    int isUnread;
	    std::string senderName;
	    std::string senderEmail;
	    int isDraft;
	    std::string mark;
	    std::string subjectHeader;
	    std::string subject;
	    int isAttachment;
	    std::string attachmentList;
	    std::string messageDate;
	    std::string message;
	    int isWarning;
	    std::vector<AttachmentItem>attachments;
	}MessageItem;

	typedef std::vector<MessageItem>MessageItemList;
	
	struct MessageList:public Master{
	    MessageList(){};
	    ~MessageList(){};
	    MessageItemList messageItemList;
	};

	typedef struct _MESSAGEMENU
	{
	    int inbox;
	    int sending;
	    int draft;
	    int trash;
	}MessageMenu;

	struct Compose:public Master{
	    Compose(){};
	    ~Compose(){};
	};

	struct Read:public Master{
	    Read(){};
	    ~Read(){};
	    MessageItem currentMessage;
	};

	struct Reply:public Master{
	    Reply(){};
	    ~Reply(){};
	};


    std::string userRef;
    int newMsgCount;
    int allMsgCount;
    
    //std::string mainNews;
    //-------------------------------------------------------------------------------------
    // Dsc: Конструктор страницы
    //-------------------------------------------------------------------------------------
    Messages():newMsgCount(0) //:
      //Master()
    {}
    //-------------------------------------------------------------------------------------
    // Dsc: Ленивый деструктор
    //-------------------------------------------------------------------------------------
    ~Messages(){}
  };

}


#endif // TMPL_MESSAGES_H
