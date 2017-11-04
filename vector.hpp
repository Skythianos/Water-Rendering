#ifndef VECTOR
#define VECTOR

struct Vector {

	GLfloat x, y, z, w;

 
    Vector(GLfloat x0 = 0, GLfloat y0 = 0, GLfloat z0 = 0, GLfloat w0 = 0) {
        x = x0;
        y = y0;
        z = z0;
        w = w0;
    }
    
    Vector operator*(GLfloat a) {
        return Vector(x * a, y * a, z * a, w * a);
    }
    
    Vector operator+(Vector v) {
        return Vector(x + v.x, y + v.y, z + v.z, w + v.w);
    }

	void operator+=(Vector v) {
		x += v.x;
		y += v.y;
		z += v.z;
    }

	void operator-=(Vector v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
    }

	void operator*=(float f) {
		x *= f;
		y *= f;
		z *= f;
	}

	Vector operator/(float f) {
		Vector vector( x/f, y/f, z/f ); 
		return vector;
	}
    
    Vector operator-(Vector v) {
        return Vector(x - v.x, y - v.y, z - v.z, w - v.w);
    }
 
    double operator*(Vector v) {
        return (x * v.x + y * v.y + z * v.z);
    }
 
    Vector operator%(Vector v) {
        return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
 
   GLfloat Length() {
       return sqrt(x * x + y * y + z * z);
   }
 
   void Normalize() {
       GLfloat len = Length();
       x /= len;
       y /= len;
       z /= len;
   }

   void setVector3(GLfloat *vector) {
	   x = vector[0];
	   y = vector[1];
	   z = vector[2];
	   w = 0;
   }

    void setVector4(GLfloat *vector) {
	   x = vector[0];
	   y = vector[1];
	   z = vector[2];
	   w = vector[3];
   }
 
   GLfloat* getVector(GLfloat *vector) {
	   vector[0] = x;
	   vector[1] = y;
	   vector[2] = z;
	   return vector;
   }

   	Vector UnitVector() {
		Vector r = * this;
		r.Normalize();
		return r;
	}

};

#endif