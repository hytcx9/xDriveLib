#define			XDRIVE_API			__declspec(dllexport)
typedef unsigned char byte;
#ifdef WIN32
#include <WinSock2.h>
#endif

#include <string>
#include <vector>
/* Standard headers */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

enum  wzSocketResult{
	WZSR_OK,
	WZSR_Err_ConnectFailed,
	WZSR_Err_SelectTimeOut
};

const int websock_buf_size = 8192;
namespace WZSocket{

	class XDRIVE_API WZSocketHandler
	{
	public:
		WZSocketHandler(){};
		~WZSocketHandler(){};
		virtual void wzsock_onConnect(int err) {
#ifdef DEBUG
			printf("[ DEBUG ] connect ok in thread:%d\n", GetCurrentThreadId());
#endif // _DEBUG
		};
		virtual void wzsock_onData(char* buf, size_t len) {};
		virtual void wzsock_onClose() {};
		virtual void wzsock_onError(void* sender, int err) {
		};
	protected:
	private:
	};

	class XDRIVE_API WZSocketClient
	{
	public:
		WZSocketClient();
		~WZSocketClient();
		virtual int connectServer(char* remoteAddr, int port);
		int RecvLoop();	// better called in single thread for UI block
		int sendData(char* msgData, int len);
		void disConnect();

	private:
		int sendPacket(char* msg, int len);
	public:
		WZSocketHandler* handlerDelegate;
#ifdef WIN32
		SOCKET	m_Sock;
#else
		int     sock;
#endif
		char	m_Host[512];
		char	dataBuf[websock_buf_size];
		int		m_Port;
		int		select_timeout;
		struct hostent *he;
		struct sockaddr_in saddr;
		bool handshakeOK;
	};
};