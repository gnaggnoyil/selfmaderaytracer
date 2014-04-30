#ifndef _RGB_H_
#define _RGB_H_ 1

/**
 * this class defines color in rgb colorspace, each component varies in [0,1]
 * there might be other colorspaces such as yuv. when needed, defines it as follow
 */
class rgb{
public:
	// constructors
	rgb(double _r,double _g,double _b);
	rgb(const rgb &_orig);
	rgb();

	double operator[](int i)const;
	double &operator[](int i);
	double getR()const;
	double getG()const;
	double getB()const;
	void setR(double _r);
	void setG(double _g);
	void setB(double _b);

	// operators
	friend rgb operator+(const rgb &c1,const rgb &c2);
	friend rgb operator-(const rgb &c1,const rgb &c2);
	friend rgb operator*(const rgb &c1,double scale);
	friend rgb operator*(double scale,const rgb &c1);
	friend rgb operator*(const rgb &c1,const rgb &c2);
	friend rgb operator/(const rgb &c1,double scale);
	friend rgb operator/(double scale,const rgb &c1);
	friend rgb operator/(const rgb &c1,const rgb &c2);

	const rgb &operator+()const;
	rgb operator-()const;
	rgb &operator+=(const rgb &c);
	rgb &operator-=(const rgb &c);
	rgb &operator*=(double scale);
	rgb &operator*=(const rgb &c);
	rgb &operator/=(double scale);
	rgb &operator/=(const rgb &c);
private:
	double r,g,b;
};
#endif // _RGB_H_