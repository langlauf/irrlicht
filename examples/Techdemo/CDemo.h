// This is the current Techdemo of the Irrlicht Engine (c) 2003 by N.Gebhardt.
// This file is not documentated.

#ifndef __C_DEMO_H_INCLUDED__
#define __C_DEMO_H_INCLUDED__

#define USE_AUDIERE

#include <windows.h>
#include <irrlicht.h>

#ifdef USE_AUDIERE
#include <audiere.h>
using namespace audiere;
#pragma comment (lib, "audiere.lib")
#endif

using namespace irr;
const int CAMERA_COUNT = 7;

class CDemo : public IEventReceiver
{
public:

	CDemo(bool fullscreen, bool music, bool shadows, video::EDriverType driver);

	void run();

	virtual bool OnEvent(SEvent event);

private:

	void doSceneSetup();
	void createLoadingScreen();
	void loadSceneData();
	void switchToNextScene();

	bool fullscreen;
	bool music;
	bool shadows;
	bool scenePrepared;
	video::EDriverType driverType;
	IrrlichtDevice *device;

#ifdef USE_AUDIERE
	void startAudiere();
	AudioDevicePtr audiereDevice;
	OutputStreamPtr stream;
#endif

	int currentScene;
	video::SColor backColor;
	gui::IGUIElement* statusText;
	scene::IAnimatedMesh* quakeLevelMesh;
	scene::ISceneNode* quakeLevelNode;
	scene::ISceneNode* skyboxNode;
	scene::IAnimatedMeshSceneNode* model1;
	scene::IAnimatedMeshSceneNode* model2;

	u32 time;
	u32 timeForThisScene;
};

#endif