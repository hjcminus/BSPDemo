/*==============================================================================
  BSPDemo
  MathLib.h - math library
================================================================================
*/

#ifndef __MATHLIB_H__
#define __MATHLIB_H__

#define DX_INLINE inline
#define DX_PI     3.14159265358979323846f

#define PLANE_X 0
#define PLANE_Y 1
#define PLANE_Z 2

#define DX_PITCH  0  //y
#define DX_YAW    1  //z
#define DX_ROLL   2  //x

DX_INLINE float Inv(float f);
DX_INLINE float DegToRad(float a);
DX_INLINE float AngleNormalize360(float a);

/*
========================================
  dxVec2
========================================
*/
class dxVec2
{
public:

    float x;
    float y;
};

/*
========================================
  dxVec3
========================================
*/
class dxVec3
{
public:

    float x;
    float y;
    float z;

    DX_INLINE dxVec3();
    DX_INLINE dxVec3(const dxVec3 &other);
    DX_INLINE dxVec3(float x, float y, float z);
    DX_INLINE dxVec3(float *v);

    DX_INLINE operator float * ();
    DX_INLINE operator const float * () const;

    DX_INLINE void          Set(float x, float y, float z);
    DX_INLINE void          Set(float *v);
    DX_INLINE void          Clear();

    DX_INLINE float         Length() const;
    DX_INLINE void          Normalize();

    DX_INLINE float &       operator [] (int i);
    DX_INLINE const float & operator [] (int i) const;

    DX_INLINE dxVec3        operator - () const;

    DX_INLINE dxVec3 &      operator = (const dxVec3 &other);
    DX_INLINE dxVec3        operator + (const dxVec3 &other);
	DX_INLINE dxVec3 &      operator += (const dxVec3 &other);
    DX_INLINE dxVec3 &      operator *= (float f);

	DX_INLINE dxVec3        operator * (float f);
};

DX_INLINE float Vec3DotProduct(const float *a, const float *b);

/*
========================================
  dxVec4
========================================
*/
class dxVec4
{
public:

    float x;
    float y;
    float z;
    float w;

    DX_INLINE operator float * ();
    DX_INLINE operator const float * () const;
};

/*
========================================
  dxMat3
========================================
*/
class dxMat3
{
public:

    float         mat[9];

    DX_INLINE void          Identity();

    DX_INLINE float &       operator [] (int i);
    DX_INLINE const float & operator [] (int i) const;
    DX_INLINE dxMat3 &      operator * (const dxMat3 &other);
    
};

/*
========================================
  dxPlane
========================================
*/
class dxPlane
{
public:

    dxVec3        normal;
    float         dist;
    byte          type;
    byte          signbits;
    byte          pad[2];

    DX_INLINE int BoxOnPlaneSide(float *mins, float *maxs);
};

DX_INLINE void AngleToVec3s(const dxVec3 &angles, dxVec3 &forward, dxVec3 &right, dxVec3 &up);
DX_INLINE void Vec3Transform(const dxVec3 &v, const dxMat3 &m, dxVec3 &out);
DX_INLINE void Mat3Rotation(float angle, const dxVec3 &vector, dxMat3 &out);

#include "MathLib.inl"

#endif /* !__MATHLIB_H__ */
