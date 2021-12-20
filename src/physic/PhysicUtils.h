#pragma once

#define PMAKETRANSFORM(t, m)					physx::PxTransform t(*((physx::PxMat44*)&m)); if(!t.isValid()) t.q.normalize();

#define PXVEC(v)								*((physx::PxVec3*)&v)
#define PXVEC_C(v)								*((const physx::PxVec3*)&v)

#define VEC3(v)									*((Core::Vector3*)&v)
#define VEC3_C(v)								*((const Core::Vector3*)&v)
