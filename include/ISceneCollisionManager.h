// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_SCENE_COLLISION_MANAGER_H_INCLUDED__
#define __I_SCENE_COLLISION_MANAGER_H_INCLUDED__

#include "IUnknown.h"
#include "vector3d.h"
#include "position2d.h"
#include "line3d.h"

namespace irr
{


namespace scene
{
	class ISceneNode;
	class ICameraSceneNode;

	//!	The Scene Collision Manager provides methods for performing collision tests and picking on scene nodes.
	class ISceneCollisionManager : public IUnknown
	{
	public:

		//! destructor
		virtual ~ISceneCollisionManager() {};

		//! Returns the scene node, which is currently visible under the overgiven 
		//! screencoordinates, viewed from the currently active camera. The collision
		//! tests are done using a bounding box for each scene node.
		//! \param pos: Position in pixel screen coordinates, under which the returned
		//! scene node will be.
		//! \param idBitMask: Only scene nodes with an id with bits set like in this mask
		//! will be tested. If the BitMask is 0, this feature is disabled.
		//! \return Returns the visible scene node under screen coordinates with matching
		//! bits in its id. If there is no scene node under this position, 0 is returned.
		virtual ISceneNode* getSceneNodeFromSceenCoordinates(core::position2d<s32> pos,
			s32 idBitMask=0) = 0;

		//! Returns the nearest scene node which collides with a 3d ray and 
		//! which id matches a bitmask. The collision tests are done using a bounding
		//! box for each scene node.
		//! \param ray: Line witch witch collisions are tested.
		//! \param idBitMask: Only scene nodes with an id with bits set like in this mask
		//! will be tested. If the BitMask is 0, this feature is disabled.
		//! \return Returns the scene node nearest to ray.start, which collides with the
		//! ray and matches the idBitMask, if the mask is not null. If no scene
		//! node is found, 0 is returned.
		virtual ISceneNode* getSceneNodeFromRay(core::line3d<f32> ray, s32 idBitMask=0) = 0;

		//! Returns the scene node, at which the overgiven camera is looking at and
		//! which id matches the bitmask. A ray is simply casted from the position
		//! of the camera to the view target position, and all scene nodes are tested
		//! against this ray. The collision tests are done using a bounding
		//! box for each scene node.
		//! \param camera: Camera from which the ray is casted.
		//! \param idBitMaks: Only scene nodes with an id with bits set like in this mask
		//! will be tested. If the BitMask is 0, this feature is disabled.
		//! \return Returns the scene node nearest to the camera, which collides with the
		//! ray and matches the idBitMask, if the mask is not null. If no scene
		//! node is found, 0 is returned.
		virtual ISceneNode* getSceneNodeFromCamera(ICameraSceneNode* camera, s32 idBitMask=0) = 0;
	};


} // end namespace scene
} // end namespace irr

#endif

