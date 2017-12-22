#include "PTZListener.h"

CPTZListener::CPTZListener(ULONG ip,int nPort)
{
	m_hListenThread = NULL;
	m_lpPTZListenerFunc = NULL;

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
	int buflen = 0;
    
    wVersionRequested = MAKEWORD( 1, 1 );
    
    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 ) 
    {
        throw;
    }
    if ( LOBYTE( wsaData.wVersion ) != 1 ||
        HIBYTE( wsaData.wVersion ) != 1 ) 
    {
        WSACleanup( );
        throw; 
    }

	m_sock=socket(AF_INET,SOCK_DGRAM,NULL);
    setsockopt(m_sock,SOL_SOCKET,SO_RCVBUF, (char*)&buflen,sizeof(buflen));
    SOCKADDR_IN sevaddr;
    sevaddr.sin_addr.S_un.S_addr=ip;
    sevaddr.sin_family=AF_INET;
    sevaddr.sin_port=htons(nPort);
    
    bind(m_sock,(SOCKADDR*)&sevaddr,sizeof(SOCKADDR));
}

CPTZListener::~CPTZListener(void)
{
    closesocket(m_sock);
    WSACleanup();
}

BOOL CPTZListener::Start(
	const PTZLISTENERPROC lpPTZListenerFunc,//回调方法的入口
	const LPVOID lpParam//回调方法的参数
	)
{
	assert(m_hListenThread == NULL);

	m_lpPTZListenerFunc = lpPTZListenerFunc;
	m_lpParam = lpParam;

	m_hListenThread = (HANDLE)_beginthreadex(NULL,0,CPTZListener::ListenThreadFunc,
		(LPVOID)this,0,NULL);
	if(m_hListenThread == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CPTZListener::Stop()
{
	if (m_hListenThread != NULL)
	{
		m_bRun = FALSE;
		WaitForSingleObject(m_hListenThread,100);
		CloseHandle(m_hListenThread);
	}

	return TRUE;
}

UINT CPTZListener::ListenThreadFunc(LPVOID lpParam)
{
	CPTZListener* pListener = (CPTZListener*)lpParam;
	MsgBase* pMsg = NULL;

	SOCKADDR_IN cliaddr;
    char clibuf[100];
    int len=sizeof(SOCKADDR);
	int pcgNum = 0;
	
	while(pListener->m_bRun)
	{
		int er = recvfrom(pListener->m_sock,clibuf,100,0,(SOCKADDR*)&cliaddr,&len);
		if(er > 0 && pListener->m_lpPTZListenerFunc != NULL)
		{
			int i = 0;
			while(i < er)
			{
				pMsg = (MsgBase*)(clibuf+i);
				if(pMsg->MsgPackNum != pcgNum)
				{
					pcgNum = pMsg->MsgPackNum;
					pMsg->MsgSuccess = pListener->m_lpPTZListenerFunc(pListener->m_lpParam,pMsg,(pMsg->MsgSize+4));
					pMsg->MsgCode++;
					sendto(pListener->m_sock,(char*)pMsg,sizeof(MsgBase),0,(SOCKADDR*)&cliaddr,len);
				}

			    i += (pMsg->MsgSize+4);
			}
		}
	}

	return 0;
}
