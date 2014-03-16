/*==============================================================================
  BSPDemo
  MathLib.inl - math library
================================================================================
*/

#include <math.h>

static const int   IEEE_FLT_MANTISSA_BITS    = 23;
static const int   SMALLEST_NON_DENORMAL	 = 1 << IEEE_FLT_MANTISSA_BITS;
static const float FLT_SMALLEST_NON_DENORMAL = *reinterpret_cast<const float*>(&SMALLEST_NON_DENORMAL);
static const float FLT_INFINITY              = 1e30f;

DX_INLINE float Inv(float f)
{
    return (f > FLT_SMALLEST_NON_DENORMAL) ? 1.0f / f : FLT_INFINITY;
}

DX_INLINE float DegToRad(float a)
{
    return a * (DX_PI / 180.0f);
}

DX_INLINE float AngleNormalize360(float a)
{
    if (a >= 360.0f || a < 0.0f)
    {
        a -= floor(a * 1.0f / 360.0f) * 360.0f;
    }
    return a;
}

/*
========================================
  dxVec3
========================================
*/
DX_INLINE dxVec3::dxVec3()
{
    x = y = z = 0;
}

DX_INLINE dxVec3::dxVec3(const dxVec3 &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}

DX_INLINE dxVec3::dxVec3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

DX_INLINE dxVec3::dxVec3(float *v)
{
    this->x = v[0];
    this->y = v[1];
    this->z = v[2];
}

DX_INLINE dxVec3::operator float * ()
{
    return &x;
}

DX_INLINE dxVec3::operator const float * () const
{
    return &x;
}

DX_INLINE void dxVec3::Set(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

DX_INLINE void dxVec3::Set(float *v)
{
    this->x = v[0];
    this->y = v[1];
    this->z = v[2];
}

DX_INLINE void dxVec3::Clear()
{
    x = y = z = 0;
}

DX_INLINE float dxVec3::Length() const
{
    return sqrt(x * x + y * y + z * z);
}

DX_INLINE void dxVec3::Normalize()
{
    float l = Length();
    float inv = Inv(l);

    x *= inv;
    y *= inv;
    z *= inv;
}

DX_INLINE float & dxVec3::operator [] (int i)
{
    return (&x)[i];
}

DX_INLINE const float & dxVec3::operator [] (int i) const
{
    return (&x)[i];
}

DX_INLINE dxVec3 dxVec3::operator - () const
{
    return dxVec3(-x, -y, -z);
}

DX_INLINE dxVec3 & dxVec3::operator = (const dxVec3 &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

DX_INLINE dxVec3 dxVec3::operator + (const dxVec3 &other)
{
    return dxVec3(x + other.x, y + other.y, z + other.z);
}

DX_INLINE dxVec3 & dxVec3::operator += (const dxVec3 &other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

DX_INLINE dxVec3 & dxVec3::operator *= (float f)
{
    x *= f;
    y *= f;
    z *= f;
    return *this;
}

DX_INLINE dxVec3 dxVec3::operator * (float f)
{
	return dxVec3(x * f, y * f, z * f);
}

DX_INLINE float Vec3DotProduct(const float *a, const float *b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

/*
========================================
  dxVec4
========================================
*/
DX_INLINE dxVec4::operator float * ()
{
    return &x;
}

DX_INLINE dxVec4::operator const float * () const
{
    return &x;
}

/*
========================================
  dxMat3
========================================
*/
DX_INLINE void dxMat3::Identity()
{
    memset(mat, 0, sizeof(mat));

    mat[0*3+0] = 1.0f;
    mat[1*3+1] = 1.0f;
    mat[2*3+2] = 1.0f;
}

DX_INLINE dxMat3::operator float * ()
{
	return mat;
}

DX_INLINE dxMat3::operator const float * () const
{
	return mat;
}

DX_INLINE float & dxMat3::operator [] (int i)
{
    return mat[i];
}

DX_INLINE const float & dxMat3::operator [] (int i) const
{
    return mat[i];
}

DX_INLINE dxMat3 & dxMat3::operator * (const dxMat3 &other)
{
    float temp[9];

    //row 1
    temp[0*3+0] = mat[0*3+0] * other[0*3+0] + mat[1*3+0] * other[0*3+1] + mat[2*3+0] * other[0*3+2];
    temp[1*3+0] = mat[0*3+0] * other[1*3+0] + mat[1*3+0] * other[1*3+1] + mat[2*3+0] * other[1*3+2];
    temp[2*3+0] = mat[0*3+0] * other[2*3+0] + mat[1*3+0] * other[2*3+1] + mat[2*3+0] * other[2*3+2];

    //row 2
    temp[0*3+1] = mat[0*3+1] * other[0*3+0] + mat[1*3+1] * other[0*3+1] + mat[2*3+1] * other[0*3+2];
    temp[1*3+1] = mat[0*3+1] * other[1*3+0] + mat[1*3+1] * other[1*3+1] + mat[2*3+1] * other[1*3+2];
    temp[2*3+1] = mat[0*3+1] * other[2*3+0] + mat[1*3+1] * other[2*3+1] + mat[2*3+1] * other[2*3+2];

    //row 3
    temp[0*3+2] = mat[0*3+2] * other[0*3+0] + mat[1*3+2] * other[0*3+1] + mat[2*3+2] * other[0*3+2];
    temp[1*3+2] = mat[0*3+2] * other[1*3+0] + mat[1*3+2] * other[1*3+1] + mat[2*3+2] * other[1*3+2];
    temp[2*3+2] = mat[0*3+2] * other[2*3+0] + mat[1*3+2] * other[2*3+1] + mat[2*3+2] * other[2*3+2];

	memcpy(mat, temp, sizeof(mat));

    return *this;
}

/*
========================================
  dxMat4
========================================
*/
DX_INLINE void dxMat4::Identity()
{
    memset(mat, 0, sizeof(mat));

    mat[0*4+0] = 1.0f;
    mat[1*4+1] = 1.0f;
    mat[2*4+2] = 1.0f;
	mat[3*4+3] = 1.0f;
}

DX_INLINE dxMat4::operator float * ()
{
	return mat;
}

DX_INLINE dxMat4::operator const float * () const
{
	return mat;
}

DX_INLINE float & dxMat4::operator [] (int i)
{
    return mat[i];
}

DX_INLINE const float & dxMat4::operator [] (int i) const
{
    return mat[i];
}

DX_INLINE dxMat4 & dxMat4::operator * (const dxMat4 &other)
{
	float temp[16];

	//row 1
	temp[0*4+0] = mat[0*4+0] * other[0*4+0] + mat[1*4+0] * other[0*4+1] + mat[2*4+0] * other[0*4+2] + mat[3*4+0] * other[0*4+3];
	temp[1*4+0] = mat[0*4+0] * other[1*4+0] + mat[1*4+0] * other[1*4+1] + mat[2*4+0] * other[1*4+2] + mat[3*4+0] * other[1*4+3];
	temp[2*4+0] = mat[0*4+0] * other[2*4+0] + mat[1*4+0] * other[2*4+1] + mat[2*4+0] * other[2*4+2] + mat[3*4+0] * other[2*4+3];
	temp[3*4+0] = mat[0*4+0] * other[3*4+0] + mat[1*4+0] * other[3*4+1] + mat[2*4+0] * other[3*4+2] + mat[3*4+0] * other[3*4+3];

	//row 2
	temp[0*4+1] = mat[0*4+1] * other[0*4+0] + mat[1*4+1] * other[0*4+1] + mat[2*4+1] * other[0*4+2] + mat[3*4+1] * other[0*4+3];
	temp[1*4+1] = mat[0*4+1] * other[1*4+0] + mat[1*4+1] * other[1*4+1] + mat[2*4+1] * other[1*4+2] + mat[3*4+1] * other[1*4+3];
	temp[2*4+1] = mat[0*4+1] * other[2*4+0] + mat[1*4+1] * other[2*4+1] + mat[2*4+1] * other[2*4+2] + mat[3*4+1] * other[2*4+3];
	temp[3*4+1] = mat[0*4+1] * other[3*4+0] + mat[1*4+1] * other[3*4+1] + mat[2*4+1] * other[3*4+2] + mat[3*4+1] * other[3*4+3];

	//row 3
	temp[0*4+2] = mat[0*4+2] * other[0*4+0] + mat[1*4+2] * other[0*4+1] + mat[2*4+2] * other[0*4+2] + mat[3*4+2] * other[0*4+3];
	temp[1*4+2] = mat[0*4+2] * other[1*4+0] + mat[1*4+2] * other[1*4+1] + mat[2*4+2] * other[1*4+2] + mat[3*4+2] * other[1*4+3];
	temp[2*4+2] = mat[0*4+2] * other[2*4+0] + mat[1*4+2] * other[2*4+1] + mat[2*4+2] * other[2*4+2] + mat[3*4+2] * other[2*4+3];
	temp[3*4+2] = mat[0*4+2] * other[3*4+0] + mat[1*4+2] * other[3*4+1] + mat[2*4+2] * other[3*4+2] + mat[3*4+2] * other[3*4+3];

	//row 4
	temp[0*4+3] = mat[0*4+3] * other[0*4+0] + mat[1*4+3] * other[0*4+1] + mat[2*4+3] * other[0*4+2] + mat[3*4+3] * other[0*4+3];
	temp[1*4+3] = mat[0*4+3] * other[1*4+0] + mat[1*4+3] * other[1*4+1] + mat[2*4+3] * other[1*4+2] + mat[3*4+3] * other[1*4+3];
	temp[2*4+3] = mat[0*4+3] * other[2*4+0] + mat[1*4+3] * other[2*4+1] + mat[2*4+3] * other[2*4+2] + mat[3*4+3] * other[2*4+3];
	temp[3*4+3] = mat[0*4+3] * other[3*4+0] + mat[1*4+3] * other[3*4+1] + mat[2*4+3] * other[3*4+2] + mat[3*4+3] * other[3*4+3];

	memcpy(mat, temp, sizeof(mat));

    return *this;
}

/*
========================================
  dxPlane
========================================
*/
int dxPlane::BoxOnPlaneSide(float *mins, float *maxs)
{
    dxVec3 v;
    float d;

    bool positive = false, negative = false;

    v[0] = mins[0];
    v[1] = mins[1];
    v[2] = mins[2];

    d = Vec3DotProduct(v, normal) + dist;
    if (d < 0)
    {
        negative = true;
    }
    else
    {
        positive = true;
    }

    v[2] = maxs[2];
    d = Vec3DotProduct(v, normal) + dist;
    if (d < 0)
    {
        negative = true;
    }
    else
    {
        positive = true;
    }

    v[1] = maxs[1];
    v[2] = mins[2];
    d = Vec3DotProduct(v, normal) + dist;
    if (d < 0)
    {
        negative = true;
    }
    else
    {
        positive = true;
    }

    v[2] = maxs[2];
    d = Vec3DotProduct(v, normal) + dist;
    if (d < 0)
    {
        negative = true;
    }
    else
    {
        positive = true;
    }

    v[0] = maxs[0];
    v[1] = mins[1];
    v[2] = mins[2];

    d = Vec3DotProduct(v, normal) + dist;
    if (d < 0)
    {
        negative = true;
    }
    else
    {
        positive = true;
    }

    v[2] = maxs[2];
    d = Vec3DotProduct(v, normal) + dist;
    if (d < 0)
    {
        negative = true;
    }
    else
    {
        positive = true;
    }

    v[1] = maxs[1];
    v[2] = mins[2];
    d = Vec3DotProduct(v, normal) + dist;
    if (d < 0)
    {
        negative = true;
    }
    else
    {
        positive = true;
    }

    v[2] = maxs[2];
    d = Vec3DotProduct(v, normal) + dist;
    if (d < 0)
    {
        negative = true;
    }
    else
    {
        positive = true;
    }

    if (negative && positive)
    {
        return 0;
    }
    else if (positive)
    {
        return 1;
    }

    return 2;
}

DX_INLINE void AngleToVec3s(const dxVec3 &angles, dxVec3 &forward, dxVec3 &right, dxVec3 &up)
{
    float p = DegToRad(angles[DX_PITCH]);
    float y = DegToRad(angles[DX_YAW]);
    float r = DegToRad(angles[DX_ROLL]);

    float sp = sinf(p);
    float cp = cosf(p);

    float sy = sinf(y);
    float cy = cosf(y);

    float sr = sinf(r);
    float cr = cosf(r);

    forward[0] =  cp * cy;
    forward[1] =  cp * sy;
    forward[2] = -sp;

    right[0] = sy *  cr + sp * cy * -sr;
    right[1] =-cy *  cr + sp * sy * -sr;
    right[2] = cp * -sr;

    up[0] = sp * cy * cr + -sy * -sr;
    up[1] = sp * sy * cr +  cy * -sr;
    up[2] = cp * cr;
}

DX_INLINE void Vec3Transform(const dxVec3 &v, const dxMat3 &m, dxVec3 &out)
{
    out[0] = m[0*3+0] * v[0] + m[1*3+0] * v[1] + m[2*3+0] * v[2];
    out[1] = m[0*3+1] * v[0] + m[1*3+1] * v[1] + m[2*3+1] * v[2];
    out[2] = m[0*3+2] * v[0] + m[1*3+2] * v[1] + m[2*3+2] * v[2];
}

//assume vector normalized
DX_INLINE void Mat3Rotation(float angle, const dxVec3 &vector, dxMat3 &out)
{
    float x = vector.x;
    float y = vector.y;
    float z = vector.z;

    float rad = DegToRad(angle);

    float s = sinf(rad);
    float c = cosf(rad);

    float one_minus_c = 1.0f - c;

    float xy_one_minus_c = x * y * one_minus_c;
    float xz_one_minus_c = x * z * one_minus_c;
    float yz_one_minus_c = y * z * one_minus_c;

    float xs = x * s;
    float ys = y * s;
    float zs = z * s;

    out[0*3+0] = c + x * x * one_minus_c;
    out[0*3+1] = xy_one_minus_c + zs;
    out[0*3+2] = xz_one_minus_c - ys;

    out[1*3+0] = xy_one_minus_c - zs;
    out[1*3+1] = c + y * y * one_minus_c;
    out[1*3+2] = yz_one_minus_c + xs;

    out[2*3+0] = xz_one_minus_c + ys;
    out[2*3+1] = yz_one_minus_c - xs;
    out[2*3+2] = c + z * z * one_minus_c;
}
