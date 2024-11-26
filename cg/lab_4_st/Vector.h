#ifndef VECTOR_H
#define VECTOR_H

#include "Matrix.h"

class Vector
{
 public:
  
  float x, y, z, w;

  Vector() : x(0), y(0), z(0), w(1.0f)
  {
  }
  
  Vector(float _x, float _y, float _z, float _w = 1.0f) : x(_x), y(_y), z(_z), w(_w)
  {
  }


  Vector operator * (const Matrix& A) const
  {
      Vector E;
      E.x = x * A.M[0][0] + y * A.M[1][0] + z * A.M[2][0] + w * A.M[3][0];
      E.y = x * A.M[0][1] + y * A.M[1][1] + z * A.M[2][1] + w * A.M[3][1];
      E.z = x * A.M[0][2] + y * A.M[1][2] + z * A.M[2][2] + w * A.M[3][2];
      E.w = x * A.M[0][3] + y * A.M[1][3] + z * A.M[2][3] + w * A.M[3][3];
      return E;
  }

  void hom_to_st(Matrix &matrix){
      Vector temp(this->x, this->y, this->z, this->w);
      temp = temp * matrix;

//      temp.x /= temp.w;
//      temp.y /= temp.w;
//      temp.z /= temp.w;

      this->x = temp.x;
      this->y = temp.y;
      this->z = temp.z;
      this->w = temp.w;
  }

};

#endif // VECTOR_H