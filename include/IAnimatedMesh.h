// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_ANIMATED_MESH_H_INCLUDED__
#define __I_ANIMATED_MESH_H_INCLUDED__

#include "IUnknown.h"
#include "IMesh.h"
#include "matrix4.h"

namespace irr
{
namespace scene
{

	//! Interface for an animated mesh.
	class IAnimatedMesh : public IUnknown
	{
	public:

		//! destructor
		virtual ~IAnimatedMesh() {};

		//! Gets the frame count of the animated mesh.
		//! \return Returns the amount of frames. If the amount is 1, it is a static, non animated mesh.
		virtual s32 getFrameCount() = 0;

		//! Returns the IMesh interface for a frame.
		//! \param frame: Frame number as zero based index. The maximum frame number is
		//! getFrameCount() - 1;
		//! \param detailLevel: Level of detail. 0 is the lowest,
		//! 255 the highest level of detail. Most meshes will ignore the detail level.
		//! \param startFrameLoop: Because some animated meshes (.MD2) are blended between 2
		//! static frames, and maybe animated in a loop, the startFrameLoop and the endFrameLoop
		//! have to be defined, to prevent the animation to be blended between frames which are
		//! outside of this loop. 
		//! If startFrameLoop and endFrameLoop are both -1, they are ignored.
		//! \param endFrameLoop: see startFrameLoop.
		//! \return Returns the animated mesh based on a detail level. 
		virtual IMesh* getMesh(s32 frame, s32 detailLevel=255, s32 startFrameLoop=-1, s32 endFrameLoop=-1) = 0;

		//! Returns a pointer to a transformation matrix of a part of the 
		//! mesh based on a frame time. This is used for being able to attach
		//! objects to parts of animated meshes. For example a weapon to an animated 
		//! hand. Not all animated meshes implement this, they will then simply return
		//! null.
		//! \param name: Name of the mesh part. For example if you loaded a .ms3d file
		//! and want to get the transformation matrix of the hand to be able to display
		//! a weapon like it would be holded by the left hand, you can get the matrix
		//! by calling getMatrixOfMeshPart("lefthand", yourframe). "lefthand" would then
		//! be the name of the joint/bone.
		//! \param frame: Frame of the animation.
		//! \return: Returns a pointer to the matrix of the mesh part or 
		//! null if the mesh doesn't support mesh part matrizes or there is no
		//! mesh part with this name.
		virtual core::matrix4* getMatrixOfMeshPart(const c8* name, s32 frame) 
		{
			return 0;
		}

		//! Returns an axis aligned bounding box of the mesh.
		//! \return A bounding box of this mesh is returned.
		virtual const core::aabbox3d<f32>& getBoundingBox() const = 0;
	};

} // end namespace scene
} // end namespace irr

#endif

