////////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "app/TrabeApp.h"

////////////////////////////////////////////////////////////////////////////////

CAppModule _Module;

////////////////////////////////////////////////////////////////////////////////

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

	int nRet = app.Run(nCmdShow);

	return nRet;
}

////////////////////////////////////////////////////////////////////////////////
