#ifndef __MATRIX_MATH_H_
#define __MATRIX_MATH_H_

#include <math.h>
//#include <glm.hpp> :)
#include <vectors.h>
#include <cstring>


namespace mm
{
    const double Pi = 3.14159;

  struct Matrix
  {
    float mat[16] = {
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
    };

    void null()
    {
      memset(mat, 0, sizeof(float)*16);
    }
  };
  
static int multMatrix(float *a, const int n, float *b, float *c);

static void nullset(float matrix[16])
{
    for(int i = 0; i < 16; ++i)
        matrix[i] = 0.0f;   
}
/*
static glm::vec3 to(MVector3f& vector)
{
    return glm::vec3(vector.x, vector.y, vector.z);
}
*/


static float dot(MVector3f fir, MVector3f sec)
{
    return ((fir.x * sec.x) + (fir.y * sec.y) + (fir.z * sec.z));
}


static MVector3f cross(MVector3f& A, MVector3f& B) 
{
    return MVector3f(
        A.y * B.z - B.y * A.z,
		A.z * B.x - B.z * A.x,
		A.x * B.y - B.x * A.y);
}


static void oneset(float matrix[16])
{
    float d[] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };
    for (int i = 0; i < 16; ++i)
        matrix[i] = d[i];
}


static MVector3f normalize(const MVector3f v)
{
    float length_of_v = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
    return MVector3f(v.x / length_of_v, v.y / length_of_v, v.z / length_of_v);
}


static void rotate(float* matrix, float x, float y, float z, bool not_mult = false)
{
    float cos_x = cosf(x), cos_y = cosf(y), cos_z = cosf(z);
    float sin_x = sinf(x), sin_y = sinf(y), sin_z = sinf(z);
    float tidim[] = 
    {
        cos_y * cos_z,                        -sin_z * cos_y,                          sin_y,           0.0f,
        sin_x * sin_y * cos_z + sin_z * cos_x, sin_x * sin_y * sin_z + cos_x * cos_z, -sin_x * cos_y,   0.0f,
        sin_x * sin_z - sin_y * cos_x * cos_z, sin_x * cos_z + sin_y * sin_z * cos_x,  cos_x * cos_y,   0.0f,
        0.0f,                                  0.0f,                                   0.0f,            1.0f
    };
    if(!not_mult)
    {
        float timed[16];
        multMatrix(matrix, 4, tidim, timed);
        for(int i = 0; i < 16; ++i)
            matrix[i] = timed[i];
    }
    else
    {
        for(int i = 0; i < 16; ++i)
            matrix[i] = tidim[i];
    }
}

static void scale(float* matrix, float x, float y, float z, bool not_mult = false)
{
    float sr[16] = 
        {
        x,    0.0f, 0.0f, 0.0f,
        0.0f, y,    0.0f, 0.0f,
        0.0f, 0.0f, z,    0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    
    if(!not_mult)
    {
        float timed[16];
        multMatrix(matrix, 4, sr, timed);
        for(int i = 0; i < 16; ++i)
            matrix[i] = timed[i];
    }
    else
    {
        for(int i = 0; i < 16; ++i)
            matrix[i] = sr[i];
    }
}

static void translate(float* matrix, float x, float y, float z, bool not_mult = false)
{
    float tr[16] = 
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x,    y,    z,    1.0f
    };
    if(!not_mult)
    {
        float timed[16];
        multMatrix(matrix, 4, tr, timed);
        for(int i = 0; i < 16; ++i)
            matrix[i] = timed[i];
    }
    else
    {
        for(int i = 0; i < 16; ++i)
            matrix[i] = tr[i];
    }
}

static void printMatrix(const char* name, float* projection)
{
    printf("matrix \"%s\":\n", name);
    for (int i = 0; i < 4; ++i)
    {
        printf("%.4f %.4f %.4f %.4f\n", projection[i * 4], projection[i * 4 + 1], projection[i * 4 + 2], projection[i * 4 + 3]);
    }
}

static void perspective(float* mret, float fov, float aspect, float near, float far) {
    float D2R = M_PI / 180.0;
    float yScale = 1.0 / tan(D2R * fov / 2);
    float xScale = yScale / aspect;
    float nearmfar = near - far;
    float m[] = {
        xScale, 0,      0,                          0,
        0,      yScale, 0,                          0,
        0,      0,      (far + near) / nearmfar,   -1,
        0,      0,      2 * far * near / nearmfar,  0 // 1 but not 0 
    };
    memcpy(mret, m, sizeof(float) * 16);
}


static void LookAt(float* _mat, MVector3f eye, MVector3f at, MVector3f up)
{
    
    MVector3f zaxis = normalize(at - eye);
    MVector3f xaxis = normalize(cross(zaxis, up));
    MVector3f yaxis = cross(xaxis, zaxis);

    zaxis *= -1;

    float mat[] = {
        xaxis.x,          yaxis.x,         zaxis.x,  0,
        xaxis.y,          yaxis.y,         zaxis.y,  0,
        xaxis.z,          yaxis.z,         zaxis.z,  0,
        dot(xaxis, eye * -1),  dot(yaxis, eye * -1),  dot(zaxis, eye * -1),  1
    };

    memcpy(_mat, mat, sizeof(float) * 16);
}


static float radians(float deg)
{
    return deg * 0.01745329251994329576923690768489;
}

static double radians(double deg)
{
    return deg * 0.01745329251994329576923690768489;
}



int multMatrix(float *a, const int n, float *b, float *c)
{
	int i, j, k;
	for (i = 0; i < n; i++)
    {
		for (j = 0; j < n; ++j)
        {
			c[i * n + j] = 0;
			for (k = 0; k < n; ++k)
            {
				c[i * n + j] += a[i * n + k] * b[k * n + j];
			}
		}
	}
	return 0;
}


}




#endif // !__MATRIX_MATH_H_
