////////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "app/TrabeApp.h"

////////////////////////////////////////////////////////////////////////////////

CAppModule _Module;

////////////////////////////////////////////////////////////////////////////////

void ConsoleEcho(BOOL bEcho)
{
	if(!bEcho)
		return;

	// 带窗口的应用程序，创建一个临时的控制台，以供cout输出
	AllocConsole();

	HANDLE hin = ::GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hout = ::GetStdHandle(STD_OUTPUT_HANDLE);

	int hcin = _open_osfhandle((intptr_t)hin,_O_TEXT);
	FILE* fpin = _fdopen(hcin,"r");
	*stdin = *fpin; 

	int hcout = _open_osfhandle((intptr_t)hout,_O_TEXT);
	FILE* fpout = _fdopen(hcout,"wt");
	*stdout = *fpout;

	std::ios_base::sync_with_stdio();

	std::cout << "Test Console Echo" << endl << endl;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(iccx);
	iccx.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
	BOOL bRet = ::InitCommonControlsEx(&iccx);
	bRet;
	ATLASSERT(bRet);

	//my app class
	TrabeApp app(_Module);
	
	HRESULT hRes = app.Init(hInstance);
	hRes;
	ATLASSERT(SUCCEEDED(hRes));
    
    // 控制台
	// ConsoleEcho(TRUE);
    // printf("123\n");
    // OutputDebugString(LPCWSTR(_T("Hello WTL\n")));
    OutputDebugPrintf(_T("Hello WTL\n"));
    
	int nRet = app.Run(nCmdShow);

	return nRet;
}

////////////////////////////////////////////////////////////////////////////////
