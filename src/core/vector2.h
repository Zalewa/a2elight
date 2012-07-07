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

#ifndef __A2E_VECTOR_2_H__
#define __A2E_VECTOR_2_H__

#include "core/cpp_headers.h"

template <typename T> class vector2;
typedef vector2<float> float2;
typedef vector2<double> double2;
typedef vector2<unsigned int> uint2;
typedef vector2<int> int2;
typedef vector2<short> short2;
typedef vector2<bool> bool2;
typedef vector2<size_t> size2;
typedef vector2<ssize_t> ssize2;

typedef vector2<unsigned int> pnt;
typedef vector2<int> ipnt;
typedef vector2<float> coord;

template <typename T> class A2E_API __attribute__((packed, aligned(4))) vector2 {
public:
	union {
		T x, u;
	};
	union {
		T y, v;
	};
	
	constexpr vector2() noexcept : x((T)0), y((T)0) {}
	constexpr vector2(const vector2<T>& vec2) noexcept : x(vec2.x), y(vec2.y) {}
	constexpr vector2(const pair<T, T>& vec2) noexcept : x(vec2.first), y(vec2.second) {}
	constexpr vector2(const T& vx, const T& vy) noexcept : x(vx), y(vy) {}
	constexpr vector2(const T& f) noexcept : x(f), y(f) {}
	template <typename U> constexpr vector2(const vector2<U>& vec2) noexcept : x((T)vec2.x), y((T)vec2.y) {}
	
	T& operator[](size_t index) const {
		return ((T*)this)[index];
	}
	
	friend ostream& operator<<(ostream& output, const vector2<T>& vec2) {
		output << "(" << vec2.x << ", " << vec2.y << ")";
		return output;
	}
	
	void set(const T& vx, const T& vy) {
		x = vx;
		y = vy;
	}
	void set(const vector2& vec2) {
		x = vec2.x;
		y = vec2.y;
	}
	
	vector2& round();
	vector2& normalize() {
		if(!is_null()) {
			*this = *this / length();
		}
		return *this;
	}

	// TODO: fully integrate ...
	
	friend vector2 operator*(const T& f, const vector2& v) {
		return vector2<T>(f * v.x, f * v.y);
	}
	vector2 operator*(const T& f) const {
		return vector2<T>(f * this->x, f * this->y);
	}
	vector2 operator/(const T& f) const {
		return vector2<T>(this->x / f, this->y / f);
	}
	
	vector2 operator-(const vector2<T>& vec2) const {
		return vector2<T>(this->x - vec2.x, this->y - vec2.y);
	}
	vector2 operator+(const vector2<T>& vec2) const {
		return vector2<T>(this->x + vec2.x, this->y + vec2.y);
	}
	vector2 operator*(const vector2<T>& vec2) const {
		return vector2<T>(this->x * vec2.x, this->y * vec2.y);
	}
	vector2 operator/(const vector2<T>& vec2) const {
		return vector2<T>(this->x / vec2.x, this->y / vec2.y);
	}
	
	vector2& operator+=(const vector2& v) {
		*this = *this + v;
		return *this;
	}
	vector2& operator-=(const vector2& v) {
		*this = *this - v;
		return *this;
	}
	vector2& operator*=(const vector2& v) {
		*this = *this * v;
		return *this;
	}
	vector2& operator/=(const vector2& v) {
		*this = *this / v;
		return *this;
	}
	vector2 operator%(const vector2& v) const;
	vector2& operator%=(const vector2& v);
	
	T length() const {
		return sqrt(dot());
	}

	T distance(const vector2<T>& vec2) const {
		return (vec2 - *this).length();
	}
	
	T dot(const vector2<T>& vec2) const {
		return x*vec2.x + y*vec2.y;
	}
	T dot() const {
		return dot(*this);
	}
	
	bool is_null() const;
	bool is_nan() const;
	bool is_inf() const;
	
	vector2 abs() const;
	
	vector2<T>& clamp(const T& vmin, const T& vmax) {
		x = (x < vmin ? vmin : (x > vmax ? vmax : x));
		y = (y < vmin ? vmin : (y > vmax ? vmax : y));
		return *this;
	}
	vector2<T>& clamp(const vector2<T>& vmin, const vector2<T>& vmax) {
		x = (x < vmin.x ? vmin.x : (x > vmax.x ? vmax.x : x));
		y = (y < vmin.y ? vmin.y : (y > vmax.y ? vmax.y : y));
		return *this;
	}
	vector2<T> sign() const {
		return vector2<T>(x < 0.0f ? -1.0f : 1.0f, y < 0.0f ? -1.0f : 1.0f);
	}
	
	// a component-wise minimum between two vector2s
	static const vector2 min(const vector2& v1, const vector2& v2) {
		return vector2(std::min(v1.x, v2.x), std::min(v1.y, v2.y));
	}
	
	// a component-wise maximum between two vector2s
	static const vector2 max(const vector2& v1, const vector2& v2) {
		return vector2(std::max(v1.x, v2.x), std::max(v1.y, v2.y));
	}
	
};

struct rect {
	union {
		struct {
			unsigned int x1;
			unsigned int y1;
			unsigned int x2;
			unsigned int y2;
		};
		// TODO: find a way to make gcc support this ...
		/*struct {
			uint2 low;
			uint2 high;
		};*/
	};
	
	void set(const unsigned int& x1_, const unsigned int& y1_, const unsigned int& x2_, const unsigned int& y2_) {
		x1 = x1_; y1 = y1_; x2 = x2_; y2 = y2_;
	}
	
	friend ostream& operator<<(ostream& output, const rect& r) {
		output << "(" << r.x1 << ", " << r.y1 << ") x (" << r.x2 << ", " << r.y2 << ")";
		return output;
	}
	
	rect() : x1(0), y1(0), x2(0), y2(0) {}
	rect(const rect& r) : x1(r.x1), y1(r.y1), x2(r.x2), y2(r.y2) {}
	rect(const unsigned int& x1_, const unsigned int& y1_, const unsigned int& x2_, const unsigned int& y2_) : x1(x1_), y1(y1_), x2(x2_), y2(y2_) {}
	//rect(const uint2& low_, const uint2& high_) : low(low_), high(high_) {}
	~rect() {}
};

template<> vector2<float> vector2<float>::operator%(const vector2& v) const; // specialize for float (mod)
template<> vector2<double> vector2<double>::operator%(const vector2& v) const; // specialize for double (mod)
template<typename T> vector2<T> vector2<T>::operator%(const vector2<T>& v) const {
	return vector2<T>(x % v.x, y % v.y);
}

template<> vector2<float>& vector2<float>::operator%=(const vector2& v); // specialize for float (mod)
template<> vector2<double>& vector2<double>::operator%=(const vector2& v); // specialize for double (mod)
template<typename T> vector2<T>& vector2<T>::operator%=(const vector2<T>& v) {
	x %= v.x;
	y %= v.y;
	return *this;
}

template<> vector2<float>& vector2<float>::round();
template<typename T> vector2<T>& vector2<T>::round() {
	x = ::round(x);
	y = ::round(y);
	return *this;
}

template<typename T> bool vector2<T>::is_null() const {
	return (this->x == (T)0 && this->y == (T)0 ? true : false);
}

template<typename T> bool vector2<T>::is_nan() const {
	if(!numeric_limits<T>::has_quiet_NaN) return false;
	
	T nan = numeric_limits<T>::quiet_NaN();
	if(x == nan || y == nan) {
		return true;
	}
	return false;
}

template<typename T> bool vector2<T>::is_inf() const {
	if(!numeric_limits<T>::has_infinity) return false;
	
	T inf = numeric_limits<T>::infinity();
	if(x == inf || x == -inf || y == inf || y == -inf) {
		return true;
	}
	return false;
}

template<> vector2<float> vector2<float>::abs() const; // specialize for float
template<> vector2<bool> vector2<bool>::abs() const; // specialize for bool, no need for abs here
template<typename T> vector2<T> vector2<T>::abs() const {
	return vector2<T>((T)::llabs(x), (T)::llabs(y));
}

#if defined(A2E_EXPORT)
// only instantiate this in the vector2.cpp
extern template class vector2<float>;
extern template class vector2<double>;
extern template class vector2<unsigned int>;
extern template class vector2<int>;
extern template class vector2<short>;
extern template class vector2<bool>;
extern template class vector2<size_t>;
extern template class vector2<ssize_t>;
#endif

#endif
