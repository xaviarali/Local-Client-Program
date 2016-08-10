// local_client_program_V2.0.cpp : Defines the entry point for the console application.
//

// oneir_local_client.cpp : Defines the entry point for the console application.
//
// header files
#include "stdafx.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <io.h>
#include<iostream>
#include <stdio.h>

#include<string.h>
#include<time.h>
#include<winsock2.h>
#include<ws2tcpip.h> 
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#pragma comment(lib,"Ws2_32.lib")
#include "tagSession.h"
#include "globals.h"
#include <tchar.h>
#include <psapi.h>
int tabId = -1;
int subMenu = 0;
bool login = false;
bool show_window = false;
bool hide_window = false;
bool back_button = false;
int VERT = 0;
int HEIGHT = 0;
struct sessions
{
	HWND handler;
	bool id;
};
bool isSessionActive(int id);
void bringtoTop(int v, int h, HWND hh);
sessions active_sessions[50];

typedef VOID(CALLBACK* ETNDATAPROC)(HWND, LPBYTE, DWORD);
typedef BOOL(__stdcall *etnEnableInput)(HWND hSession, BOOL bEnable);
typedef BOOL(__stdcall *etnIsSession)(HWND hSession);
typedef HWND(__stdcall *etnObtainHandle)(LPCTSTR lpSessionName, DWORD dwMilliseconds);
typedef BOOL(__stdcall *etnReceiveData)(HWND hSession, LPBYTE lpData, LPDWORD lpcbData, DWORD dwMilliseconds);
typedef BOOL(__stdcall *etnRegisterCallback)(HWND hSession, ETNDATAPROC lpDataProc);
typedef BOOL(__stdcall *etnReleaseHandle)(HWND hSession);
typedef BOOL(__stdcall *etnSendData)(HWND hSession, LPBYTE lpData, DWORD cbData);
typedef BOOL(__stdcall *etnSetDelay)(DWORD dwMilliseconds);
typedef BOOL(__stdcall *etnReadSession)(LPBYTE filepath, tagSESSION *session);
typedef BOOL(__stdcall *etnWriteSession)(LPBYTE filepath, tagSESSION *session);


// loads the .DLL provided by Erik's Telnet98
static HANDLE GetEtnApi(void)
{
	static HANDLE hDLL = NULL;
#if _WIN64
	if (!hDLL)
		hDLL = LoadLibrary(L"ETNAPI64.DLL");
#else
	if (!hDLL)
		hDLL = LoadLibrary(L"ETNAPI32.DLL");
#endif
	if (!hDLL) {
		MessageBox(NULL, L"Failed to load ETNAPI", (LPCWSTR)__FILE__, MB_ICONSTOP);
		exit(1);
	} // if
	return hDLL;
} // GetEtnApi


  // wrapper function for etnEnableInput which is loaded from the .DLL file
BOOL WINAPI xetnEnableInput(HWND hSession, BOOL bEnable)
{
	//static BOOL(STDAPICALLTYPE *etnEnableInput)(HWND, BOOL) = NULL;
	static etnEnableInput x = NULL;
	if (!x)
		x = (etnEnableInput)GetProcAddress((HMODULE)GetEtnApi(), "etnEnableInput");
	if (x)
		return x(hSession, bEnable);
	return FALSE;
} // etnEnableInput


  // wrapper function for etnIsSession which is loaded from the .DLL file
BOOL WINAPI xetnIsSession(HWND hSession)
{
	//static BOOL(STDAPICALLTYPE *etnIsSession)(HWND) = NULL;
	static etnIsSession x = NULL;
	if (!x)
		x = (etnIsSession)GetProcAddress((HMODULE)GetEtnApi(), "etnIsSession");
	if (x)
		return x(hSession);
	return FALSE;
} // etnIsSession


  // wrapper function for etnObtainHandle which is loaded from the .DLL file
HWND WINAPI xetnObtainHandle(LPCTSTR lpSessionName, DWORD dwMilliseconds)
{
	//static HWND(STDAPICALLTYPE *etnObtainHandle)(LPCTSTR, DWORD) = NULL;
	static etnObtainHandle x = NULL;
	if (!x)
		x = (etnObtainHandle)GetProcAddress((HMODULE)GetEtnApi(), "etnObtainHandle");
	if (x)
		return x(lpSessionName, dwMilliseconds);
	return NULL;
} // etnObtainHandle


  // wrapper function for etnReceiveData which is loaded from the .DLL file
BOOL WINAPI xetnReceiveData(HWND hSession, LPBYTE lpData, LPDWORD lpcbData, DWORD dwMilliseconds)
{
	//static BOOL(STDAPICALLTYPE *etnReceiveData)(HWND, LPBYTE, LPDWORD, DWORD) = NULL;
	static etnReceiveData x = NULL;
	if (!x)
		x = (etnReceiveData)GetProcAddress((HMODULE)GetEtnApi(), "etnReceiveData");
	if (x)
		return x(hSession, lpData, lpcbData, dwMilliseconds);
	return FALSE;
} // etnReceiveData


  // wrapper function for etnRegisterCallback which is loaded from the .DLL file
BOOL WINAPI xetnRegisterCallback(HWND hSession, ETNDATAPROC lpDataProc)
{
	//static BOOL(STDAPICALLTYPE *etnRegisterCallback)(HWND, ETNDATAPROC) = NULL;
	static etnRegisterCallback x = NULL;
	if (!x)
		x = (etnRegisterCallback)GetProcAddress((HMODULE)GetEtnApi(), "etnRegisterCallback");
	if (x)
		return x(hSession, lpDataProc);
	return FALSE;
} // etnRegisterCallback


  // wrapper function for etnReleaseHandle which is loaded from the .DLL file
BOOL WINAPI xetnReleaseHandle(HWND hSession)
{
	//static BOOL(STDAPICALLTYPE *etnReleaseHandle)(HWND) = NULL;
	static etnReleaseHandle x = NULL;
	if (!x)
		x = (etnReleaseHandle)GetProcAddress((HMODULE)GetEtnApi(), "etnSetDelay");
	if (x)
		return x(hSession);
	return FALSE;
} // etnReleaseHandle


  // wrapper function for etnSendData which is loaded from the .DLL file
BOOL WINAPI xetnSendData(HWND hSession, LPBYTE lpData, DWORD cbData)
{
	//static BOOL(STDAPICALLTYPE *etnSendData)(HWND, LPBYTE, DWORD) = NULL;
	static etnSendData x = NULL;
	if (!x)
		x = (etnSendData)GetProcAddress((HMODULE)GetEtnApi(), "etnSendData");
	if (x)
		return x(hSession, lpData, cbData);
	return FALSE;
} // etnSendData


  // wrapper function for etnSetDelay which is loaded from the .DLL file
VOID WINAPI xetnSetDelay(DWORD dwMilliseconds)
{
	//static VOID(STDAPICALLTYPE *etnSetDelay)(DWORD) = NULL;
	static etnSetDelay x = NULL;
	if (!x)
		x = (etnSetDelay)GetProcAddress((HMODULE)GetEtnApi(), "etnSetDelay");
	if (x)
		x(dwMilliseconds);
} // etnSetDelay


  // wrapper function for etnSetDelay which is loaded from the .DLL file
BOOL WINAPI xetnReadSession(LPBYTE filepath, tagSESSION *session)
{
	static etnReadSession x = NULL;
	if (!x)
		x = (etnReadSession)GetProcAddress((HMODULE)GetEtnApi(), "etnReadSession");
	if (x)
		return x(filepath, session);
	printf("ERROR %d \n", GetLastError());
	return FALSE;
} // etnReadSession




  // wrapper function for etnSetDelay which is loaded from the .DLL file
BOOL WINAPI xetnWriteSession(LPBYTE filepath, tagSESSION *session)
{
	static etnWriteSession x = NULL;
	if (!x)
		x = (etnWriteSession)GetProcAddress((HMODULE)GetEtnApi(), "etnWriteSession");
	if (x)
		return x(filepath, session);
	return FALSE;
} // etnWriteSession


  /*
  *  temporary function which extracts a character from a char* and store it into global variable 'choice'
  */
void check(char* s)
{
	if (s[96] == 'x')
	{
		printf("Command Recieved from the Cloud.(Login)\n");
		choice = (int)(s[96] - '0');
		login = true;
	}
	if (s[96] >= '1' && s[96] <= '9')
	{
		printf("Command Recieved from the Cloud.(Menu option)\n");
		choice = (int)(s[96] - '0');
	}

	if (s[98] >= '1' && s[98] <= '9')
	{
		printf("(SubMenuId)\n");
		subMenu = (int)(s[100] - '0');
	}

	if (s[100] >= '1' && s[100] <= '9')
	{
		printf("(TabId).\n");
		tabId = (int)(s[100] - '0');
	}

	if (s[96] == 'm')
	{
		show_window = true;
	}

	if (s[96] == 'h')
	{
		hide_window = true;
	}

	if (s[96] == 'b')
	{
		printf("(Back Button).\n");
		back_button = true;
	}

}
/*
* client which sends a session Id to the server and recives a JSON data in responce
*/
int client()
{
	// username hasnt set
	if (credentials[username] == NULL)
	{
		printf("Username couldnt be read.\n");
		return -23;
	}
	const int request_line = 150;
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char message[request_line], recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	// 54.235.98.223:443
	server.sin_addr.s_addr = inet_addr("54.235.98.223");
	server.sin_family = AF_INET;
	server.sin_port = htons(80);

	//Connect to remote server
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}
	//Send some data
	sprintf_s(message, "GET /oneir?q=%s HTTP/1.1\r\nHost: %s\r\n\r\n", credentials[username], REMOTE_HOST_IP);
	if (send(s, message, strlen(message), 0) < 0)
	{
		puts("Send failed");
		return 1;
	}

	//Receive a reply from the server
	int iResult = 0;
	// Receive until the peer closes the connection
	int i = 0;
	while ((iResult = recv(s, recvbuf, recvbuflen, 0)) > 0)
	{
		recvbuf[iResult - 1] = '\0'; break;
	}
	check(recvbuf);
} // end of client()

bool sendDataToTelnet98()
{
	HWND    hSession = NULL;
	DWORD   cbData = 0;
	char    buf[ETNBUFSIZ];
	if (back_button)
	{
		if (!isSessionActive(tabId))
		{
			return false;
		}
		printf("(Backed).\n");
		back_button = false;
		char cmd[2];
		// escape button ascii value
		cmd[0] = 0x1b;
		// null terminate the string
		cmd[1] = '\0';
		// get the handler stored in the datastructure
		hSession = active_sessions[tabId].handler;
		// send back button command to telnet
		xetnSendData(hSession, (LPBYTE)cmd, 1);
		ShowWindow(active_sessions[tabId].handler, SW_RESTORE);
		ShowWindow(hSession, SW_SHOWMAXIMIZED);
		ShowWindow(hSession, SW_SHOWNORMAL);
        SetWindowPos(hSession, HWND_TOPMOST, (HEIGHT * 25) / 100, 0, (HEIGHT * 75) / 100, VERT - 50, 0x0040);
		xetnEnableInput(hSession, TRUE);
		return true;
	}

	if (hide_window)
	{
		if (isSessionActive(tabId))
		{
			ShowWindow(active_sessions[tabId].handler, SW_MINIMIZE);
			SetWindowPos(hSession, HWND_NOTOPMOST, (HEIGHT * 25) / 100, 0, (HEIGHT * 75) / 100, VERT - 50, 0x0040);
		}
		return true;
	}
	else if (show_window)
	{
		if (isSessionActive(tabId))
		{
			ShowWindow(active_sessions[tabId].handler, SW_RESTORE);
			//ShowWindow(active_sessions[tabId].handler, SW_MAXIMIZE);
			//SetForegroundWindow(active_sessions[tabId].handler);
			//ShowWindow(active_sessions[tabId].handler, SW_RESTORE);
			ShowWindow(active_sessions[tabId].handler, SW_SHOWMAXIMIZED);
			ShowWindow(active_sessions[tabId].handler, SW_SHOWNORMAL);
			SetWindowPos(active_sessions[tabId].handler, HWND_TOPMOST, (HEIGHT * 25) / 100, 0, (HEIGHT * 75) / 100, VERT - 50, 0x0040);

		}
		return true;
	}
	/*
	* increase this value if your application
	* is too slow to fetch all incoming data
	*/
	etnSetDelay(10);

	/*
	*  Hard Coded Data which is being sent to the telnet98 session
	*/
	if (login)
	{
		HWND h = NULL;
		if (hSession = xetnObtainHandle(NULL, (DWORD)1000)) {

			xetnEnableInput(hSession, FALSE);
			//HWND h = GetTopWindow(NULL);
			//ShowWindow(h, SW_SHOWNORMAL);
			strncpy_s(buf, sizeof(buf), "1", 1);
			xetnSendData(hSession, (LPBYTE)"1", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"D", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"E", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"M", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"O", (DWORD)strlen(buf));
			xetnSendData(hSession, (LPBYTE)"\r", (DWORD)strlen(buf));
			active_sessions[tabId].id = true;
			active_sessions[tabId].handler = hSession;
			xetnEnableInput(hSession, TRUE);
			ShowWindow(hSession, SW_RESTORE);
			SetWindowPos(hSession, HWND_TOPMOST, (HEIGHT * 25) / 100, 0, (HEIGHT * 75) / 100, VERT - 50, 0x0040);
			return true;
		}
	}
	else
	{
		if (isSessionActive(tabId))
		{
			hSession = active_sessions[tabId].handler;
			char cmd[2];
			if (choice)
			{
				cmd[0] = (char)(choice + '0');
				cmd[1] = '\0';
				xetnSendData(hSession, (LPBYTE)cmd, 1);
			}
			else
			{
				cmd[0] = (char)(subMenu + '0');
				cmd[1] = '\0';
				xetnSendData(hSession, (LPBYTE)cmd, 1);
			}
			ShowWindow(active_sessions[tabId].handler, SW_RESTORE);
			ShowWindow(hSession, SW_SHOWMAXIMIZED);
			ShowWindow(hSession, SW_SHOWNORMAL);
			ShowWindow(hSession, SW_RESTORE);
			SetWindowPos(hSession, HWND_TOPMOST, (HEIGHT * 25) / 100, 0, (HEIGHT * 75) / 100, VERT - 50, 0x0040);
			xetnEnableInput(hSession, TRUE);
			return true;
		}
	}
	// if
	printf("No Telnet98 Running!\n");
	return false;
}// end of sendDataToTelnet

 // check whether telnet98 session is running
bool isTelnetReady()
{
	if (xetnObtainHandle(NULL, (DWORD)10000) == NULL) return 0;
	return 1;
}

// wait telnet to load , time may vary with the system
void wait(unsigned timeout)
{
	timeout += clock();
	while (clock() < timeout) continue;
}

// Get the horizontal and vertical screen sizes in pixel
void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}// end of GetDesktopResolution

 // resizes the telnet98 window
void resizeApps(int v, int h)
{   //HWND_TOPMOST  0x0400
	// create an Windows Handler Object
	HWND Window = NULL;
	// get the handle of current running telnet98 Windows
	Window = FindWindowEx(0, Window, L"tn98FrameClass", 0);
	// check if handle exists, if it exists then adjust the telnet98 size and position
	if (Window != NULL)
		SetWindowPos(Window, HWND_TOPMOST, (h * 25) / 100, 0, (h * 75) / 100, v - 50, 0x0040);
	// check if handle exists, if it exists then adjust the telnet98 size and position
	Window = xetnObtainHandle(NULL, (DWORD)1000);
	// check if handle exists, if it exists then adjust the telnet98 size and position
	if (Window != NULL)
		SetWindowPos(Window, HWND_TOPMOST, (h * 25) / 100, 0, (h * 75) / 100, v - 50, 0x0040);
}// end of resizeApps


 // grab user's credential from credentials .ini
void readCredentials(char** array)
{
	// get a file pointer to .INI file
	FILE* credential_file = fopen(path_to_ini_file, "r");
	// file doesnt exist
	if (credential_file == NULL)
	{
		printf("Error reading credentials.Please check credential.ini file.");
		return;
	}
	// hard-code the path folder to Session File i.e .ETX file
	for (int j = 1; j < CREDENTIAL_SIZE_LIMIT; ++j)
		strcpy(array[j], path);
	// counter temp variable
	int i = 0;
	// small space for holding the usernama and .ETX file
	char buffer[1000];
	// read data from the file until hit EOF
	while (fscanf(credential_file, "%s : %s\n", buffer, buffer) != EOF && i < CREDENTIAL_SIZE_LIMIT)
	{
		// check for buffer over-flows
		if (strlen(buffer) + strlen(path) > CREDENTIAL_SPACE - 1)
		{
			printf("Too long filename size in credential.ini.\n");
			return;
		}
		// when data read is not user's email/username
		if (i != 0)
			strcat(array[i], buffer);
		// when data read is name of .ETX file
		else
			strcpy(array[i], buffer);

		// for debugging print data
		// printf("%s\n\t\t\tLenght=%d\n\n", array[i],strlen(array[i]));
		// increment the counter
		i++;
	}
	// let the global world know, the size of credentials
	CREDENTIALS_SIZE = i;
	// close the file
	fclose(credential_file);
}
// end of readCredentials

// intialize gloabal array for storing Credentials i.e username, session file names etc
void initCredentials(char** array)
{
	// iterate over each pointer
	for (int i = 0; i < CREDENTIAL_SIZE_LIMIT; i++)
		// allocate 100 bytes for each pointer
		array[i] = new char[100];

}
//end of initCredentials
void readUserInfoFromSession()
{
	tagSESSION* session = (tagSESSION*)malloc(sizeof(tagSESSION));
	if (xetnReadSession((LPBYTE)credentials[2], session))
	{
		printf("Username:%s\nPassword:%s\n", session->UserID, session->Passwd);
		printf("SessionName:%s\nID:%s\n", session->SessionName, session->ID);
		char* s = "Mehri Khari";
		strcpy(session->UserID, s);
		xetnWriteSession((LPBYTE)credentials[2], session);
	}

}

bool isSessionActive(int id)
{
	if (id == -1)
	{
		cout << "Invalid Tab Id\n";
		return false;
	}
	if (active_sessions[id].id && active_sessions[id].handler != NULL)
	{
		cout << "Previous Session Preserved!!\n";
		return true;
	}
	cout << "Session Lost\n";
	return false;
}

void initializeActiveSessionArray()
{
	for (int i = 0; i < 30; i++)
	{
		active_sessions[i].id = false;
		active_sessions[i].handler = NULL;
	}
}

void stayOnfront()
{
	for (int i = 0; i < 30; i++)
	{
		if (active_sessions[i].id && active_sessions[i].handler)
			SetForegroundWindow(active_sessions[i].handler);
	}
}

void letOtherTelnetLoad(int v, int h)
{   //HWND_TOPMOST  0x0400
	// create an Windows Handler Object
	HWND Window = NULL;
	// get the handle of current running telnet98 Windows
	Window = FindWindowEx(0, Window, L"tn98FrameClass", 0);
	// check if handle exists, if it exists then adjust the telnet98 size and position
	if (Window != NULL)
		SetWindowPos(Window, HWND_BOTTOM, (h * 25) / 100, 0, (h * 75) / 100, v - 50, 0x0040);
	// check if handle exists, if it exists then adjust the telnet98 size and position
	Window = xetnObtainHandle(NULL, (DWORD)1000);
	// check if handle exists, if it exists then adjust the telnet98 size and position
	if (Window != NULL)
		SetWindowPos(Window, HWND_BOTTOM, (h * 25) / 100, 0, (h * 75) / 100, v - 50, 0x0040);
}// end of resizeApps

void bringtoTop(int v, int h, HWND hh)
{   //HWND_TOPMOST  0x0400
	SetWindowPos(hh, HWND_TOPMOST, (h * 25) / 100, 0, (h * 75) / 100, v - 50, 0x0040);
}// end of resizeApps

void minimizeAllTelnetsExceptMe()
{
	for (int i = 0; i < 30; i++)
	{
		if (active_sessions[i].id && active_sessions[i].handler) {
			ShowWindow(active_sessions[i].handler,SW_FORCEMINIMIZE);
			SetWindowPos(active_sessions[i].handler, HWND_NOTOPMOST, (HEIGHT * 25) / 100, 0, (HEIGHT * 75) / 100, VERT - 50, 0x0040);
		}
	}
}
int main(void)
{
	initializeActiveSessionArray();
	int vert, horz, taskbar_sz;
	GetDesktopResolution(horz, vert);
	VERT = vert;
	HEIGHT = horz;
	initCredentials(credentials);
	readCredentials(credentials);
	printf("\n\t\t\tSession Id: %s\n", credentials[username], CREDENTIALS_SIZE);
	readUserInfoFromSession();
	while (true)
	{
		resizeApps(vert, horz);
		Sleep(1000);
		//readCredentials(credentials);
		client();
		//end of server
		if (choice != 0 || subMenu != 0 || show_window || hide_window || back_button)
		{
			//Sleep(25);
			if (login)
			{
				//letOtherTelnetLoad(vert, horz);
				//if (choice == 5 || choice == 6) system(terminal);
				//else 
				minimizeAllTelnetsExceptMe();
				system(telnet98);
				Sleep(6000);
			}
			resizeApps(vert, horz);

			printf("Waiting For Telnet To load\n");
			while (!isTelnetReady());
			system("COLOR 2");
			printf("Time Ended For Telnet To load\n");
			sendDataToTelnet98();
			choice = 0;
			subMenu = 0;
			login = false;
			erik98 = NULL;
			tabId = -1;
			show_window = false;
			hide_window = false;
			back_button = false;
		}
	}
	return 0;
} // main