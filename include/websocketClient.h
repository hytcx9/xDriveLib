#pragma once
//////////////////////////////////////////////////////////////////////////
/*
WebSocket request headers:
-------------------
GET ws://47.94.131.95:2346/ HTTP/1.1
Host: 47.94.131.95:2346
Connection: Upgrade
Pragma: no-cache
Cache-Control: no-cache
Upgrade: websocket
Origin: http://47.94.131.95:8001
Sec-WebSocket-Version: 13
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36
Accept-Encoding: gzip, deflate, sdch
Accept-Language: zh-CN,zh;q=0.8,en;q=0.6
Sec-WebSocket-Key: SYO+T/UjRLnwpwFKS8rUkg==
Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits
--------------------
Response headers:
-------------------
Connection:Upgrade
Sec-WebSocket-Accept:gQ/1/Wd6b5URc59g4npELXOHKkg=
Sec-WebSocket-Version:13
Server:workerman/3.4.2
Upgrade:websocket
-----------------------------end 

*/
//////////////////////////////////////////////////////////////////////////
//
//#define			FIRSTTIMEOUT			0      /* MSeconds */
//#define			TIMEOUTs				300      /* MSeconds */
//#define			MAXPACKETSIZE			819200		/* max HTML body size */
//#define			MAXURLSIZE				5000
//#define			VERSIONINFO				"1.0"
//#define         USER_AGENT              "Mozilla/5.0 (compatible; MSIE 10.7; Windows NT 7.1; RT/7.0)"
//#define			REQ_LEN					819200
//#define			PAGE_DIR_LEN			1024
//#define			MIN(a,b)				(a<b)?a:b
//#define			SESSION_STR_SIZE		130*3
//#define			WZ_RECV_BUF_SIZE		4096
#define			XDRIVE_API			__declspec(dllexport)
typedef unsigned char byte;
#ifdef WIN32
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#endif

#include <string>
#include <vector>
/* Standard headers */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

#define     OP_FIT_SET     0x00
#define     OP_TXT_SET     0x01
#define     OP_BIN_SET     0x02
#define     OP_CLO_SET     0x08
#define     OP_PNG_SET     0x09
#define     OP_POG_SET     0x0a
#define     OP_LFT_SET     0x0b

enum  websockClientResult{
	WSC_OK,
	Error_HostNotFound,
	Error_ConnectFailed,
	Error_SelectTimeOut,
	Error_VirtualAllocEx
};

const int websock_buf_size = 8192;
namespace WEBSocketClient{

	class XDRIVE_API WebSocketHandler
	{
	public:
		WebSocketHandler(){};
		~WebSocketHandler(){};
		virtual void websocket_onConnecting(int state) {};
		virtual void websocket_onConnect(int err) {
#ifdef DEBUG
			printf("[ DEBUG ] connect ok in thread:%d\n", GetCurrentThreadId());
#endif // _DEBUG
		};
		virtual void websocket_onMessage(char* buf, size_t len) {};
		virtual void websocket_onClose() {};
		virtual void websocket_onError(void* sender, int err) {
		};
	protected:
	private:
	};

	class XDRIVE_API websocketClient
	{
	public:
		websocketClient();
		websocketClient(int id);
		~websocketClient();
		int init_sock();
		virtual int connectWSServer(char* remoteAddr, bool useCacheIP = false);
		bool parseURL(char* remoteAddr);					// parse ws url
		int RecvLoop(); // better called in single thread for UI block
		bool parseHeadInfo(char* buf);						// get websocket protocol info and judge
		int sendText(char* msg, int len);			// send text string
		int sendBinData(byte* msgData);
		void disConnect();

		char* getCacheIP() { return cacheIP; };
		void updateCacheIP(char* newip);
	private:
		int sendPacket(char* msg, int len);
	public:
		WebSocketHandler* handlerDelegate;
		bool	isSockConnected;
		char	host[512];
		char	dataBuf[websock_buf_size];
		int		port;
		int		select_timeout;
		int		wsId;
	private:
#ifdef WIN32
		SOCKET	sock;
#else
		int     sock;
#endif
		char	cacheIP[20];
		struct hostent *he;
		struct sockaddr_in saddr;
		bool handshakeOK;
	};
};


