/*
 * pureHTTPSocket, from WZ_HTTPSocket
 */

#pragma once

#include <WinSock.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include "stdio.h"
#pragma   warning(disable:4996)
#pragma   warning(disable:4101)
#pragma	  warning(disable:4267)
#pragma	  warning(disable:4312)
#pragma	  warning(disable:4244)
#define			USING_UDP_CHECK			0

#define			FIRSTTIMEOUT			0      /* MSeconds */
#define			TIMEOUTs				300      /* MSeconds */
//#define			MAXPACKETSIZE			819200		/* max HTML body size */
#define			MAXURLSIZE				5000
#define			VERSIONINFO				"1.0"
#define         USER_AGENT              "Mozilla/5.0 (compatible; MSIE 10.7; Windows NT 7.1; mt/7.0)"
#define			REQ_LEN					81920
#define			PAGE_DIR_LEN			1024
#define			PHS_MIN(a,b)			(a<b)?a:b
#define			SESSION_STR_SIZE		130*3
#define			WZ_RECV_BUF_SIZE		4096

enum PostContentType
{
	CONTENT_TYPE_JSON,
	CONTENT_TYPE_FORM,
	CONTENT_TYPE_IMG,
};
#include <string>
#include <vector>
/* Standard headers */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

#define			XDRIVE_API			__declspec(dllexport)

namespace PURE_HTTPSocket{
	class SingleTonClass
	{
	public:
		static SingleTonClass* getInstance();
		~SingleTonClass(){};
	private:
		static SingleTonClass* instance;
		SingleTonClass(){};

		//把复制构造函数和=操作符也设为私有,防止被复制
		SingleTonClass(const SingleTonClass&){};
		SingleTonClass& operator=(const SingleTonClass&){};
	private:

	};
	/////////////////////////////////////////////////////////////////////
	// WZ_HttpSocket proxy class.
	// HttpHandler save handle functions as CALLBACK to WZ_HTTPSocket class
	// User can implement or inherint HttpHanlder to send/recv data.
	class XDRIVE_API HttpHandler
	{
	public:
		HttpHandler(){};
		~HttpHandler(){};
		virtual void wzh_onConnect(void* sender, size_t len) {
#ifdef _DEBUG
			printf("[ WZ: ] connect ok in thread:%d\n", GetCurrentThreadId());
#endif 
		};
		virtual void wzh_onReadSome(void* sender, char* buf, size_t len) {
		};
		virtual void wzh_onReadALL(void* sender, char* buf, size_t len) {
#ifdef _DEBUG
			//printf("[ WZ thread=%d] html=%s\n", GetCurrentThreadId(), buf);
#endif 
		};
		virtual void wzh_onClose(void* sender) {
			printf("Closing WZHSocket\n");
		};
	protected:
	private:
	};

	/***************************************************************
	* httpSocket class
	*
	***************************************************************/

	//// call back to tell connect ok or not
	//typedef void(HttpHandler::*connectStatePtr)(void* sender, size_t);
	//// call back function, read one time and call back,
	//typedef void(HttpHandler::*readBytesPtr)(void* sender, char*, size_t);
	///**
	// * callback function, read all finished and back.
	// * NOTE !! object sender should not be delete in this function . cause other
	// * procedure is still need to be execute after this call.
	// */
	//typedef void(HttpHandler::*readALLPtr)(void* sender, char*, size_t);


	class XDRIVE_API pureHTTPSocket
	{
	public:
		pureHTTPSocket(void);
		pureHTTPSocket(int tag);
		pureHTTPSocket(char* iden);
		pureHTTPSocket(char* url, char* iden);
		virtual ~pureHTTPSocket(void);
		void init_chars();
	public:
		HttpHandler default_handler;
		HttpHandler* pHttpHanderDelegate;;
		/*connectStatePtr ptrConnectState;
		readBytesPtr	ptrReadSome;
		readALLPtr		ptrReadAll;*/

#ifdef WIN32
		SOCKET	sock;
#else
		int     sock;
#endif

		int		tag;
		int		depth; // in which depth
		int     isPost;

		int		select_timeout;
		long    total_bytes;
		char	identifier[64];// string tag
		struct hostent *he;
		struct sockaddr_in saddr;
		char head_packet[REQ_LEN];
		//char	html[MAXPACKETSIZE];
		bool    flag_cleanALL; // for release class , when in recving loop if delete got , should break first and release
		char*	html;									// pointer to html all with headers
		size_t  html_bufsize;
		char	encodeType[32];							// UTF-8 or GBK, etc
		char	host[512];
		int		port;
		char	page_dir[PAGE_DIR_LEN];					// path to page with argvs
		size_t	header_len;
		char	html_header[WZ_RECV_BUF_SIZE];
	
		bool	logReport;								// if output log
		FILE*	fLog;
		int		names_num;
		std::vector<std::string> nicknameList;			// other nicknames in page
		char	cur_nickname[64];						// the user nickname of BBS
		char	session[SESSION_STR_SIZE];				// save the session string

		int		ret_status;		// header info ,status , 200, 301, etc
		//char	ret_encode[32];	// encoding type, in content-type

		char	pContentType[32];
		long	lContentLen;
		int		iPostContentType;		// json, application/x-www-form-urlencoded,...
	public:
		bool    parseURL(char* url);
		bool    redirectURL(char* url);
		int     init_ip_sock(char* ipaddr, int port = 80); // use cached ip
		char*   init_sock(char* hostname, int port = 80);  // use new dns ip
		int     connect_wait(void);
		char*	url_encode(char const *sUtf8, int len, int *new_length);
		void	setHTMLBuffer(char* outBuf, size_t bufsize);// 外部设置html缓冲区大小，方便处理
		size_t  do_get();
		size_t	post_JSON(char* jsonStr);
		size_t  post_UrlEncode(char* postStr);
		size_t	post_Multipart(char* postStr, char* boundary);
		size_t  RecvBytes(void); // better called in single thread for UI block
		int     setnonblock(int n_timeout = 0);

		// append post value into string for later use, utf8 is default
		void    addPostValue(char* key, char* vStr);
		// helper functions
		int cleanALL();
		
		size_t SendGETRequest();
		size_t SendPOSTRequest(char* url, char* postValuesStr);
		size_t SendPOSTRequest(char* url, char* postValuesStr, char* boundary);
		size_t SendPackets(char* buf, int len);

		void setLogReport();
		bool replaceTag(char* inStr, char* tag, char* replacedTag, char* replacedStr);
		char* change2lower(char* s, bool set_return_to_blank = false);
		int UnHtml(char* html, char* text, int maxout);
		int br2return(char* html, char* back_txt); // change <br> to \n for print out

		// retrun 1 if success else 0
		int getHTMLHeader();	
		// parse Header info and return Content-Length
		// if found content-length, return the number, else return default size 81920, html[81920]
		int parseHeaderInfo(); // return HTTP Status number, ex:200
	};
};
