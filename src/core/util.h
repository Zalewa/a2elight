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

#ifndef __A2E_UTIL_H__
#define __A2E_UTIL_H__

#include "core/cpp_headers.h"
#include "core/functor.h"
#include "core/type_list.h"

template<int i> struct int2type {
	enum { value = i };
};

template<typename T> struct type2type {
	typedef T orig_type;
};

template<bool flag, typename T1, typename T2> struct type_select {
	typedef T1 result;
};
template<typename T1, typename T2> struct type_select<false, T1, T2> {
	typedef T2 result;
};

// convert functions
#define A2E_CONVERT_VAR_TO_BUFFER \
stringstream buffer; \
buffer.precision(10); \
buffer.setf(ios::fixed, ios::floatfield); \
buffer << var;

template <typename type_from, typename type_to> struct converter {
	static type_to convert(const type_from& var) {
		return (type_to)var;
	}
};

template <typename type_from> struct converter<type_from, string> {
	static string convert(const type_from& var) {
		A2E_CONVERT_VAR_TO_BUFFER;
		return buffer.str();
	}
};

#if defined(__WINDOWS__) && !defined(WIN_UNIXENV)
template <> float converter<string, float>::convert(const string& var) {
	A2E_CONVERT_VAR_TO_BUFFER;
	return strtof(buffer.str().c_str(), nullptr);
}

template <> unsigned int converter<string, unsigned int>::convert(const string& var) {
	A2E_CONVERT_VAR_TO_BUFFER;
	return (unsigned int)strtoul(buffer.str().c_str(), nullptr, 10);
}

template <> int converter<string, int>::convert(const string& var) {
	A2E_CONVERT_VAR_TO_BUFFER;
	return (int)strtol(buffer.str().c_str(), nullptr, 10);
}

template <> bool converter<string, bool>::convert(const string& var) {
	return (var == "true" || var == "1" ? true : false);
}

#if defined(PLATFORM_X64)
template <> size_t converter<string, size_t>::convert(const string& var) {
	A2E_CONVERT_VAR_TO_BUFFER;
	return (size_t)strtoull(buffer.str().c_str(), nullptr, 10);
}

template <> ssize_t converter<string, ssize_t>::convert(const string& var) {
	A2E_CONVERT_VAR_TO_BUFFER;
	return (ssize_t)strtoll(buffer.str().c_str(), nullptr, 10);
}
#endif

#else
template <> float converter<string, float>::convert(const string& var);
template <> unsigned int converter<string, unsigned int>::convert(const string& var);
template <> int converter<string, int>::convert(const string& var);
template <> bool converter<string, bool>::convert(const string& var);
#if defined(A2E_IOS)
template <> unsigned long int converter<string, unsigned long int>::convert(const string& var);
#endif
#if defined(PLATFORM_X64)
template <> size_t converter<string, size_t>::convert(const string& var);
template <> ssize_t converter<string, ssize_t>::convert(const string& var);
#endif
#endif

#define string2float(value) converter<string, float>::convert(value)
#define string2uint(value) converter<string, unsigned int>::convert(value)
#define string2int(value) converter<string, int>::convert(value)
#define string2bool(value) converter<string, bool>::convert(value)
#define string2size_t(value) converter<string, size_t>::convert(value)
#define string2ssize_t(value) converter<string, ssize_t>::convert(value)
	
#define float2string(value) converter<float, string>::convert(value)
#define uint2string(value) converter<unsigned int, string>::convert(value)
#define int2string(value) converter<int, string>::convert(value)
#define bool2string(value) converter<bool, string>::convert(value)
#define size_t2string(value) converter<size_t, string>::convert(value)
#define ssize_t2string(value) converter<ssize_t, string>::convert(value)

// misc
class A2E_API a2e_exception : public exception {
protected:
	string error_str;
public:
	a2e_exception(const string& error_str_) : error_str(error_str_) {}
	virtual ~a2e_exception() throw() {}
    virtual const char* what() const throw ();
};

#define enum_class_bitwise_or(enum_class) \
friend enum_class operator|(const enum_class& e0, const enum_class& e1) { \
	return (enum_class)((typename underlying_type<enum_class>::type)e0 | \
						(typename underlying_type<enum_class>::type)e1); \
} \
friend enum_class& operator|=(enum_class& e0, const enum_class& e1) { \
	e0 = e0 | e1; \
	return e0; \
}

#define enum_class_bitwise_and(enum_class) \
friend enum_class operator&(const enum_class& e0, const enum_class& e1) { \
	return (enum_class)((typename underlying_type<enum_class>::type)e0 & \
						(typename underlying_type<enum_class>::type)e1); \
} \
friend enum_class& operator&=(enum_class& e0, const enum_class& e1) { \
	e0 = e0 & e1; \
	return e0; \
}

#endif
