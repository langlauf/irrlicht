/* Irrlicht.h -- interface of the 'Irrlicht Engine' version 0.1

  Copyright (C) 2002-2003 Nikolaus Gebhardt

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef __IRRLICHT_H_INCLUDED__
#define __IRRLICHT_H_INCLUDED__

#include "aabbox3d.h"
#include "array.h"
#include "SColor.h"
#include "SLight.h"
#include "dimension2d.h"
#include "EDriverTypes.h"
#include "heapsort.h"
#include "IAnimatedMesh.h"
#include "IAnimatedMeshMD2.h"
#include "IAnimatedMeshMS3D.h"
#include "IAnimatedMeshSceneNode.h"
#include "IBspTreeSceneNode.h"
#include "ICameraSceneNode.h"
#include "IDummyTransformationSceneNode.h"
#include "IEventReceiver.h"
#include "IFileList.h"
#include "IFileSystem.h"
#include "IGUIButton.h"
#include "IGUICheckBox.h"
#include "IGUIElement.h"
#include "IGUIEnvironment.h"
#include "IGUIFileOpenDialog.h"
#include "IGUIFont.h"
#include "IGUIImage.h"
#include "IGUIInOutFader.h"
#include "IGUIListBox.h"
#include "IGUIMeshViewer.h"
#include "IGUIScrollBar.h"
#include "IGUISkin.h"
#include "IGUIStaticText.h"
#include "IGUIWindow.h"
#include "IMesh.h"
#include "IMeshBuffer.h"
#include "IMeshManipulator.h"
#include "IMetaTriangleSelector.h"
#include "IReadFile.h"
#include "IrrlichtDevice.h"
#include "irrMath.h"
#include "irrString.h"
#include "ISceneManager.h"
#include "ISceneNode.h"
#include "ITriangleSelector.h"
#include "ILightSceneNode.h"
#include "ISceneNodeAnimator.h"
#include "ISceneCollisionManager.h"
#include "ISceneNodeAnimatorCollisionResponse.h"
#include "IParticleSystemSceneNode.h"
#include "IParticleEmitter.h"
#include "IParticleAffector.h"
#include "IBillboardSceneNode.h"
#include "ITexture.h"
#include "IUnknown.h"
#include "IVideoDriver.h"
#include "IVideoModeList.h"
#include "Keycodes.h"
#include "line2d.h"
#include "line3d.h"
#include "irrList.h"
#include "matrix4.h"
#include "plane3d.h"
#include "plane3d.h"
#include "vector2d.h"
#include "vector3d.h"
#include "triangle3d.h"
#include "position2d.h"
#include "rect.h"
#include "S3DVertex.h"
#include "SMaterial.h"
#include "SMesh.h"
#include "SMeshBuffer.h"
#include "SMeshBufferLightMap.h"
#include "irrTypes.h"

/*! \mainpage Irrlicht Engine API documentation
 *
 * \section intro Introduction
 *
 * Welcome to the Irrlicht Engine API documentation.
 * Here you'll find any information you'll need to develop applications with
 * the Irrlicht Engine. If you look for a tutorial on how to start, take a look
 * at the homepage of the Irrlicht Engine at 
 * <A HREF="http://irrlicht.sourceforge.net" >irrlicht.sourceforge.net</A> 
 * or into the SDK in the directory \examples.
 *
 * The Irrlicht Engine is intended to be an easy-to-use 3d engine, so
 * this documentation is an important part of it. If you have any questions or
 * suggestions, just send a email to the author of the engine, Nikolaus Gebhardt
 * (niko (at) code3d.com).
 *
 * \section links Links
 *
 * <A HREF="namespaces.html">Namespaces</A>: A very good place to start reading
 * the documentation.<BR>
 * <A HREF="annotated.html">Compound list</A>: List of all classes with descriptions.<BR>
 * <A HREF="hierarchy.html">Class hierarchy</A>: Class hierarchy list.<BR>
 * <A HREF="classes.html">Alphabetical class list</A>: Good place to find forgotten class names.<BR>
 *
 * \section example Short example
 *
 * A simple application, starting up the engine, loading a Quake2 animated 
 * model file and the corresponding texture, animating and displaying it
 * in front of a blue background and placing a 3d camera would look like
 * the following code. I think this example shows the usage of the engine quite good:
 *
 * \code
 * #include <Irrlicht.h>
 * using namespace irr;
 *
 * int main()
 * {
 *	IrrlichtDevice *device = createDevice(video::DT_DIRECTX8,
 *		core::dimension2d<s32>(640,480), false);
 *
 *	video::IVideoDriver* driver = device->getVideoDriver();
 *	scene::ISceneManager* scenemgr = device->getSceneManager();
 *
 *	device->setWindowCaption(L"Hello World!");
 *
 *	scene::ISceneNode* node = scenemgr->addAnimatedMeshSceneNode(
 *		scenemgr->getMesh("quake2model.md2"));
 *	node->setMaterialTexture(0, driver->getTexture("texture.bmp"));
 *	node->setMaterialFlag(video::EMF_LIGHTING, false);
 *
 *	scenemgr->addCameraSceneNode(0, core::vector3df(0,0,-40));
 * 
 *	while(device->run() && driver)
 *	{
 *		driver->beginScene(true, true, video::SColor(255,0,0,255));
 *		scenemgr->drawAll();
 *		driver->endScene();
 *	}
 *
 *	device->drop();
 *	return 0;
 * }
 * \endcode
 *
 * If you would like to replace the simple blue screen by a cool Quake 3 Map,
 * optimized by an octtree, just insert this code somewhere
 * before the while loop:
 *
 * \code
 * device->getFileSystem()->addZipFileArchive("quake3map.pk3");
 * scene::IAnimatedMesh* mesh = scenemgr->getMesh("quake3map.bsp");
 * node = scenemgr->addOctTreeSceneNode(mesh->getMesh(0));
 * \endcode
 *
 * As you can see, the engine uses namespaces. Everything in the engine is
 * placed into the namespace 'irr', but there are also 5 sub namespaces.
 * You can find a list of all namespaces with descriptions at the 
 * <A HREF="namespaces.html"> namespaces page</A>. 
 * This is also a good place to start reading the documentation. If you
 * don't want always write the namespace names, just use all namespaces like 
 * this:
 * \code
 * using namespace core;
 * using namespace scene;
 * using namespace video;
 * using namespace io;
 * using namespace gui;
 * \endcode
 *
 * There is a lot more the engine can do, but I hope this gave a short
 * overview over the basic features of the engine.
 */

#include <wchar.h>
#ifdef _WIN32
//! Define for swprintf because this method does not match the ISO C standard
//! on Win32 platforms, but it does on all other ones.
#define   swprintf   _snwprintf
#endif // WIN32

#ifdef WIN32
#ifdef IRRLICHT_EXPORTS
#define IRRLICHT_API __declspec(dllexport)
#else
#define IRRLICHT_API __declspec(dllimport)
#endif // IRRLICHT_EXPORT
#else
#define IRRLICHT_API 
#endif // WIN32

//! Everything in the Irrlicht Engine can be found in this namespace.
namespace irr
{
	//! Creates an Irrlicht device. The Irrlicht device is the root object for using the engine.
	/** \param deviceType: Type of the device. This can currently be video::DT_NULL, 
	video::DT_SOFTWARE,	video::DT_DIRECTX8 and video::DT_OPENGL.
	\param windowSize: Size of the window or the video mode in fullscreen mode.
	\param bits: Bits per pixel in fullscreen mode. Ignored if windowed mode.
	\param fullscreen: Should be set to true if the device should run in fullscreen. Otherwise
		the device runs in window mode.
	\param stencilbuffer: Specifies if the stencil buffer should be enabled. Set this to true,
	    if you want the engine be able to draw stencil buffer shadows. Note that not all
		devices are able to use the stencil buffer. If they don't no shadows will be drawn.
	\param receiver: A user created event receiver.
	\return Returns pointer to the created IrrlichtDevice or null if the 
	device could not be created.
	*/
	IRRLICHT_API IrrlichtDevice* createDevice(
		video::EDriverType deviceType = video::DT_SOFTWARE, 
		const core::dimension2d<s32>& windowSize = core::dimension2d<s32>(640,480),
		u32 bits = 16,
		bool fullscreen = false,
		bool stencilbuffer=false,
		IEventReceiver* receiver = 0);

	// THE FOLLOWING IS AN EMPTY LIST OF ALL SUB NAMESPACES
	// EXISTING ONLY FOR THE DOCUMENTION SOFTWARE DOXYGEN.

	//! In this namespace can be found basic classes like vectors, planes, arrays, lists and so on.
	namespace core
	{
	}

	//! The gui namespace contains useful classes for easy creation of a graphical user interface.
	namespace gui
	{
	}

	//! This namespace provides interfaces for input/output: Reading and writing files, accessing zip archives, and so on.
	namespace io
	{
	}

	//! All scene management can be found in this namespace: Mesh loading, special scene nodes like octrees and billboards, and so on.
	namespace scene
	{
	}

	//! The video namespace contains classes for accessing the video driver. All 2d and 3d rendering is done here.
	namespace video
	{
	}
}


#endif

