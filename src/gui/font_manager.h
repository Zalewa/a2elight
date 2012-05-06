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

#ifndef __FONT_MANAGER_H__
#define __FONT_MANAGER_H__

#include "global.h"
#include "threading/thread_base.h"
#include "gui/font.h"

/*! @class font_manager
 *  @brief loads and caches fonts
 */

class engine;
class rtt;
struct FT_LibraryRec_;
typedef struct FT_LibraryRec_* FT_Library;
class A2E_API font_manager : public thread_base {
public:
	font_manager(engine* e);
	virtual ~font_manager();
	FT_Library get_ft_library();
	
	font& add_font(const string& identifier, const string& filename);
	font& add_font_family(const string& identifier, const vector<string> filenames);
	font* get_font(const string& identifier) const;
	bool remove_font(const string& identifier);
	
	virtual void run();

protected:
	engine* e;
	rtt* r;

	// identifier -> font object
	unordered_map<string, font*> fonts;
	FT_Library ft_library;

};

#endif
