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

#ifndef __VECTOR_4_H__
#define __VECTOR_4_H__

#include "core/cpp_headers.h"
#include "core/vector3.h"

template <typename T> class vector4;
typedef vector4<float> float4;
typedef vector4<double> double4;
typedef vector4<unsigned char> uchar4;
typedef vector4<unsigned short> ushort4;
typedef vector4<unsigned int> uint4;
typedef vector4<unsigned int> index4;
typedef vector4<char> char4;
typedef vector4<short> short4;
typedef vector4<int> int4;
typedef vector4<bool> bool4;
typedef vector4<size_t> size4;
typedef vector4<ssize_t> ssize4;

template <typename T> class A2E_API __attribute__((packed)) vector4 : public vector3<T> {
public:
	union {
		T w, a;
	};
	
	vector4() : vector3<T>((T)0, (T)0, (T)0), w((T)0) {}
	vector4(const vector3<T>& vec3) : vector3<T>(vec3.x, vec3.y, vec3.z), w((T)0) {}
	vector4(const vector3<T>& vec3, const T& vw) : vector3<T>(vec3.x, vec3.y, vec3.z), w(vw) {}
	vector4(const vector4<T>& vec4) : vector3<T>(vec4.x, vec4.y, vec4.z), w(vec4.w) {}
	vector4(const T& vx, const T& vy, const T& vz, const T& vw) : vector3<T>(vx, vy, vz), w(vw) {}
	vector4(const T& f) : vector3<T>(f, f, f), w(f) {}
	~vector4() {}
	
	// overloading routines
	T& operator[](size_t index);
	const T& operator[](size_t index) const;
	vector4& operator=(const vector4& v);
	bool operator==(const vector4& v);
	bool operator!=(const vector4& v);
	
	friend ostream& operator<<(ostream& output, const vector4<T>& v) {
		output << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		return output;
	}
	
	void set(const T& vx, const T& vy, const T& vz, const T& vw);
	void set(const vector4& v);
	void scale(const T& f);
	void scale(const vector4& v);
	
	template<size_t comp1, size_t comp2, size_t comp3, size_t comp4> vector4 swizzle() const {
		return vector4(((T*)this)[comp1], ((T*)this)[comp2], ((T*)this)[comp3], ((T*)this)[comp4]);
	}
		
	// a component-wise minimum between two vector4s
	static const vector4 min(const vector4& v1, const vector4& v2) {
		return vector4(std::min(v1.x, v2.x), std::min(v1.y, v2.y), 
					   std::min(v1.z, v2.z), std::min(v1.w, v2.w));
	}
	
	// a component-wise maximum between two vector4s
	static const vector4 max(const vector4& v1, const vector4& v2) {
		return vector4(std::max(v1.x, v2.x), std::max(v1.y, v2.y), 
					   std::max(v1.z, v2.z), std::max(v1.w, v2.w));
	}
	
	T dot(const vector4& v) const {
		return (this->x * v.x + this->y * v.y + this->z * v.z + this->w * v.w);
	}
	
	static const vector4 mix(const vector4& v1, const vector4& v2, const T& coef) {
		return vector4(v1.x * coef + v2.x * ((T)1 - coef),
					   v1.y * coef + v2.y * ((T)1 - coef),
					   v1.z * coef + v2.z * ((T)1 - coef),
					   v1.w * coef + v2.w * ((T)1 - coef));
	}
	
	// cross equals the vector3 cross function
	
	// no need for a separate "+-*/" vector4 definition at the moment

};

#if defined(A2E_EXPORT)
// only instantiate this in the vector4.cpp
extern template class vector4<float>;
extern template class vector4<unsigned int>;
extern template class vector4<int>;
extern template class vector4<short>;
extern template class vector4<char>;
extern template class vector4<unsigned short>;
extern template class vector4<unsigned char>;
extern template class vector4<bool>;
extern template class vector4<size_t>;
extern template class vector4<ssize_t>;
#endif

////////////////////////////////////////////////////////////////////////////////
// additional vector types
template <typename T> class vector8 {
public:
	vector4<T> lo, hi;
	
	vector8() : lo(), hi() {}
	vector8(const vector8<T>& vec8) : lo(vec8.lo), hi(vec8.hi) {}
	vector8(const vector4<T>& vec4_lo, const vector4<T>& vec4_hi) : lo(vec4_lo), hi(vec4_hi) {}
	~vector8() {}
	
	T& operator[](size_t index) {
		return index < 4 ? lo[index] : hi[index-4];
	}
	
	friend ostream& operator<<(ostream& o, const vector8<T>& v) {
		o << "(";
		o << v.lo[0] << ", " << v.lo[1] << ", " << v.lo[2] << ", " << v.lo[3] << ", " << v.hi[0] << ", " << v.hi[1] << ", " << v.hi[2] << ", " << v.hi[3];
		o << ")";
		return o;
	}
};
typedef vector8<float> float8;
typedef vector8<unsigned int> uint8;
typedef vector8<int> int8;

template <typename T> class vector16 {
public:
	vector8<T> lo, hi;
	
	vector16() : lo(), hi() {}
	vector16(const vector16<T>& vec16) : lo(vec16.lo), hi(vec16.hi) {}
	vector16(const vector8<T>& vec8_lo, const vector8<T>& vec8_hi) : lo(vec8_lo), hi(vec8_hi) {}
	vector16(const float* floats) {
		lo.lo = float4(floats[0], floats[1], floats[2], floats[3]);
		lo.hi = float4(floats[4], floats[5], floats[6], floats[7]);
		hi.lo = float4(floats[8], floats[9], floats[10], floats[11]);
		hi.hi = float4(floats[12], floats[13], floats[14], floats[15]); 
	}
	~vector16() {}
	
	T& operator[](size_t index) {
		return index < 8 ? lo[index] : hi[index-8];
	}
	
	friend ostream& operator<<(ostream& o, const vector16<T>& v) {
		o << "(";
		o << v.lo.lo[0] << ", " << v.lo.lo[1] << ", " << v.lo.lo[2] << ", " << v.lo.lo[3] << ", " << v.lo.hi[0] << ", " << v.lo.hi[1] << ", " << v.lo.hi[2] << ", " << v.lo.hi[3] << ", "
		<< v.hi.lo[0] << ", " << v.hi.lo[1] << ", " << v.hi.lo[2] << ", " << v.hi.lo[3] << ", " << v.hi.hi[0] << ", " << v.hi.hi[1] << ", " << v.hi.hi[2] << ", " << v.hi.hi[3];
		o << ")";
		return o;
	}
};
typedef vector16<float> float16;
typedef vector16<unsigned int> uint16;
typedef vector16<int> int16;
////////////////////////////////////////////////////////////////////////////////

template<typename T> T& vector4<T>::operator[](size_t index) {
	return ((T*)this)[index];
}

template<typename T> const T& vector4<T>::operator[](size_t index) const {
	return ((T*)this)[index];
}

/*! = operator overload
 */
template<typename T> vector4<T>& vector4<T>::operator=(const vector4<T>& v) {
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = v.w;
	return *this;
}

/*! == operator overload
 */
template<typename T> bool vector4<T>::operator==(const vector4<T>& v) {
	if(this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w) {
		return true;
	}
	return false;
}

/*! != operator overload
 */
template<typename T> bool vector4<T>::operator!=(const vector4<T>& v) {
	return (*this == v) ^ true;
}

template<typename T> void vector4<T>::set(const T& vx, const T& vy, const T& vz, const T& vw) {
	this->x = vx;
	this->y = vy;
	this->z = vz;
	this->w = vw;
}

template<typename T> void vector4<T>::set(const vector4<T>& v) {
	*this = v;
}

template<typename T> void vector4<T>::scale(const T& f) {
	this->x *= f;
	this->y *= f;
	this->z *= f;
	this->w *= f;
}

template<typename T> void vector4<T>::scale(const vector4<T>& v) {
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	this->w *= v.w;
}

#endif
