// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_ANIMATED_MESH_SCENE_NODE_H_INCLUDED__
#define __I_ANIMATED_MESH_SCENE_NODE_H_INCLUDED__

#include "ISceneNode.h"
#include "IShadowVolumeSceneNode.h"

namespace irr
{
namespace scene
{

	//! Scene node capable of displaying an animated mesh and its shadow.
	/** The shadow is optional: If a shadow should be displayed too, just invoke
	the IAnimatedMeshSceneNode::createShadowVolumeSceneNode().*/
	class IAnimatedMeshSceneNode : public ISceneNode
	{
	public:

		//! Constructor
		IAnimatedMeshSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f))
			: ISceneNode(parent, mgr, id, position, rotation, scale) {}

		//! Destructor
		virtual ~IAnimatedMeshSceneNode() {};

		//! Sets the current frame number. 
		//! From now on the animation is played from this frame. 
		//! \param frame: Number of the frame to let the animation be started from.
		//! The frame number must be a valid frame number of the IMesh used by this 
		//! scene node. Set IAnimatedMesh::getMesh() for details.
		virtual void setCurrentFrame(s32 frame) = 0;

		//! Sets the frame numbers between the animation is looped.
		//! The default is 0 - MaximalFrameCount of the mesh.
		//! \param begin: Start frame number of the loop.
		//! \param end: End frame number of the loop.
		//! \return Returns true if successful, false if not.
		virtual bool setFrameLoop(s32 begin, s32 end) = 0;

		//! Sets the speed with witch the animation is played.
		//! \param framesPerSecond: Frames per second played.
		virtual void setAnimationSpeed(s32 framesPerSecond) = 0;

		//! Creates shadow volume scene node as child of this node
		//! and returns a pointer to it. 
		//! \param id: Id of the shadow scene node. This id can be used to identify
		//! the node later.
		//! \return Returns pointer to the created shadow scene node.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual IShadowVolumeSceneNode* addShadowVolumeSceneNode(s32 id=-1) = 0;

		//! Adds a child scene node which is animated by a part of the animated
		//! mesh. In this way, it is for example possible to attach a weapon the
		//! the left hand of an animated model.
		//! If the attached child scene node has already a parent
		//! (which is should have), it is automaticly removed from the old
		//! parent and reattached to this one. You will not have to worry about
		//! that.
		//! \param name: Name of the mesh part with which the attached scene node
		//! will be animated. For example if you loaded a .ms3d file
		//! and want the child scene node to be animated with the left hand, like it
		//! would be holded by it, the name should be then "lefthand", if the joint of
		//! the left hand in the ms3d file is called "lefthand". 
		//! \param child: Pointer to child scene node.
		//! \return Returns true if the child was sucessfully attached to the
		//! right part of the mesh. False is returned if a part of the mesh with
		//! the specified name could not be found or the mesh does not support
		//! attaching objects to parts of the mesh.
		virtual bool addChild(const c8* name, ISceneNode* child) = 0;
	};

} // end namespace scene
} // end namespace irr

#endif

