#include "Matrix.h"
#include "Vector3.h"
#include <float.h>
#include <math.h>
#include "Common.h"

inline double det3(double a,double b,double c,
	double d,double e,double f,
	double g,double h,double i){
	return a*e*i+d*h*c+g*b*f-g*e*c-d*b*i-a*h*f;
}

Matrix::Matrix(){
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			data[i][j]=0.0f;
}

Matrix::Matrix(const Matrix &_orig){
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			data[i][j]=_orig.getData(i,j);
}

void Matrix::setData(int i,int j,double _data){
	data[i][j]=_data;
}

double Matrix::getData(int i,int j)const{
	return data[i][j];
}

double Matrix::determinant()const{
	double det=0;
	det+=data[0][0]*det3(data[1][1],data[1][2],data[1][3],
						data[2][1],data[2][2],data[2][3],
						data[3][1],data[3][2],data[3][3]);
	det-=data[0][1]*det3(data[1][0],data[1][2],data[1][3],
						data[2][0],data[2][2],data[2][3],
						data[3][0],data[3][2],data[3][3]);
	det+=data[0][2]*det3(data[1][0],data[1][1],data[1][3],
						data[2][0],data[2][1],data[2][3],
						data[3][0],data[3][1],data[3][3]);
	det-=data[0][3]*det3(data[1][0],data[1][1],data[1][2],
						data[2][0],data[2][1],data[2][2],
						data[3][0],data[3][1],data[3][2]);
	return det;
}

Matrix Matrix::getInvert()const{
	Matrix res=*this;
	res.invert();
	return res;
}

bool Matrix::invert(){
	double det=determinant();
	if(fabs(det)<DBL_EPSILON)
		return false;

	Matrix inverse;
	inverse.setData(0,0,det3(data[1][1],data[1][2],data[1][3],
							data[2][1],data[2][2],data[2][3],
							data[3][1],data[3][2],data[3][3])/det);
	inverse.setData(0,1,-det3(data[0][1],data[0][2],data[0][3],
							data[2][1],data[2][2],data[2][3],
							data[3][1],data[3][2],data[3][3])/det);
	inverse.setData(0,2,det3(data[0][1],data[0][2],data[0][3],
							data[1][1],data[1][2],data[1][3],
							data[3][1],data[3][2],data[3][3])/det);
	inverse.setData(0,3,-det3(data[0][1],data[0][2],data[0][3],
							data[1][1],data[1][2],data[1][3],
							data[2][1],data[2][2],data[2][3])/det);

	inverse.setData(1,0,-det3(data[1][0],data[1][2],data[1][3],
							data[2][0],data[2][2],data[2][3],
							data[3][0],data[3][2],data[3][3])/det);
	inverse.setData(1,1,det3(data[0][0],data[0][2],data[0][3],
							data[2][0],data[2][2],data[2][3],
							data[3][0],data[3][2],data[3][3])/det);
	inverse.setData(1,2,-det3(data[0][0],data[0][2],data[0][3],
							data[2][0],data[2][2],data[2][3],
							data[3][0],data[3][2],data[3][3])/det);
	inverse.setData(1,3,det3(data[0][0],data[0][2],data[0][3],
							data[1][0],data[1][2],data[1][3],
							data[2][0],data[2][2],data[2][3])/det);

	inverse.setData(2,0,det3(data[1][0],data[1][1],data[1][3],
							data[2][0],data[2][1],data[2][3],
							data[3][0],data[3][1],data[3][3])/det);
	inverse.setData(2,1,-det3(data[0][0],data[0][1],data[0][3],
							data[2][0],data[2][1],data[2][3],
							data[3][0],data[3][1],data[3][3])/det);
	inverse.setData(2,2,det3(data[0][0],data[0][1],data[0][3],
							data[1][0],data[1][1],data[1][3],
							data[3][0],data[3][1],data[3][3])/det);
	inverse.setData(2,3,-det3(data[0][0],data[0][1],data[0][3],
							data[1][0],data[1][1],data[1][3],
							data[2][0],data[2][1],data[2][3])/det);

	inverse.setData(3,0,-det3(data[1][0],data[1][1],data[1][2],
							data[2][0],data[2][1],data[2][2],
							data[3][0],data[3][1],data[3][2])/det);
	inverse.setData(3,1,det3(data[0][0],data[0][1],data[0][2],
							data[2][0],data[2][1],data[2][2],
							data[3][0],data[3][1],data[3][2])/det);
	inverse.setData(3,2,-det3(data[0][0],data[0][1],data[0][2],
							data[1][0],data[1][1],data[1][2],
							data[3][0],data[3][1],data[3][2])/det);
	inverse.setData(3,3,det3(data[0][0],data[0][1],data[0][2],
							data[1][0],data[1][1],data[1][2],
							data[2][0],data[2][1],data[2][2])/det);
	*this=inverse;
	return true;
}

Matrix Matrix::getTranspose()const{
	Matrix res=*this;
	res.transpose();
	return res;
}

void Matrix::transpose(){
	for(int i=0;i<4;i++)
		for(int j=0;j<i;j++)
			swap<double>(data[i][j],data[j][i]);
}

Matrix zeroMatrix(){
	Matrix ret;
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			ret.data[i][j]=0.0f;
	return ret;
}

Matrix identityMatrix(){
	Matrix ret=zeroMatrix();
	for(int i=0;i<4;i++)
		ret.data[i][i]=1.0f;
	return ret;
}

Matrix operator+(const Matrix &m1,const Matrix &m2){
	Matrix ret;

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			ret.data[i][j]=m1.data[i][j]+m2.data[i][j];

	return ret;
}

Matrix operator-(const Matrix &m1,const Matrix &m2){
	Matrix ret;

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			ret.data[i][j]=m1.data[i][j]-m2.data[i][j];

	return ret;
}

Matrix operator*(const Matrix &m1,const Matrix &m2){
	Matrix ret=zeroMatrix();

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			for(int k=0;k<4;k++)
				ret.data[i][j]+=m1.data[i][k]*m2.data[k][j];

	return ret;
}

Vector3 operator*(const Matrix &m,const Vector3 &v){
	Vector3 ret;
	double temp;

	ret[0]=v[0]*m.data[0][0]+v[1]*m.data[0][1]+v[2]*m.data[0][2]+m.data[0][3];
	ret[1]=v[0]*m.data[1][0]+v[1]*m.data[1][1]+v[2]*m.data[1][2]+m.data[1][3];
	ret[2]=v[0]*m.data[2][0]+v[1]*m.data[2][1]+v[2]*m.data[2][2]+m.data[2][3];
	temp  =v[0]*m.data[3][0]+v[1]*m.data[3][1]+v[2]*m.data[3][2]+m.data[3][3];
	ret/=temp;

	return ret;
}

Matrix operator*(const Matrix &m1,double scale){
	Matrix ret;

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			ret.data[i][j]=m1.data[i][j]*scale;

	return ret;
}

Matrix operator*(double scale,const Matrix &m1){
	Matrix ret;

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			ret.data[i][j]=m1.data[i][j]*scale;

	return ret;
}

Matrix operator/(const Matrix &m1,double scale){
	Matrix ret;

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			ret.data[i][j]=m1.data[i][j]/scale;

	return ret;
}

Matrix operator/(double scale,const Matrix &m1){
	Matrix ret;

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			ret.data[i][j]=m1.data[i][j]/scale;

	return ret;
}

bool operator==(const Matrix &m1,const Matrix &m2){
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			if(m1.data[i][j]!=m2.data[i][j])
				return false;
	return true;
}

bool operator!=(const Matrix &m1,const Matrix &m2){
	return !(m1==m2);
}

const Matrix &Matrix::operator+()const{
	return *this;
}

Matrix Matrix::operator-()const{
	Matrix res;
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			res.setData(i,j,-data[i][j]);
	return res;
}

Matrix &Matrix::operator+=(const Matrix &m){
	*this=*this+m;
	return *this;
}

Matrix &Matrix::operator-=(const Matrix &m){
	*this=*this-m;
	return *this;
}

Matrix &Matrix::operator*=(const Matrix &m){
	*this=*this*m;
	return *this;
}

Matrix &Matrix::operator*=(double scale){
	*this=*this*scale;
	return *this;
}

Matrix &Matrix::operator/=(double scale){
	*this=*this/scale;
	return *this;
}

Matrix translate(const Vector3 &v){
	Matrix ret=identityMatrix();
	ret.data[0][3]=v.getX();
	ret.data[1][3]=v.getY();
	ret.data[2][3]=v.getZ();

	return ret;
}

Matrix translate(double _x,double _y,double _z){
	Matrix ret=identityMatrix();
	ret.data[0][3]=_x;
	ret.data[1][3]=_y;
	ret.data[2][3]=_z;

	return ret;
}

Matrix scale(const Vector3 &scaleV){
	Matrix ret=zeroMatrix();

	ret.data[0][0]=scaleV.getX();
	ret.data[1][1]=scaleV.getY();
	ret.data[2][2]=scaleV.getZ();
	ret.data[3][3]=1.0f;

	return ret;
}

Matrix scale(double _x,double _y,double _z){
	Matrix ret=zeroMatrix();

	ret.data[0][0]=_x;
	ret.data[1][1]=_y;
	ret.data[2][2]=_z;
	ret.data[3][3]=1.0f;

	return ret;
}

Matrix rotate(const Vector3 &axis,double angle){
	Vector3 _axis=normalize(axis);
	Matrix ret;
	double x=_axis.getX();
	double y=_axis.getY();
	double z=_axis.getZ();
	double cosine=cos(angle);
	double sine=sin(angle);
	double t=1-cosine;

	ret.data[0][0]=t*x*x+cosine;
	ret.data[0][1]=t*x*y-sine*y;
	ret.data[0][2]=t*x*z+sine*y;
	ret.data[0][3]=0.0f;

	ret.data[1][0]=t*x*y+sine*z;
	ret.data[1][1]=t*y*y+cosine;
	ret.data[1][2]=t*y*z-sine*x;
	ret.data[1][3]=0.0f;

	ret.data[2][0]=t*x*z-sine*y;
	ret.data[2][1]=t*y*z+sine*x;
	ret.data[2][2]=t*z*z+cosine;
	ret.data[2][3]=0.0f;

	ret.data[3][0]=0.0f;
	ret.data[3][1]=0.0f;
	ret.data[3][2]=0.0f;
	ret.data[3][3]=1.0f;

	return ret;
}

//angle is in radians
Matrix rotateX(double angle){
	Matrix ret=identityMatrix();

	double cosine=cos(angle);
	double sine=sin(angle);

	ret.data[1][1]=cosine;
	ret.data[1][2]=-sine;
	ret.data[2][1]=sine;
	ret.data[2][2]=cosine;
	
	return ret;
}

//angle is in radians

Matrix rotateY(double angle){
	Matrix ret=identityMatrix();

	double cosine=cos(angle);
	double sine=sin(angle);

	ret.data[0][0]=cosine;
	ret.data[0][2]=sine;
	ret.data[2][0]=-sine;
	ret.data[2][2]=cosine;

	return ret;
}

//angle is in radians
Matrix rotateZ(double angle){
	Matrix ret=identityMatrix();

	double cosine=cos(angle);
	double sine=sin(angle);
	
	ret.data[0][0]=cosine;
	ret.data[0][1]=-sine;
	ret.data[1][0]=sine;
	ret.data[1][1]=cosine;

	return ret;
}