// This is the current Techdemo of the Irrlicht Engine (c) 2003 by N.Gebhardt.
// This file is not documentated.

#include "CMainMenu.h"

CMainMenu::CMainMenu()
: startButton(0), device(0), start(false), fullscreen(true), selected(1),
	music(true), shadows(false)
{
}



bool CMainMenu::run(bool& outFullscreen, bool& outMusic, bool& outShadows,
					video::EDriverType& outDriver)
{
	device = createDevice(video::DT_SOFTWARE,
		core::dimension2d<s32>(512, 384), 16, false, false, this);

	device->getFileSystem()->addZipFileArchive("irrlicht.dat");
	device->getFileSystem()->addZipFileArchive("..\\..\\media\\irrlicht.dat");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	wchar_t tmp[255];
	swprintf(tmp, 255, L"Irrlicht Engine Techdemo v%s", device->getVersion());
	device->setWindowCaption(tmp);

	// load font

	gui::IGUIFont* font = guienv->getFont("../../media/fonthaettenschweiler.bmp");
	if (font)
		guienv->getSkin()->setFont(font);

	// add images

	gui::IGUIImage* img = guienv->addImage(core::rect<int>(0,0,512,384));
	img->setImage(driver->getTexture("..\\..\\media\\techdemoback.bmp"));

	const s32 leftX = 280;

	// add list box

	gui::IGUIListBox* box = guienv->addListBox(core::rect<int>(leftX,23,485,90), 0, 1);
	box->addItem(L"Irrlicht Software Engine [too slow]");
	box->addItem(L"Direct3D 8.1");
	box->addItem(L"OpenGL 1.2");
	box->setSelected(selected);

	// add button

	startButton = guienv->addButton(core::rect<int>(420,105,485,130), 0, 2, L"Start Demo");

	// add checkbox

	guienv->addCheckBox(fullscreen, core::rect<int>(leftX,105,400,130), 0, 3, L"Fullscreen");
	guienv->addCheckBox(music, core::rect<int>(leftX,130,400,155), 0, 4, L"Music & Sfx");
	guienv->addCheckBox(shadows, core::rect<int>(leftX,160,400,175), 0, 5, L"Shadows");

	// add text

	wchar_t* text[] = {		L"Welcome to the Irrlicht Engine.",
							L"Please select a device for the",
							L"interactive demo. What you currently",
							L" see is displayed using the Software",
							L"Renderer, but this would be too slow",
							L"for the demo. The Irrlicht Engine was",
							L"written by me, N.Gebhardt. The models,",
							L" maps and textures were placed at my",
							L"disposal by B.Collins, M.Cook and",
							L"J.Marton. The music was created by",
							L"M.Rohde and is played back by Audiere.", 
							0};

	const int textHeight = 15;
	const int top = 200;

	int t=0;
	while(text[t])
	{
		gui::IGUIStaticText* st = guienv->addStaticText(text[t], false, 
			core::rect<int>(leftX, top + t*textHeight, 495, top + (t+1)*textHeight));

		st->setOverrideColor(video::SColor(255,156,156,156));
		++t;
	}
	
	// add md2 model

	scene::IAnimatedMesh* mesh = smgr->getMesh("..\\..\\media\\faerie.md2");
	scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
	if (node)
	{
		node->setMaterialTexture(0, driver->getTexture("..\\..\\media\\faerie2.bmp"));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
		node->setFrameLoop(0, 310);
	}

	smgr->addCameraSceneNode(0, core::vector3df(35,0,0), core::vector3df(0,0,20));

	// irrlicht logo
	video::ITexture* irrlichtLogo = driver->getTexture("../../media/irrlichtlogo.bmp");

	// draw all

	while(device->run())
	{
		if (device->isWindowActive())
		{
			driver->beginScene(false, true, video::SColor(0,0,0,0));
			guienv->drawAll();
			smgr->drawAll();	

			if (irrlichtLogo)
				driver->draw2DImage(irrlichtLogo, core::position2d<int>(10,10));

			driver->endScene();
		}
	}

	device->drop();

	outFullscreen = fullscreen;
	outMusic = music;
	outShadows = shadows;


	switch(selected)
	{
	case 0:	outDriver = video::DT_SOFTWARE; break;
	case 1:	outDriver = video::DT_DIRECTX8; break;
	case 2:	outDriver = video::DT_OPENGL; break;
	}

	return start;
}



bool CMainMenu::OnEvent(SEvent event)
{
	if (event.EventType == EET_GUI_EVENT)
	{
		s32 id = event.GUIEvent.Caller->getID();
		switch(id)
		{
		case 1:
			if (event.GUIEvent.EventType == gui::EGET_LISTBOX_CHANGED)
			{
				selected = ((gui::IGUIListBox*)event.GUIEvent.Caller)->getSelected();
				startButton->setEnabled(selected != 0);
			}
			break;
		case 2:
			if (event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED )
			{
				device->closeDevice();
				start = true;
			}
		case 3:
			if (event.GUIEvent.EventType == gui::EGET_CHECKBOX_CHANGED )
				fullscreen = ((gui::IGUICheckBox*)event.GUIEvent.Caller)->isChecked();
			break;
		case 4:
			if (event.GUIEvent.EventType == gui::EGET_CHECKBOX_CHANGED )
				music = ((gui::IGUICheckBox*)event.GUIEvent.Caller)->isChecked();
			break;
		case 5:
			if (event.GUIEvent.EventType == gui::EGET_CHECKBOX_CHANGED )
				shadows = ((gui::IGUICheckBox*)event.GUIEvent.Caller)->isChecked();
			break;
		}
	}

	return false;
}
