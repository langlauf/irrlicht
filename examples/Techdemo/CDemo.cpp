// This is the current Techdemo of the Irrlicht Engine (c) 2003 by N.Gebhardt.
// This file is not documentated.

#include "CDemo.h"
#include <stdio.h>

CDemo::CDemo(bool f, bool m, bool s, video::EDriverType d)
: fullscreen(f), driverType(d), currentScene(0),
 model1(0), model2(0), scenePrepared(false), music(m), 
shadows(s), quakeLevelMesh(0), quakeLevelNode(0), timeForThisScene(0),
skyboxNode(0)
{
}


void CDemo::run()
{
	device = createDevice(driverType,
		core::dimension2d<s32>(640, 480), 16, fullscreen, shadows, this);

	device->getFileSystem()->addZipFileArchive("irrlicht.dat");
	device->getFileSystem()->addZipFileArchive("..\\..\\media\\irrlicht.dat");
	device->getFileSystem()->addZipFileArchive("map-20kdm2.pk3");
	device->getFileSystem()->addZipFileArchive("..\\..\\media\\map-20kdm2.pk3");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	device->setWindowCaption(L"Irrlicht Engine Techdemo");

	while(device->run() && driver)
	{
		if (device->isWindowActive())
		{
			doSceneSetup();

			driver->beginScene(true, true, backColor);

			smgr->drawAll();
			guienv->drawAll();

			driver->endScene();

			wchar_t tmp[255];
			swprintf(tmp, 255, L"%s fps:%d polys:%d", 
					driver->getName(), 
					driver->getFPS(),
					driver->getPrimitiveCountDrawed());

			statusText->setText(tmp);
		}
	}

	device->drop();
}


void CDemo::doSceneSetup()
{
	if (currentScene>(CAMERA_COUNT*2)+1)
		currentScene = 2;

	switch(currentScene)
	{
	case 0:
		createLoadingScreen();
		break;
	case 1:
		loadSceneData();
		// FALLTHROUGH
	default:
		{
			if (!scenePrepared)
			{
				time = device->getTimer()->getTime();
				scenePrepared = true;
				switchToNextScene();
			}

			u32 now = device->getTimer()->getTime();

			if (now - time > timeForThisScene)
			{
				++currentScene;
				scenePrepared = false;
			}
		}
		break;
	}
}


bool CDemo::OnEvent(SEvent event)
{
	if (event.EventType == EET_KEY_INPUT_EVENT)
	{
		if (event.KeyInput.Key == KEY_ESCAPE &&
			event.KeyInput.PressedDown == false)
			device->closeDevice();
	}

	return false;
}


void CDemo::switchToNextScene()
{
	scene::ISceneManager* sm = device->getSceneManager();
	scene::ISceneNodeAnimator* sa = 0;
	scene::ICameraSceneNode* camera = 0;

	camera = sm->getActiveCamera();
	if (camera)
	{
		sm->setActiveCamera(0);
		camera->remove();
	}

	if (currentScene>8)
	{
		quakeLevelNode->setMaterialFlag(video::EMF_LIGHTING, true);
		quakeLevelNode->setMaterialType(video::EMT_SOLID);
		skyboxNode->setVisible(false);
	}
	else
	{
		quakeLevelNode->setMaterialFlag(video::EMF_LIGHTING, false);
		quakeLevelNode->setMaterialType(video::EMT_LIGHTMAP);
		skyboxNode->setVisible(true);
	}

	switch(currentScene)
	{
	case 11:
	case 4: // fly around shpere mapped thing camera
		camera = sm->addCameraSceneNode(0, core::vector3df(100,40,-80),core::vector3df(100,30,-80));
		sa = sm->createFlyCircleAnimator(core::vector3df(0,1,0), 30, 0.0005f);
		camera->addAnimator(sa);
		timeForThisScene = 9900;
		sa->drop();
		model2->setVisible(false);
		model1->setVisible(true);
		break;
	case 10:
	case 3:	// down fly anim camera
		camera = sm->addCameraSceneNode(0, core::vector3df(100,40,-80), core::vector3df(844,670,-885));
		sa = sm->createFlyStraightAnimator(	core::vector3df(94, 1002, 127),
			core::vector3df(108, 15, -60), 10000, true);
		camera->addAnimator(sa);
		timeForThisScene = 9900;
		model1->setVisible(true);
		model2->setVisible(false);		
		sa->drop();
		break;
	case 9:
	case 2: // panorama camera 1
		camera = sm->addCameraSceneNode(0, core::vector3df(0,0,0), core::vector3df(-586,708,52));
		sa = sm->createFlyStraightAnimator(core::vector3df(509,600,812),
			core::vector3df(844,670,-885), 10000, true);
		camera->addAnimator(sa);
		timeForThisScene = 9900;
		sa->drop();
		model1->setVisible(false);
		model2->setVisible(false);
		break;
	case 12:
	case 5: // portal sky watcher
		camera = sm->addCameraSceneNode(0, core::vector3df(0,0,0), core::vector3df(104,351,471));
		sa = sm->createFlyStraightAnimator(core::vector3df(806,407,442),
			core::vector3df(-477,407,466), 10000, true);
		camera->addAnimator(sa);
		timeForThisScene = 9900;
		sa->drop();
		model1->setVisible(false);
		model2->setVisible(true);
		break;
	case 13:
	case 6: // circle at floor before sphere mapped thing (dyn light on)
		camera = sm->addCameraSceneNode(0, core::vector3df(100,0,80), core::vector3df(104,351,-471));
		sa = sm->createFlyCircleAnimator(core::vector3df(0,1,0), 100, 0.0005f);
		camera->addAnimator(sa);
		timeForThisScene = 9900;
		sa->drop();
		model1->setVisible(true);
		model2->setVisible(false);
		break;
	case 14:
	case 7: // circle at sky above running model
		camera = sm->addCameraSceneNode(0, core::vector3df(51,380,476),core::vector3df(51,360,476));
		sa = sm->createFlyCircleAnimator(core::vector3df(0,1,0), 100, -0.0005f);
		camera->addAnimator(sa);
		timeForThisScene = 9900;
		sa->drop();
		model1->setVisible(false);
		model2->setVisible(true);
		break;
	case 15:
	case 8: // circle around tower with light
		camera = sm->addCameraSceneNode(0, core::vector3df(109,620,-622), core::vector3df(109,320,-622));
		sa = sm->createFlyCircleAnimator(core::vector3df(0,1,0), 300, 0.0003f);
		camera->addAnimator(sa);
		timeForThisScene = 9900;
		sa->drop();
		model1->setVisible(false);
		model2->setVisible(false);
		break;
	}
}


void CDemo::loadSceneData()
{
	// load quake levels

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* sm = device->getSceneManager();

	quakeLevelMesh = sm->getMesh("20kdm2.bsp");
	
	if (quakeLevelMesh)
	{
		quakeLevelNode = sm->addOctTreeSceneNode(quakeLevelMesh->getMesh(0));
		if (quakeLevelNode)
		{
			quakeLevelNode->setRelativePosition(core::vector3df(-1300,-70,-1249));
			quakeLevelNode->setVisible(true);
		}
	}

	// load sydney model and create 2 instances

	scene::IAnimatedMesh* mesh = 0;
	mesh = sm->getMesh("..\\..\\media\\sydney.md2");
	if (mesh)
	{
		model1 = sm->addAnimatedMeshSceneNode(mesh);
		if (model1)
		{
			model1->setMaterialTexture(0, driver->getTexture("..\\..\\media\\spheremap.jpg"));
			model1->setRelativePosition(core::vector3df(100,15,-80));
			model1->setFrameLoop(0, 310);
			model1->setAnimationSpeed(60);
			model1->setMaterialFlag(video::EMF_LIGHTING, true);
			model1->setMaterialType(video::EMT_SPHERE_MAP);
			model1->addShadowVolumeSceneNode();
		}

		model2 = sm->addAnimatedMeshSceneNode(mesh);
		if (model2)
		{
			model2->setRelativePosition(core::vector3df(180,15,-60));
			model2->setFrameLoop(313, 359);
			model2->setAnimationSpeed(30);
			model2->setMaterialTexture(0, device->getVideoDriver()->getTexture("..\\..\\media\\sydney.BMP"));
			model2->setMaterialFlag(video::EMF_LIGHTING, true);
			model2->setMaterialFlag(video::EMF_BILINEAR_FILTER, true);
			model2->addShadowVolumeSceneNode();
		}
	}

	scene::ISceneNodeAnimator* anim = 0;

	// create sky box

	skyboxNode = sm->addSkyBoxSceneNode(
		driver->getTexture("..\\..\\media\\irrlicht2_up.bmp"),
		driver->getTexture("..\\..\\media\\irrlicht2_dn.bmp"),
		driver->getTexture("..\\..\\media\\irrlicht2_lf.bmp"),
		driver->getTexture("..\\..\\media\\irrlicht2_rt.bmp"),
		driver->getTexture("..\\..\\media\\irrlicht2_ft.bmp"),
		driver->getTexture("..\\..\\media\\irrlicht2_bk.bmp"));

	// create walk-between-portals animation

	core::vector3df waypoint[2];
	waypoint[0].set(-66,365,469);
	waypoint[1].set(200,365,469);

	if (model2)
	{
		anim = device->getSceneManager()->createFlyStraightAnimator(waypoint[0], 
			waypoint[1], 10000, true);
		model2->addAnimator(anim);
		anim->drop();
	}

	// create animation for portals;

	core::array<video::ITexture*> textures;
	for (s32 g=1; g<8; ++g)
	{
		char tmp[64];
		sprintf(tmp, "..\\..\\media\\portal%d.bmp", g);
		video::ITexture* t = driver->getTexture(tmp);
		textures.push_back(t);
	}

	anim = sm->createTextureAnimator(textures, 100);

	// create portals

	scene::IBillboardSceneNode* bill = 0;

	for (int r=0; r<2; ++r)
	{
		bill = sm->addBillboardSceneNode(0, core::dimension2d<f32>(50,50), waypoint[r]);
		bill->setMaterialFlag(video::EMF_LIGHTING, false);
		bill->setMaterialTexture(0, driver->getTexture("..\\..\\media\\portal1.bmp"));
		bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		bill->addAnimator(anim);
	}

	anim->drop();

	// create dynamic lights

	scene::ILightSceneNode* light = 0;

	core::vector3df lights[3];
	lights[0].set(50,400,469);
	lights[1].set(100,30,-20);
	lights[2].set(109,320,-622);

	for (s32 l=0; l<3; ++l)
	{
		light = device->getSceneManager()->addLightSceneNode(0,
			lights[l],
			video::SColorf(1.0f, 0.5f, 0.0f, 1.0f), l == 2 ? 400.0f : 200.0f);

		if (l == 2)
			light->getLightData().CastShadows = false;

		anim = device->getSceneManager()->createFlyCircleAnimator(core::vector3df(0.0f, 1.0f, 0.0f),
			l == 2 ? 200.0f : 50.0f, l == 2 ? 0.001f : 0.0005f);
		light->addAnimator(anim);
		anim->drop();

		bill = device->getSceneManager()->addBillboardSceneNode(
			light, core::dimension2d<f32>(20,20));
		bill->setMaterialFlag(video::EMF_LIGHTING, false);
		bill->setMaterialTexture(0, driver->getTexture("..\\..\\media\\particlewhite.bmp"));
		bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	}

	// load music

	#ifdef USE_AUDIERE
	if (music)
		startAudiere();
	#endif

	// set background color

	backColor.set(0,0,0,0);

	++currentScene;
	scenePrepared = false;
}



void CDemo::createLoadingScreen()
{
	core::dimension2d<int> size = device->getVideoDriver()->getScreenSize();

	device->getCursorControl()->setVisible(false);

	// setup loading screen

	backColor.set(255,90,90,156);
	gui::IGUIImage* img = device->getGUIEnvironment()->addImage(
		core::rect<int>(10,10,98,41));

	img->setImage(
		device->getVideoDriver()->getTexture("..\\..\\media\\irrlichtlogoaligned.jpg"));

	const int lwidth = 170;
	const int lheight = 20;

	core::rect<int> pos(10, size.Height-lheight-10, 10+lwidth, size.Height-10);
	
	device->getGUIEnvironment()->addImage(pos);
	statusText = device->getGUIEnvironment()->addStaticText(L"Loading...",	true, pos);

	// load bigger font

	device->getGUIEnvironment()->getSkin()->setFont(
		device->getGUIEnvironment()->getFont("..\\..\\media\\fonthaettenschweiler.bmp"));

	// set new font color

	device->getGUIEnvironment()->getSkin()->setColor(gui::EGDC_BUTTON_TEXT,
		video::SColor(255,100,100,100));

	++currentScene;
}



#ifdef USE_AUDIERE
void CDemo::startAudiere()
{
	audiereDevice = OpenDevice();
	if (!audiereDevice) 
		return;

	stream = OpenSound(audiereDevice.get(), "IrrlichtTheme.mp3", true);
	if (!stream) 
	{
		stream = OpenSound(audiereDevice.get(), "..\\..\\media\\IrrlichtTheme.mp3", true);
		if (!stream)
			return;
	}
		

	stream->setRepeat(true);
	//stream->setVolume(0.5f); // 50% volume
	stream->play();
}
#endif

