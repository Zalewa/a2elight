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

#ifndef __A2E_CORE_H__
#define __A2E_CORE_H__

#include "global.h"

#include "core/basic_math.h"
#include "core/vector2.h"
#include "core/vector3.h"
#include "core/vector4.h"
#include "core/matrix4.h"
#include "core/file_io.h"

/*! @class core
 *  @brief core stuff
 */

class A2E_API core {
public:
	core();
	~core();
	
	// 3d math functions
	static ipnt get_2d_from_3d(const float3& vec, const matrix4f& mview, const matrix4f& mproj, const int4& viewport);
	static float3 get_3d_from_2d(const pnt& p, const matrix4f& mview, const matrix4f& mproj, const int4& viewport);
	
	static void compute_normal(const float3& v1, const float3& v2, const float3& v3, float3& normal);
	static void compute_normal_tangent_binormal(const float3& v1, const float3& v2, const float3& v3,
												float3& normal, float3& binormal, float3& tangent,
												const coord& t1, const coord& t2, const coord& t3);

	// stringstream functions
	static void reset(stringstream* sstr);

	// misc math functions
	static size_t lcm(size_t v1, size_t v2);
	static size_t gcd(size_t v1, size_t v2);
	
	static unsigned int next_pot(const unsigned int& num);
	static unsigned long long int next_pot(const unsigned long long int& num);
	
	template<typename T> static T clamp(const T& var, const T& min, const T& max) {
		return (var < min ? min : (var > max ? max : var));
	}
	
	static float wrap(const float& var, const float& max) {
		return (var <= 0.0f ? (max - fmodf(fabs(var), max)) : fmodf(var, max));
	}
	
	static int rand(const int& max);
	static int rand(const int& min, const int& max);
	static float rand(const float& max);
	static float rand(const float& min, const float& max);
	
	// string functions
	static string find_and_replace(const string& str, const string& find, const string& repl);
	static string trim(const string& str);
	static string escape_string(const string& str);
	static vector<string> tokenize(const string& src, const char& delim);
	static void str_to_lower_inplace(string& str);
	static void str_to_upper_inplace(string& str);
	static string str_to_lower(const string& str);
	static string str_to_upper(const string& str);
	
	// folder/path functions
	static map<string, file_io::FILE_TYPE> get_file_list(const string& directory, const string file_extension = "");
	static string strip_path(const string& in_path);
	
	// system functions
	static void system(const string& cmd, string& output);
	
	// misc
	struct triangle {
		float3 v1;
		float3 v2;
		float3 v3;
	};
	
	struct quad {
		float3 v1;
		float3 v2;
		float3 v3;
		float3 v4;
	};
	
protected:

};

#endif
