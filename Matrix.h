#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "Vector3.h"

/**
 * this defines a 4x4 matrix used for rotating or scaling or something
 * a vector can just right-multiply the matrix to apply a transform
 */
class Matrix{
public:
	//constructors
	Matrix();
	Matrix(const Matrix &_orig);

	// returns false if fails, otherwise true
	void setData(int i,int j,double data);
	double getData(int i,int j)const;

	// returns the determinant of this matrix
	double determinant();
	
	Matrix getInvert()const;
	// returns true if invert succeed, otherwise false
	bool invert();
	Matrix getTranspose()const;
	void transpose();
	friend Matrix zeroMatrix();
	friend Matrix identityMatrix();

	// operators
	friend Matrix operator+(const Matrix &m1,const Matrix &m2);
	friend Matrix operator-(const Matrix &m1,const Matrix &m2);
	friend Matrix operator*(const Matrix &m1,const Matrix &m2);
	friend Vector3 operator*(const Matrix &m,const Vector3 &v);
	friend Matrix operator*(const Matrix &m,double scale);
	friend Matrix operator*(double scale,const Matrix &m);
	friend Matrix operator/(const Matrix &m,double scale);
	friend Matrix operator/(double scale,const Matrix &m);
	friend bool operator==(const Matrix &m1,const Matrix &m2);
	friend bool operator!=(const Matrix &m1,const Matrix &m2);

	const Matrix &operator+()const;
	Matrix operator-()const;
	Matrix &operator+=(const Matrix &m);
	Matrix &operator-=(const Matrix &m);
	Matrix &operator*=(const Matrix &m);
	Matrix &operator*=(double scale);
	Matrix &operator/=(double scale);

	// generate a matrix that performs translation through a certain vector
	friend Matrix translate(const Vector3 &v);
	friend Matrix translate(double _x,double _y,double _z);
	// generate a matrix that performs scaling for a certian scaling factor
	friend Matrix scale(double scaleX,double scaleY,double scaleZ);
	// generate a matrix that performs rotation for certain angle(in radius) base on a certain axis
	friend Matrix rotate(const Vector3 &axis,double angle);
	friend Matrix rotateX(double angle);	// rotate by X-axis
	friend Matrix rotateY(double angle);	// rotate by Y-axis
	friend Matrix rotateZ(double angle);	// rotate by Z-axis
private:
	double data[4][4];
};
#endif // _MATRIX_H_