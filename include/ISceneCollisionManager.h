// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_SCENE_COLLISION_MANAGER_H_INCLUDED__
#define __I_SCENE_COLLISION_MANAGER_H_INCLUDED__

#include "IUnknown.h"
#include "vector3d.h"
#include "triangle3d.h"
#include "position2d.h"
#include "line3d.h"

namespace irr
{


namespace scene
{
	class ISceneNode;
	class ICameraSceneNode;
	class ITriangleSelector;

	//!	The Scene Collision Manager provides methods for performing collision tests and picking on scene nodes.
	class ISceneCollisionManager : public IUnknown
	{
	public:

		//! destructor
		virtual ~ISceneCollisionManager() {};

		//! Finds the collision point of a line and lots of triangles, if there is one.
		//! \param ray: Line with witch collisions are tested.
		//! \param selector: TriangleSelector containing the triangles. It can
		//! be created for example using ISceneManager::createTriangleSelector() or 
		//! ISceneManager::createTriangleOctTreeSelector().
		//! \param outCollisionPoint: If a collision is detected, this will contain the
		//! position of the nearest collision.
		//! \param outTriangle: If a collision is detected, this will contain the triangle
		//! with which the ray collided.
		//! \return Returns true if a collision was detected and false if not.
		virtual bool getCollisionPoint(const core::line3d<f32>& ray,
			ITriangleSelector* selector, core::vector3df& outCollisionPoint,
			core::triangle3df& outTriangle) = 0;

		//! Collides a moving ellipsoid with a 3d world with gravity and returns
		//! the resulting new position of the ellipsoid. This can be used for moving
		//! a character in a 3d world: The character will slide at walls and is able
		//! to walk up stairs. The method used how to calculate the collision result
		//! position is based on the paper "Improved Collision detection and Response"
		//! by Kasper Fauerby.
		//! \param selector: TriangleSelector containing the triangles of the world.
		//! It can be created for example using ISceneManager::createTriangleSelector() or 
		//! ISceneManager::createTriangleOctTreeSelector().
		//! \param ellipsoidPosition: Position of the ellipsoid.
		//! \param ellipsoidRadius: Radius of the ellipsoid.
		//! \param ellipsoidDirectionAndSpeed: Direction and speed of 
		//! the movement of the ellipsoid.
		//! \param gravityDirectionAndSpeed: Direction and force of gravity.
		//! \param triout: Optional parameter where the last triangle causing a 
		//! collision is stored, if there is a collision.
		//! \return Returns the new position of the ellipsoid.
		virtual core::vector3df getCollisionResultPosition(
			ITriangleSelector* selector,
			const core::vector3df &ellipsoidPosition,
			const core::vector3df& ellipsoidRadius, 
			const core::vector3df& ellipsoidDirectionAndSpeed,
			core::triangle3df& triout,
			f32 slidingSpeed = 0.0005f,
			const core::vector3df& gravityDirectionAndSpeed 
			= core::vector3df(0.0f, 0.0f, 0.0f)) = 0;

		//! Returns the scene node, which is currently visible under the overgiven 
		//! screencoordinates, viewed from the currently active camera. The collision
		//! tests are done using a bounding box for each scene node.
		//! \param pos: Position in pixel screen coordinates, under which the returned
		//! scene node will be.
		//! \param idBitMask: Only scene nodes with an id with bits set like in this mask
		//! will be tested. If the BitMask is 0, this feature is disabled.
		//! \return Returns the visible scene node under screen coordinates with matching
		//! bits in its id. If there is no scene node under this position, 0 is returned.
		virtual ISceneNode* getSceneNodeFromScreenCoordinatesBB(core::position2d<s32> pos,
			s32 idBitMask=0) = 0;

		//! Returns the nearest scene node which collides with a 3d ray and 
		//! which id matches a bitmask. The collision tests are done using a bounding
		//! box for each scene node.
		//! \param ray: Line with witch collisions are tested.
		//! \param idBitMask: Only scene nodes with an id with bits set like in this mask
		//! will be tested. If the BitMask is 0, this feature is disabled.
		//! \return Returns the scene node nearest to ray.start, which collides with the
		//! ray and matches the idBitMask, if the mask is not null. If no scene
		//! node is found, 0 is returned.
		virtual ISceneNode* getSceneNodeFromRayBB(core::line3d<f32> ray, 
			s32 idBitMask=0) = 0;

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
		virtual ISceneNode* getSceneNodeFromCameraBB(ICameraSceneNode* camera,
			s32 idBitMask=0) = 0;
	};


} // end namespace scene
} // end namespace irr

#endif

