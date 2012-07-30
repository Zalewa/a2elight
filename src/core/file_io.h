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

#ifndef __A2E_FILE_IO_H__
#define __A2E_FILE_IO_H__

#include "global.h"

#include "gui/unicode.h"

/*! @class file_io
 *  @brief file input/output
 */

class A2E_API file_io {
public:
	enum class OPEN_TYPE {
		READ,
		READWRITE,
		WRITE,
		READ_BINARY,
		READWRITE_BINARY,
		WRITE_BINARY,
		APPEND,
		APPEND_BINARY,
		APPEND_READ,
		APPEND_READ_BINARY
	};
	
	file_io();
	file_io(const string& filename, OPEN_TYPE open_type = OPEN_TYPE::READWRITE_BINARY);
	~file_io();
	
	enum class FILE_TYPE : unsigned int {
		NONE,
		DIR,
		ALL,
		IMAGE,
		A2E_MODEL,
		A2E_ANIMATION,
		A2E_MATERIAL,
		A2E_MAP,
		A2E_UI,
		A2E_LIST,
		A2E_SHADER,
		XML,
		TEXT,
		OPENCL
	};

	bool open(const string& filename, OPEN_TYPE open_type);
	bool file_to_buffer(const string& filename, stringstream& buffer);
	void close();
	uint64_t get_filesize();
	fstream* get_filestream();
	void seek(size_t offset);
	streampos get_current_offset();
	
	// file input:
	void read_file(stringstream* buffer);
	void get_line(char* finput, unsigned int length);
	void get_block(char* data, size_t size);
	void get_terminated_block(string& str, const char terminator);
	char get_char();
	unsigned short int get_usint();
	unsigned int get_uint();
	float get_float();
	void seek_read(size_t offset);
	streampos get_current_read_offset();
	
	// file output:
	void write_block(const char* data, size_t size, bool check_size = false);
	void write_terminated_block(const string& str, const char terminator);
	void write_char(const unsigned char& ch);
	void write_uint(const unsigned int& ui);
	void write_float(const float& f);
	void seek_write(size_t offset);
	streampos get_current_write_offset();


	//
	static bool is_file(const string& filename);
	bool eof() const;
	bool good() const;
	bool fail() const;
	bool bad() const;
	bool is_open() const;

protected:
	OPEN_TYPE open_type = OPEN_TYPE::READ;
	fstream filestream;

	bool check_open();

};

#endif
