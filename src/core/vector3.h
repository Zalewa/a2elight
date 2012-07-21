/*
 *  Albion 2 Engine "light"
 *  Copyright (C) 2004 - 2012 Florian Ziesche
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License only.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __A2E_VECTOR_3_H__
#define __A2E_VECTOR_3_H__

#include "core/cpp_headers.h"
#include "core/matrix4.h"

template <typename T> class vector3;
typedef vector3<float> float3;
typedef vector3<double> double3;
typedef vector3<unsigned int> index3;
typedef vector3<unsigned int> uint3;
typedef vector3<int> int3;
typedef vector3<short> short3;
typedef vector3<unsigned short> ushort3;
typedef vector3<char> char3;
typedef vector3<unsigned char> uchar3;
typedef vector3<bool> bool3;
typedef vector3<size_t> size3;
typedef vector3<ssize_t> ssize3;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-compare"
#endif

template <typename T> class A2E_API __attribute__((packed)) vector3 {
public:
	union {
		T x, r;
	};
	union {
		T y, g;
	};
	union {
		T z, b;
	};
	
	a2e_constexpr vector3() noexcept : x((T)0), y((T)0), z((T)0) {}
	a2e_constexpr vector3(vector3<T>&& vec3) noexcept : x(vec3.x), y(vec3.y), z(vec3.z) {}
	a2e_constexpr vector3(const vector3<T>& vec3) noexcept : x(vec3.x), y(vec3.y), z(vec3.z) {}
	a2e_constexpr vector3(const vector3<T>* vec3) noexcept : x(vec3->x), y(vec3->y), z(vec3->z) {}
	a2e_constexpr vector3(const T& vx, const T& vy, const T& vz) noexcept : x(vx), y(vy), z(vz) {}
	a2e_constexpr vector3(const T& f) noexcept : x(f), y(f), z(f) {}
	template <typename U> a2e_constexpr vector3(const vector3<U>& vec3) noexcept : x((T)vec3.x), y((T)vec3.y), z((T)vec3.z) {}
	
	T& operator[](size_t index);
	const T& operator[](size_t index) const;
	vector3& operator=(const vector3& v);
	vector3 operator+(const vector3& v) const;
	vector3 operator+(const T& f) const;
	vector3 operator-(const vector3& v) const;
	vector3 operator-() const;
	vector3 operator/(const T& f) const;
	vector3 operator/(const vector3& v) const;
	vector3 operator*(const T& f) const;
	T operator*(const vector3& v) const;		// dot product
	vector3 operator*(const matrix4f& mat) const;
	vector3 operator^(const vector3& v) const;	// cross product
	vector3& operator^=(const vector3& v);		// cross product
	vector3& operator+=(const vector3& v);
	vector3& operator-=(const vector3& v);
	vector3& operator*=(const T& f);
	vector3& operator*=(const vector3& v);		// dot product
	vector3& operator*=(const matrix4f& mat);
	vector3& operator/=(const T& f);
	vector3& operator/=(const vector3& v);
	vector3 operator%(const vector3& v) const;
	vector3& operator%=(const vector3& v);
	
	// comparisons
	bool is_equal(const vector3& v) const;
	bool is_unequal(const vector3& v) const;
	bool3 operator==(const vector3& v) const;
	bool3 operator!=(const vector3& v) const;
	bool3 operator<(const vector3& v) const;
	bool3 operator>(const vector3& v) const;
	bool3 operator<=(const vector3& v) const;
	bool3 operator>=(const vector3& v) const;
	bool3 operator&(const bool3& bv) const {
		return bool3(x && bv.x, y && bv.y, z && bv.z);
	}
	bool3 operator|(const bool3& bv) const{
		return bool3(x || bv.x, y || bv.y, z || bv.z);
	}
	bool3 operator^(const bool& bl) const{
		return bool3(bool(x) ^ bl, bool(y) ^ bl, bool(z) ^ bl);
	}
	bool any() const; //! only bool3!
	bool all() const; //! only bool3!
	
	friend vector3 operator*(const T& f, const vector3& v) {
		return vector3<T>(f * v.x, f * v.y, f * v.z);
	}
	
	friend ostream& operator<<(ostream& output, const vector3& v) {
		output << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return output;
	}
	string to_string() const {
		stringstream sstr;
		sstr << *this;
		return sstr.str();
	}
	
	vector3 normalized() const;
	vector3 scaled(const vector3& v) const;
	vector3& normalize();
	vector3& floor();
	vector3& ceil();
	vector3& round();
	vector3 floored() const;
	vector3 ceiled() const;
	vector3 rounded() const;
	vector3 sign() const;
	void scale(const vector3& v);
	void clamp(const T& min, const T& max);
	vector3 clamped(const T& max) const;
	vector3 clamped(const T& min, const T& max) const;
	void set(const T vx, const T vy, const T vz);
	void set(const vector3 v);
	void set_if(const bool3& bv, const T vx, const T vy, const T vz);
	void set_if(const bool3& bv, const vector3 v);
	template<typename Function> void apply(Function f);
	template<typename Function> void apply_if(const bool3& bv, Function f);
	vector3& rotate(const T& x_rotation, const T& y_rotation, const T& z_rotation);
	T length() const;
	T angle(const vector3& v) const;
	T distance(const vector3& v) const;
	vector3 abs() const;
	vector3& min(const vector3& v);
	vector3& max(const vector3& v);
	T min_element() const;
	T max_element() const;
	size_t min_element_index() const;
	size_t max_element_index() const;
	template<unsigned int comp1, unsigned int comp2, unsigned int comp3> vector3 swizzle() const {
		return vector3(((T*)this)[comp1], ((T*)this)[comp2], ((T*)this)[comp3]);
	}
	
	bool is_null() const;
	bool is_nan() const;
	bool is_inf() const;
		
	// a component-wise minimum between two vector3s
	static const vector3 min(const vector3& v1, const vector3& v2) {
		return vector3(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
	}
	
	// a component-wise maximum between two vector3s
	static const vector3 max(const vector3& v1, const vector3& v2) {
		return vector3(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
	}
	
	// linear interpolation between the points v1 and v2: v2 + (v1 - v2) * coef
	static const vector3 mix(const vector3& v1, const vector3& v2, const vector3& v3, const T& u, const T& v) {
		return vector3(v1.x * u + v2.x * v + v3.x * ((T)1 - u - v),
					   v1.y * u + v2.y * v + v3.y * ((T)1 - u - v),
					   v1.z * u + v2.z * v + v3.z * ((T)1 - u - v));
	}
	static const vector3 mix(const vector3& v1, const vector3& v2, const T& coef) {
		return vector3(v1.x * coef + v2.x * ((T)1 - coef),
					   v1.y * coef + v2.y * ((T)1 - coef),
					   v1.z * coef + v2.z * ((T)1 - coef));
	}
	
	static const vector3 faceforward(const vector3& N, const vector3& I, const vector3& Nref) {
		return ((Nref * I) < ((T)0) ? N : -N);
	}
	
	//! N must be normalized
	static const vector3 reflect(const vector3& N, const vector3& I) {
		return I - ((T)2) * (N * I) * N;
	}
	
	//! N, I must be normalized
	static const vector3 refract(const vector3& N, const vector3& I, const T& eta) {
		const T dNI(N * I);
		const T k(((T)1) - (eta * eta) * (((T)1) - dNI * dNI));
		return (k < ((T)0) ? vector3((T)0) : (eta * I - (eta * dNI + sqrt(k)) * N));
	}
	
	T dot(const vector3& v) const;
	vector3 cross(const vector3& v) const;
	vector3 mix(const vector3& v2, const T& coef) const;
	vector3 faceforward(const vector3& I, const vector3& Nref) const;
	vector3 reflect(const vector3& I) const;
	vector3 refract(const vector3& I, const T& eta) const;
	
};


template<typename T> T& vector3<T>::operator[](size_t index) {
	return ((T*)this)[index];
}

template<typename T> const T& vector3<T>::operator[](size_t index) const {
	return ((T*)this)[index];
}

/*! = operator overload
 */
template<typename T> vector3<T>& vector3<T>::operator=(const vector3<T>& v) {
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	return *this;
}

/*! + operator overload
 */
template<typename T> vector3<T> vector3<T>::operator+(const vector3<T>& v) const {
	return vector3<T>(this->x + v.x, this->y + v.y, this->z + v.z);
}

template<typename T> vector3<T> vector3<T>::operator+(const T& f) const {
	return vector3<T>(this->x + f, this->y + f, this->z + f);
}

/*! - operator overload
 */
template<typename T> vector3<T> vector3<T>::operator-(const vector3<T>& v) const {
	return vector3<T>(this->x - v.x, this->y - v.y, this->z - v.z);
}

/*! - operator overload
 */
template<typename T> vector3<T> vector3<T>::operator-() const {
	return vector3<T>(-this->x, -this->y, -this->z);
}

/*! / operator overload
 */
template<typename T> vector3<T> vector3<T>::operator/(const T& f) const {
	return vector3<T>(this->x / f, this->y / f, this->z / f);
}

/*! / operator overload
 */
template<typename T> vector3<T> vector3<T>::operator/(const vector3<T>& v) const {
	return vector3<T>(this->x / v.x, this->y / v.y, this->z / v.z);
}

/*! * operator overload
 */
template<typename T> vector3<T> vector3<T>::operator*(const T& f) const {
	return vector3<T>(this->x * f, this->y * f, this->z * f);
}

/*! += operator overload
 */
template<typename T> vector3<T>& vector3<T>::operator+=(const vector3<T>& v) {
	*this = *this + v;
	return *this;
}

/*! -= operator overload
 */
template<typename T> vector3<T>& vector3<T>::operator-=(const vector3<T>& v) {
	*this = *this - v;
	return *this;
}

/*! *= operator overload
 */
template<typename T> vector3<T>& vector3<T>::operator*=(const T& f) {
	*this = *this * f;
	return *this;
}

/*! *= operator overload
 */
template<typename T> vector3<T>& vector3<T>::operator*=(const vector3<T>& v) {
	*this = *this * v;
	return *this;
}

template<typename T> vector3<T> vector3<T>::operator*(const matrix4f& mat) const {
	return {
		mat.data[0] * x + mat.data[4] * y + mat.data[8] * z + mat.data[12],
		mat.data[1] * x + mat.data[5] * y + mat.data[9] * z + mat.data[13],
		mat.data[2] * x + mat.data[6] * y + mat.data[10] * z + mat.data[14],
	};
}

template<typename T> vector3<T>& vector3<T>::operator*=(const matrix4f& mat) {
	*this = *this * mat;
	return *this;
}

/*! /= operator overload
 */
template<typename T> vector3<T>& vector3<T>::operator/=(const T& f) {
	*this = *this / f;
	return *this;
}

template<typename T> vector3<T>& vector3<T>::operator/=(const vector3<T>& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

/*! * operator overload / cross product (^=)
 */
template<typename T> vector3<T>& vector3<T>::operator^=(const vector3<T>& v) {
	*this = *this ^ v;
	return *this;
}

/*! == operator overload
 */
template<typename T> bool vector3<T>::is_equal(const vector3<T>& v) const {
	if(this->x == v.x && this->y == v.y && this->z == v.z) {
		return true;
	}
	return false;
}

/*! != operator overload
 */
template<typename T> bool vector3<T>::is_unequal(const vector3<T>& v) const {
	return (this->is_equal(v)) ^ true;
}

/*! == operator overload
 */
template<typename T> bool3 vector3<T>::operator==(const vector3<T>& v) const {
	return bool3(this->x == v.x, this->y == v.y, this->z == v.z);
}

/*! != operator overload
 */
template<typename T> bool3 vector3<T>::operator!=(const vector3<T>& v) const {
	return bool3(this->x != v.x, this->y != v.y, this->z != v.z);
}

/*! < operator overload
 */
template<typename T> bool3 vector3<T>::operator<(const vector3<T>& v) const {
	return bool3(this->x < v.x, this->y < v.y, this->z < v.z);
}

/*! > operator overload
 */
template<typename T> bool3 vector3<T>::operator>(const vector3<T>& v) const {
	return bool3(this->x > v.x, this->y > v.y, this->z > v.z);
}

/*! <= operator overload
 */
template<typename T> bool3 vector3<T>::operator<=(const vector3<T>& v) const {
	return bool3(this->x <= v.x, this->y <= v.y, this->z <= v.z);
}

/*! >= operator overload
 */
template<typename T> bool3 vector3<T>::operator>=(const vector3<T>& v) const {
	return bool3(this->x >= v.x, this->y >= v.y, this->z >= v.z);
}

template<> bool vector3<bool>::any() const; // specialize for bool, makes no sense for all other types
template<typename T> bool vector3<T>::any() const {
	return false;
}

template<> bool vector3<bool>::all() const; // specialize for bool, makes no sense for all other types
template<typename T> bool vector3<T>::all() const {
	return false;
}

/*! returns a normalized vector3 object
 */
template<typename T> vector3<T> vector3<T>::normalized() const {
	return *this / length();
}

/*! normalizes the vector3 object
 */
template<typename T> vector3<T>& vector3<T>::normalize() {
	if(!is_null()) {
		*this = *this / length();
	}
	return *this;
}

/*! * operator overload / cross product
 */
template<> vector3<bool> vector3<bool>::operator^(const bool3& v) const; // specialize for bool (xor)
template<typename T> vector3<T> vector3<T>::operator^(const vector3<T>& v) const {
	return vector3<T>(this->y * v.z - this->z * v.y,
					  this->z * v.x - this->x * v.z,
					  this->x * v.y - this->y * v.x);
}

/*! * operator overload / dot product
 */
template<typename T> T vector3<T>::operator*(const vector3<T>& v) const {
	return (this->x * v.x + this->y * v.y + this->z * v.z);
}

template<typename T> void vector3<T>::clamp(const T& vmin, const T& vmax) {
	x = (x < vmin ? vmin : (x > vmax ? vmax : x));
	y = (y < vmin ? vmin : (y > vmax ? vmax : y));
	z = (z < vmin ? vmin : (z > vmax ? vmax : z));
}

template<typename T> vector3<T> vector3<T>::clamped(const T& vmax) const {
	return {
		(x < ((T)0) ? ((T)0) : (x > vmax ? vmax : x)),
		(y < ((T)0) ? ((T)0) : (y > vmax ? vmax : y)),
		(z < ((T)0) ? ((T)0) : (z > vmax ? vmax : z))
	};
}

template<typename T> vector3<T> vector3<T>::clamped(const T& vmin, const T& vmax) const {
	return {
		(x < vmin ? vmin : (x > vmax ? vmax : x)),
		(y < vmin ? vmin : (y > vmax ? vmax : y)),
		(z < vmin ? vmin : (z > vmax ? vmax : z))
	};
}

template<typename T> void vector3<T>::scale(const vector3<T>& v) {
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
}

template<typename T> vector3<T> vector3<T>::scaled(const vector3& v) const {
	return vector3<T>(x*v.x, y*v.y, z*v.z);
}

template<typename T> void vector3<T>::set(const T vx, const T vy, const T vz) {
	this->x = vx;
	this->y = vy;
	this->z = vz;
}

template<typename T> void vector3<T>::set(const vector3<T> v) {
	*this = v;
}

template<typename T> void vector3<T>::set_if(const bool3& bv, const T vx, const T vy, const T vz) {
	x = bv.x ? vx : x;
	y = bv.y ? vy : y;
	z = bv.z ? vz : z;
}

template<typename T> void vector3<T>::set_if(const bool3& bv, const vector3<T> v) {
	x = bv.x ? v.x : x;
	y = bv.y ? v.y : y;
	z = bv.z ? v.z : z;
}

template<typename T> template<typename Function> void vector3<T>::apply(Function f) {
	set(f(x), f(y), f(z));
}

template<typename T> template<typename Function> void vector3<T>::apply_if(const bool3& bv, Function f) {
	set(bv.x ? f(x) : x, bv.y ? f(y) : y, bv.z ? f(z) : z);
}

template<typename T> T vector3<T>::length() const {
	return sqrt(*this * *this);
}

/*! computes the angle between this vector and the given one, both their origin is (0, 0, 0)!
 *  @param v the other vertex/vector
 */
template<typename T> T vector3<T>::angle(const vector3<T>& v) const {
	if(is_null() || v.is_null()) return (T)0;
	// acos(<x, y> / |x|*|y|)
	return acos((v * *this) / (length() * v.length()));
}

template<typename T> T vector3<T>::distance(const vector3<T>& v) const {
	return (v - *this).length();
}

template<> vector3<float> vector3<float>::abs() const; // specialize for float
template<> vector3<bool> vector3<bool>::abs() const; // specialize for bool, no need for abs here
template<typename T> vector3<T> vector3<T>::abs() const {
	return vector3<T>((T)::llabs(x), (T)::llabs(y), (T)::llabs(z));
}

template<typename T> vector3<T>& vector3<T>::min(const vector3<T>& v) {
	*this = min(*this, v);
	return *this;
}

template<typename T> vector3<T>& vector3<T>::max(const vector3<T>& v) {
	*this = max(*this, v);
	return *this;
}

template<typename T> T vector3<T>::min_element() const {
	return x < y ? (x < z ? x : z) : (y < z ? y : z);
}

template<typename T> T vector3<T>::max_element() const {
	return x > y ? (x > z ? x : z) : (y > z ? y : z);
}

template<typename T> size_t vector3<T>::min_element_index() const {
	return (x < y && x < z) ? 0 : (y < z ? 1 : 2);
}

template<typename T> size_t vector3<T>::max_element_index() const {
	return (x > y && x > z) ? 0 : (y > z ? 1 : 2);
}

template<typename T> bool vector3<T>::is_null() const {
	return (this->x == (T)0 && this->y == (T)0 && this->z == (T)0 ? true : false);
}

template<typename T> bool vector3<T>::is_nan() const {
	if(!numeric_limits<T>::has_quiet_NaN) return false;
	
	const T nan(numeric_limits<T>::quiet_NaN());
	if(x == nan || y == nan || z == nan) {
		return true;
	}
	return false;
}

template<typename T> bool vector3<T>::is_inf() const {
	if(!numeric_limits<T>::has_infinity) return false;
	
	const T inf(numeric_limits<T>::infinity());
	if(x == inf || x == -inf || y == inf || y == -inf || z == inf || z == -inf) {
		return true;
	}
	return false;
}

template<typename T> T vector3<T>::dot(const vector3& v) const {
	return *this * v;
}
template<typename T> vector3<T> vector3<T>::cross(const vector3& v) const {
	return *this ^ v;
}

template<typename T> vector3<T> vector3<T>::mix(const vector3& v2, const T& coef) const {
	return mix(*this, v2, coef);
}

template<typename T> vector3<T> vector3<T>::faceforward(const vector3& I, const vector3& Nref) const {
	return faceforward(*this, I, Nref);
}

template<typename T> vector3<T> vector3<T>::reflect(const vector3& I) const {
	return reflect(*this, I);
}

template<typename T> vector3<T> vector3<T>::refract(const vector3& I, const T& eta) const {
	return refract(*this, I, eta);
}

template<typename T> vector3<T>& vector3<T>::rotate(const T& x_rotation, const T& y_rotation, const T& z_rotation) {
	const T sinx(sin(x_rotation));
	const T siny(sin(y_rotation));
	const T sinz(sin(z_rotation));
	const T cosx(cos(x_rotation));
	const T cosy(cos(y_rotation));
	const T cosz(cos(z_rotation));
	
	// is this correct? -- works at least for the camera stuff
	*this = {
		cosy * cosz * x + (-cosx * sinz + sinx * siny * cosz) * z + (sinx * sinz + cosx * siny * cosz) * y,
		-siny * x + sinx * cosy * z + cosx * cosy * y,
		cosy * sinz * x + (cosx * cosz + sinx * siny * sinz) * z + (-sinx * cosz + cosx * siny * sinz) * y,
	};
	return *this;
}

template<> vector3<float> vector3<float>::operator%(const float3& v) const; // specialize for float (mod)
template<> vector3<double> vector3<double>::operator%(const double3& v) const; // specialize for double (mod)
template<typename T> vector3<T> vector3<T>::operator%(const vector3<T>& v) const {
	return vector3<T>(x % v.x, y % v.y, z % v.z);
}

template<> vector3<float>& vector3<float>::operator%=(const float3& v); // specialize for float (mod)
template<> vector3<double>& vector3<double>::operator%=(const double3& v); // specialize for double (mod)
template<typename T> vector3<T>& vector3<T>::operator%=(const vector3<T>& v) {
	x %= v.x;
	y %= v.y;
	z %= v.z;
	return *this;
}

template<> vector3<float>& vector3<float>::floor();
template<typename T> vector3<T>& vector3<T>::floor() {
	x = ::floor(x);
	y = ::floor(y);
	z = ::floor(z);
	return *this;
}

template<> vector3<float>& vector3<float>::ceil();
template<typename T> vector3<T>& vector3<T>::ceil() {
	x = ::ceil(x);
	y = ::ceil(y);
	z = ::ceil(z);
	return *this;
}

template<> vector3<float>& vector3<float>::round();
template<typename T> vector3<T>& vector3<T>::round() {
	x = ::round(x);
	y = ::round(y);
	z = ::round(z);
	return *this;
}

template<> vector3<float> vector3<float>::floored() const;
template<typename T> vector3<T> vector3<T>::floored() const {
	return vector3<T>(::floor(x), ::floor(y), ::floor(z));
}

template<> vector3<float> vector3<float>::ceiled() const;
template<typename T> vector3<T> vector3<T>::ceiled() const {
	return vector3<T>(::ceil(x), ::ceil(y), ::ceil(z));
}

template<> vector3<float> vector3<float>::rounded() const;
template<typename T> vector3<T> vector3<T>::rounded() const {
	return vector3<T>(::round(x), ::round(y), ::round(z));
}

template<typename T> vector3<T> vector3<T>::sign() const {
	return vector3<T>((x < (T)0) ? ((T)-1) : ((T)1),
					  (y < (T)0) ? ((T)-1) : ((T)1),
					  (z < (T)0) ? ((T)-1) : ((T)1));
}

#if defined(A2E_EXPORT)
// only instantiate this in the vector3.cpp
extern template class vector3<float>;
extern template class vector3<double>;
extern template class vector3<unsigned int>;
extern template class vector3<int>;
extern template class vector3<short>;
extern template class vector3<unsigned short>;
extern template class vector3<char>;
extern template class vector3<unsigned char>;
//extern template class vector3<bool>;
extern template class vector3<size_t>;
extern template class vector3<ssize_t>;
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif
