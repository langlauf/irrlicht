// this example only runs in windows and demonstrates that Irrlicht 
// can run inside a win32 window.

#include <irrlicht.h>
#include <windows.h>

using namespace irr;

#pragma comment(lib, "irrlicht.lib")

HWND hOKButton;
HWND hCancelButton;
HWND hWnd;

static LRESULT CALLBACK CustomWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_COMMAND:
		{
			HWND hwndCtl = (HWND)lParam;
			int code = HIWORD(wParam);

			if (hwndCtl == hOKButton ||
				hwndCtl == hCancelButton)
			{
				DestroyWindow(hWnd);
				PostQuitMessage(0);
				return 0;
			}		
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


	


int main()
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hpre, LPSTR cmd, int cc)
{
	HINSTANCE hInstance = 0;
	// create dialog

	const char* Win32ClassName = "CIrrlichtWindowsTestDialog";

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)CustomWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= DLGWINDOWEXTRA; 
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= Win32ClassName;
	wcex.hIconSm		= 0;

	RegisterClassEx(&wcex);

	DWORD style = WS_SYSMENU | WS_BORDER | WS_CAPTION |
		WS_CLIPCHILDREN | WS_CLIPSIBLINGS;// | WS_CHILD;

	int windowWidth = 440;
	int windowHeight = 380;

	hWnd = CreateWindow( Win32ClassName, "Irrlicht Win32 window example",
		style, 100, 100, windowWidth, windowHeight,
		NULL, NULL, hInstance, NULL);

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	windowWidth = clientRect.right;
	windowHeight = clientRect.bottom;

	int okButtonWidth = 100;
	int okButtonHeight = 20;
	int borderDistance = 10;
	
	// create ok and cancel button

	hCancelButton = CreateWindow("BUTTON", "Cancel", WS_CHILD | WS_VISIBLE | BS_TEXT, 
		windowWidth - okButtonWidth*2 - borderDistance*2, 
		windowHeight - okButtonHeight - borderDistance,
		okButtonWidth, okButtonHeight, hWnd, NULL, hInstance, NULL);

	hOKButton = CreateWindow("BUTTON", "OK", WS_CHILD | WS_VISIBLE | BS_TEXT, 
		windowWidth - okButtonWidth - borderDistance, 
		windowHeight - okButtonHeight - borderDistance,
		okButtonWidth, okButtonHeight, hWnd, NULL, hInstance, NULL);

	// create a senseless scroll bar and some text
	
	int scrollBarWidth = 150;
	int scrollBarHeight = 15;
	int scrollBarXPos = borderDistance + 10;
	int scrollBarYPos = 0;
	int scrollBarHDistance = 35;
	int labelHeight = 20;
	int editControlWidth = 50;

	CreateWindow("SCROLLBAR", "", WS_CHILD | WS_VISIBLE | SBS_HORZ,
		scrollBarXPos, scrollBarYPos + (scrollBarHDistance + scrollBarHeight),
		scrollBarWidth, scrollBarHeight, hWnd, NULL, hInstance, NULL);

	CreateWindow("STATIC", "This is Irrlicht running inside a custom Win32 window.",
		WS_CHILD | WS_VISIBLE, 
		scrollBarXPos, scrollBarYPos + (scrollBarHDistance + scrollBarHeight) - labelHeight,
		400,labelHeight, hWnd, NULL, hInstance, NULL);

	CreateWindow("EDIT", "123", WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_BORDER,
		scrollBarXPos + scrollBarWidth + borderDistance, 
		scrollBarYPos + (scrollBarHDistance + scrollBarHeight),
		editControlWidth, scrollBarHeight, hWnd, NULL, hInstance, NULL);

	// create window to put irrlicht in

	int irrlichtWindowX = 50;
	int irrlichtWindowY = 80;
	int irrlichtWindowWidth = 320;
	int irrlichtWindowHeight = 230;

	HWND hIrrlichtWindow = CreateWindow("BUTTON", "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
			irrlichtWindowX, irrlichtWindowY, irrlichtWindowWidth, irrlichtWindowHeight, hWnd, 
			NULL, hInstance, NULL);

	// create irrlicht device in the button window

	irr::SIrrlichtCreationParameters param;
	param.WindowId = reinterpret_cast<s32>(hIrrlichtWindow); // hColorButton
	param.DriverType = video::EDT_DIRECTX8;

	irr::IrrlichtDevice* device = irr::createDeviceEx(param);
	
	// setup a simple 3d scene

	irr::scene::ISceneManager* smgr = device->getSceneManager();
	video::IVideoDriver* driver = device->getVideoDriver();

	scene::ICameraSceneNode* cam = smgr->addCameraSceneNode();
	cam->setTarget(core::vector3df(0,0,0));

	scene::ISceneNodeAnimator* anim = smgr->createFlyCircleAnimator(core::vector3df(0,10,0), 30.0f);
	cam->addAnimator(anim);
	anim->drop();

	scene::ISceneNode* cube = smgr->addTestSceneNode();
	
	cube->setMaterialTexture(0, driver->getTexture("../../media/rockwall.bmp"));

	smgr->addSkyBoxSceneNode(
	driver->getTexture("../../media/irrlicht2_up.jpg"),
	driver->getTexture("../../media/irrlicht2_dn.jpg"),
	driver->getTexture("../../media/irrlicht2_lf.jpg"),
	driver->getTexture("../../media/irrlicht2_rt.jpg"),
	driver->getTexture("../../media/irrlicht2_ft.jpg"),
	driver->getTexture("../../media/irrlicht2_bk.jpg"));

	// show and execute dialog

	ShowWindow(hWnd , SW_SHOW);
	UpdateWindow(hWnd);

	// do message queue

	// Instead of this, you can also simply use your own message loop
	// using GetMessage, DispatchMessage and whatever. Calling
	// Device->run() will cause Irrlicht to dispatch messages internally too. 
	// You need not call Device->run() if you want to do your own message 
	// dispatching loop, but Irrlicht will not be able to fetch
	// user input then and you have to do it on your own using the window
	// messages, DirectInput, or whatever.

	while (device->run())
	{
		driver->beginScene(true, true, 0);
		smgr->drawAll();
		driver->endScene();
	}

	// the alternative, own message dispatching loop without Device->run() would
	// look like this:

	/*MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}

		// draw engine picture
		driver->beginScene(true, true, 0);
		smgr->drawAll();
		driver->endScene();
	}*/

	device->closeDevice();
	device->drop();

	return 0;
}