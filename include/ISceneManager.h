// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_SCENE_MANAGER_H_INCLUDED__
#define __I_SCENE_MANAGER_H_INCLUDED__

#include "array.h"
#include "IUnknown.h"
#include "vector3d.h"
#include "dimension2d.h"
#include "SColor.h"
#include "SMaterial.h"

namespace irr
{

namespace io
{
	class IFileSystem;
}

namespace video
{
	class IVideoDriver;
}



namespace scene
{
	//! Enumeration with render times.
	/** A parameter passed to the registerNodeForRendering() method of the ISceneManager, 
	specifiing when the mode wants to be drawed in relation to the other nodes. */
	enum ESceneNodeRenderTime
	{
		//! scene nodes which are lights or camera should use this,
		//! the very first pass.
		SNRT_LIGHT_AND_CAMERA,	

		//! this is used for sky boxes.
		SNRT_SKY_BOX,

		//! Default render time, all normal objects should use this.
		//! The scene Manager will determine by itself if an object is 
		//! transparent, and do everything necessary by itself.
		SNRT_DEFAULT,			

		//! Drawn after the transparent nodes, the time for drawing shadow volumes						
		SNRT_SHADOW,			

		//! Never used, value specifing how much parameters there are
		SNRT_COUNT				
	};

	class IMesh;
	class IAnimatedMesh;
	class ISceneNode;
	class ICameraSceneNode;
	class IAnimatedMeshSceneNode;
	class IBspTreeSceneNode;
	class ISceneNodeAnimator;
	class ILightSceneNode;
	class IBillboardSceneNode;
	class IMeshLoader;

	//!	The Scene Manager manages scene nodes, mesh recources, cameras and all the other stuff.
	/** All Scene nodes can be created only here. There is a always growing list of scene 
	nodes for lots of purposes: Indoor rendering scene nodes like the Octree 
	(addOctTreeSceneNode()) or the binary space partition tree (addBspTreeSceneNode()),
	different Camera scene nodes (addCameraSceneNode(), addCameraSceneNodeMaya()),
	scene nodes for Light (addLightSceneNode()), Billboards (addBillboardSceneNode())
	and so on.
	A scene node is a node in the hirachical scene graph. Every scene node may have children,
	which are other scene nodes. Children move relative the their parents position. If the parent of a node is not
	visible, its children won't be visible too. In this way, it is for example easily possible
	to attach a light to a moving car, or to place a walking character on a moving platform
	on a moving ship.
	*/
	class ISceneManager : public IUnknown
	{
	public:

		//! destructor
		virtual ~ISceneManager() {};

		//! Gets pointer to an animateable mesh. Loads it if needed.
		//! Currently there are the following mesh formats supported:
		//! .obj(Alias Wavefront Maya), .ms3d(Milkshape3D), .bsp(Quake3 Level),
		//! and .md2(Quake2 Model). More formats coming soon, make a feature request on
		//! the Irrlicht Engine homepage if you like.
		//! \param filename: Filename of the mesh to load.
		//! \return Returns NULL if failed and the pointer to the mesh if 
		//! successful. 
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual IAnimatedMesh* getMesh(const c8* filename) = 0;

		//! Returns the video driver.
		//! \return Returns pointer to the video Driver.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual video::IVideoDriver* getVideoDriver() = 0;

		//! Adds a test scene node for test purposes of the scene. It is a simple cube of (1,1,1) size. 
		//! \param size: Size of the cube.
		//! \param parent: Parent of the scene node. Can be NULL if no parent.
		//! \param id: Id of the node. This id can be used to identify the scene node.
		//! \param position: Position of the space relative to its parent where the
		//! scene node will be placed.
		//! \param rotation: Initital rotation of the scene node.
		//! \param scale: Initial scale of the scene node.
		//! \return Returns pointer to the created test scene node.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ISceneNode* addTestSceneNode(f32 size=10.0f, ISceneNode* parent=0, s32 id=-1,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)) = 0;

		//! Adds a scene node for rendering an animated mesh model.
		//! \param mesh: Pointer to the loaded animated mesh to be displayed.
		//! \param parent: Parent of the scene node. Can be NULL if no parent.
		//! \param id: Id of the node. This id can be used to identify the scene node.
		//! \param position: Position of the space relative to its parent where the
		//! scene node will be placed.
		//! \param rotation: Initital rotation of the scene node.
		//! \param scale: Initial scale of the scene node.
		//! \return Returns pointer to the created scene node.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual IAnimatedMeshSceneNode* addAnimatedMeshSceneNode(IAnimatedMesh* mesh, ISceneNode* parent=0, s32 id=-1,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)) = 0;

		//! Adds a scene node for rendering a static mesh.
		//! \param mesh: Pointer to the loaded static mesh to be displayed.
		//! \param parent: Parent of the scene node. Can be NULL if no parent.
		//! \param id: Id of the node. This id can be used to identify the scene node.
		//! \param position: Position of the space relative to its parent where the
		//! scene node will be placed.
		//! \param rotation: Initital rotation of the scene node.
		//! \param scale: Initial scale of the scene node.
		//! \return Returns pointer to the created scene node.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ISceneNode* addMeshSceneNode(IMesh* mesh, ISceneNode* parent=0, s32 id=-1,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)) = 0;

		//! Adds a scene node for rendering using a binary space partition tree.
		//! This is a nice method for rendering indoor scenes. Based on a mesh given as parameter, the
		//! tree will be build. Note that The build process can be slow, if there are lots of polygons.
		//! Note that the implementation of the BspTree in the Irrlicht Engine is currently
		//! not complete a little bit buggy. You may want to use the OctTree instead, it already works.
		//! Use ISceneManager::addOctTreeSceneNode() for that.
		//! \param mesh: The mesh containing all geometry from which the binary space partition tree will be build.
		//! \param parent: Parent node of the bsp tree node.
		//! \param id: id of the node. This id can be used to identify the node.
		//! \return Returns the pointer to the IBspTreeSceneNode if successful, otherwise 0. 
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual IBspTreeSceneNode* addBspTreeSceneNode(IMesh* mesh, ISceneNode* parent=0, s32 id=-1) = 0;

		//! Adss a scene node for rendering using a octtree. This a good method for rendering 
		//! scenes with lots of geometry. The Octree is built on the fly from the mesh, much
		//! faster then a bsp tree.
		//! \param mesh: The mesh containing all geometry from which the octtree will be build.
		//! \param parent: Parent node of the octtree node.
		//! \param id: id of the node. This id can be used to identify the node.
		//! \return Returns the pointer to the octtree if successful, otherwise 0. 
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ISceneNode* addOctTreeSceneNode(IMesh* mesh, ISceneNode* parent=0, s32 id=-1) = 0;

		//! Adds a camera scene node to the scene and sets it as active camera.
		//! \param position: Position of the space relative to its parent where the camera will be placed.
		//! \param lookat: Position where the camera will look at. Also known as target.
		//! \param parent: Parent scene node of the camera. Can be null. If the parent moves,
		//! the camera will move too.
		//! \param id: id of the camera. This id can be used to identify the camera.
		//! \return Returns pointer to interface to camera if successful, otherwise 0.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ICameraSceneNode* addCameraSceneNode(ISceneNode* parent = 0,
			const core::vector3df& position = core::vector3df(0,0,0), 
			const core::vector3df& lookat = core::vector3df(0,0,100), s32 id=-1) = 0;

		//! Adds a camera scene node which is able to be controlled with the mouse similar
		//! like in the 3D Software Maya by Alias Wavefront.
		//! \param parent: Parent scene node of the camera. Can be null.
		//! \param rotateSpeed: Rotation speed of the camera.
		//! \param zoomSpeed: Zoom speed of the camera.
		//! \param tranlationSpeed: TranslationSpeed of the camera.
		//! \param id: id of the camera. This id can be used to identify the camera.
		//! \return Returns a pointer to the interface of the camera if successful, otherwise 0.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ICameraSceneNode* addCameraSceneNodeMaya(ISceneNode* parent = 0,
			f32 rotateSpeed = -1500.0f, f32 zoomSpeed = 200.0f, f32 translationSpeed = 1500.0f, s32 id=-1) = 0;

		//! Adds a camera scene node which is able to be controled with the mouse and keys
		//! like in most first person shooters (FPS): 
		//! Look with the mouse, move with cursor keys.
		//! \param parent: Parent scene node of the camera. Can be null.
		//! \param rotateSpeed: Speed with wich the camera is rotated. This can be done
		//! only with the mouse.
		//! \param movespeed: Speed with which the camera is moved. Movement is done with
		//! the cursor keys.
		//! \param id: id of the camera. This id can be used to identify the camera.
		//! \return Returns a pointer to the interface of the camera if successful, otherwise 0.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ICameraSceneNode* addCameraSceneNodeFPS(ISceneNode* parent = 0,
			f32 rotateSpeed = 100.0f, f32 moveSpeed = 500.0f, s32 id=-1) = 0;

		//! Adds a dynamic light scene node. The light will cast dynamic light on all
		//! other scene nodes in the scene, which have the material flag video::MTF_LIGHTING
		//! turned on. (This is the default setting in most scene nodes).
		//! \param parent: Parent scene node of the light. Can be null. If the parent moves,
		//! the light will move too.
		//! \param position: Position of the space relative to its parent where the light will be placed.
		//! \param color: Diffuse color of the light. Ambient or Specular colors can be set manually with
		//! the ILightSceneNode::getLightData() method.
		//! \param radius: Radius of the light.
		//! \param id: id of the node. This id can be used to identify the node.
		//! \return Returns pointer to the interface of the light if successful, otherwise NULL.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ILightSceneNode* addLightSceneNode(ISceneNode* parent = 0,
			const core::vector3df& position = core::vector3df(0,0,0),
			video::SColorf color = video::SColorf(1.0f, 1.0f, 1.0f),	f32 radius=100.0f, s32 id=-1) = 0;

		//! Adds a billboard scene node to the scene. A billboard is like a 3d sprite: A 2d element,
		//! which always looks to the camera. It is usually used for things like explosions, fire,
		//! lensflares and things like that.
		//! \param parent: Parent scene node of the billboard. Can be null. If the parent moves,
		//! the billboard will move too.
		//! \param position: Position of the space relative to its parent where the billboard will be placed.
		//! \param size: Size of the billboard. This size is 2 dimensional because a billboard only has
		//! width and height.
		//! \param id: An id of the node. This id can be used to identify the node.
		//! \return Returns pointer to the billboard if successful, otherwise NULL.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual IBillboardSceneNode* addBillboardSceneNode(ISceneNode* parent = 0,
			const core::dimension2d<f32>& size = core::dimension2d<f32>(10.0f, 10.0f),
			const core::vector3df& position = core::vector3df(0,0,0), s32 id=-1) = 0;

		//! Adds a skybox scene node. A skybox is a big cube with 6 textures on it and
		//! is drawed around the camera position. 
		//! \param top: Texture for the top plane of the box.
		//! \param bottom: Texture for the bottom plane of the box.
		//! \param left: Texture for the left plane of the box.
		//! \param right: Texture for the right plane of the box.
		//! \param front: Texture for the front plane of the box.
		//! \param parent: Parent scene node of the skybox. A skybox usually has no parent,
		//! so this should be null. Note: If a parent is set to the skybox, the box will not 
		//! change how it is drawed.
		//! \param id: An id of the node. This id can be used to identify the node.
		//! \return Returns a pointer to the sky box if successful, otherwise NULL.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ISceneNode* addSkyBoxSceneNode(video::ITexture* top, video::ITexture* bottom,
			video::ITexture* left, video::ITexture* right, video::ITexture* front,
			video::ITexture* back, ISceneNode* parent = 0, s32 id=-1) = 0;

		//! Adds a Hill Plane mesh to the mesh pool. The mesh is generated on the fly
		//! and looks like a plane with some hills on it. It is uses mostly for quick
		//! tests of the engine only. You can specify how many hills there should be 
		//! on the plane and how high they should be. Also you must specify a name for
		//! the mesh, because the mesh is added to the mesh pool, and can be retieved
		//! again using ISceneManager::getMesh() with the name as parameter.
		//! \param filename: The name of this mesh which must be specified in order
		//! to be able to retrieve the mesh later with ISceneManager::getMesh().
		//! \param tileSize: Size of a tile of the mesh. (10.0f, 10.0f) would be a
		//! good value to start, for example.
		//! \param tileCount: Specifies how much tiles there will be. If you specifiy 
		//! for example that a tile has the size (10.0f, 10.0f) and the tileCount is
		//! (10,10), than you get a field of 100 tiles wich has the dimension 100.0fx100.0f.
		//! \param material: Material of the hill mesh.
		//! \param hillHeight: Height of the hills. If you specify a negative value
		//! you will get holes instead of hills. If the height is 0, no hills will be
		//! created.
		//! \param countHills: Amount of hills on the plane. There will be countHills.X
		//! hills along the X axis and countHills.Y along the Y axis. So in total there 
		//! will be countHills.X * countHills.Y hills.
        //! \return Returns null if the creation failed. The reason could be that you 
		//! specified some invalid parameters or that a mesh with that name already
		//! exists. If successful, a pointer to the mesh is returned. 
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual IAnimatedMesh* addHillPlaneMesh(const c8* name,
			const core::dimension2d<f32>& tileSize, const core::dimension2d<s32>& tileCount,
			video::SMaterial* material = 0,	f32 hillHeight = 0.0f, 
			core::dimension2d<f32> countHills = core::dimension2d<f32>(0.0f, 0.0f)) = 0;

		//! Returns the root scene node. This is the scene node wich is parent 
		//! of all scene nodes. The root scene node is a special scene node which
		//! only exists to manage all scene nodes. It is not rendered and cannot
		//! be removed from the scene.
		//! \return Returns a pointer to the root scene node.
		virtual ISceneNode* getRootSceneNode() = 0;

		//! Returns the current active camera.
		//! \return The active camera is returned. Note that this can be NULL, if there
		//! was no camera created yet.
		virtual ICameraSceneNode* getActiveCamera() = 0;

		//! Sets the active camera. The previous active camera will be deactivated.
		//! \param camera: The new camera which should be active.
		virtual void setActiveCamera(ICameraSceneNode* camera) = 0;

		//! Registers a node for rendering it at a specific time.
		//! This method should only be used by SceneNodes when they get a 
		//! ISceneNode::OnPreRender() call.
		virtual void registerNodeForRendering(ISceneNode* node, ESceneNodeRenderTime time = SNRT_DEFAULT) = 0;

		//! Draws all scene nodes.
		virtual void drawAll() = 0;

		//! Creates a rotation animator, which rotates the attached scene node around itself.
		//! \param rotationPerSecond: Specifies the speed of the animation
		//! \return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
		//! and the animator will animate it.
		//! If you no longer need the animator, you should call ISceneNodeAnimator::drop().
		//! See IUnknown::drop() for more information.
		virtual ISceneNodeAnimator* createRotationAnimator(const core::vector3df& rotationPerSecond) = 0;

		//! Creates a fly circle animator, which lets the attached scene node fly 
		//! around a center. The center is the position of the scene node.
		//! \param rotationPerSecond: Specifies the speed of the animation
		//! \return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
		//! and the animator will animate it.
		//! If you no longer need the animator, you should call ISceneNodeAnimator::drop().
		//! See IUnknown::drop() for more information.
		virtual ISceneNodeAnimator* createFlyCircleAnimator(const core::vector3df& normal, 
			f32 radius, f32 speed=0.001f) = 0;

		//! Creates a fly straight animator, which lets the attached scene node
		//! fly or move along a line between two points.
		//! \param startPoint: Start point of the line.
		//! \param endPoint: End point of the line.
		//! \param timeForWay: Time in milli seconds how long the node should need to
		//! move from the start point to the end point.
		//! \param loop: If set to false, the node stops when the end point is reached.
		//! If loop is true, the node begins again at the start.
		//! \return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
		//! and the animator will animate it.
		//! If you no longer need the animator, you should call ISceneNodeAnimator::drop().
		//! See IUnknown::drop() for more information.
		virtual ISceneNodeAnimator* createFlyStraightAnimator(const core::vector3df& startPoint,
			const core::vector3df& endPoint, u32 timeForWay, bool loop=false) = 0;

		//! Creates a texture animator, which switches the textures of the target scene
		//! node based on a list of textures.
		//! \param textures: List of textures to use.
		//! \param timePerFrame: Time in milliseconds, how long any texture in the list
		//! should be visible.
		//! \param loop: If set to to false, the last texture remains set, and the animation
		//! stops. If set to true, the animation restarts with the first texture.
		//! \return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
		//! and the animator will animate it.
		//! If you no longer need the animator, you should call ISceneNodeAnimator::drop().
		//! See IUnknown::drop() for more information.
		virtual ISceneNodeAnimator* createTextureAnimator(const core::array<video::ITexture*>& textures,
			s32 timePerFrame, bool loop=true) = 0;

		//! Adds an external mesh loader. If you want the engine to be extended with
		//! file formats it currently is not able to load (e.g. .cob), just implement
		//! the IMeshLoader interface in your loading class and add it with this method.
		//! \param externalLoader: Implementation of a new mesh loader.
		virtual void addExternalMeshLoader(IMeshLoader* externalLoader) = 0;
	};


} // end namespace scene
} // end namespace irr

#endif

