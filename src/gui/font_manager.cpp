/*
 *  Albion 2 Engine "light"
 *  Copyright (C) 2004 - 2014 Florian Ziesche
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

#include "font_manager.hpp"
#include "font.hpp"
#include "engine.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

font_manager::font_manager() :
thread_base("font_manager"),
r(engine::get_rtt()), fonts(), ft_library(nullptr) {
	// init freetype
	if(FT_Init_FreeType(&ft_library) != 0) {
		log_error("failed to initialize freetype library!");
	}
	
#if !defined(FLOOR_IOS)
	// these should always exist (desktop only)
	add_font_family("DEJAVU_SANS_SERIF", vector<string> {
		floor::data_path("fonts/DejaVuSans.ttf"),
		floor::data_path("fonts/DejaVuSans-Oblique.ttf"),
		floor::data_path("fonts/DejaVuSans-Bold.ttf"),
		floor::data_path("fonts/DejaVuSans-BoldOblique.ttf")
	});
	add_font_family("DEJAVU_SERIF", vector<string> {
		floor::data_path("fonts/DejaVuSerif.ttf"),
		floor::data_path("fonts/DejaVuSerif-Italic.ttf"),
		floor::data_path("fonts/DejaVuSerif-Bold.ttf"),
		floor::data_path("fonts/DejaVuSerif-BoldItalic.ttf")
	});
	add_font_family("DEJAVU_MONOSPACE", vector<string> {
		floor::data_path("fonts/DejaVuSansMono.ttf"),
		floor::data_path("fonts/DejaVuSansMono-Oblique.ttf"),
		floor::data_path("fonts/DejaVuSansMono-Bold.ttf"),
		floor::data_path("fonts/DejaVuSansMono-BoldOblique.ttf")
	});
#endif
	
	// TODO: add system font overrides to config
#if defined(__APPLE__) && !defined(FLOOR_IOS)
	add_font_family("SYSTEM_SANS_SERIF", vector<string> {
		floor::data_path("fonts/DejaVuSans.ttf"),
		floor::data_path("fonts/DejaVuSans-Oblique.ttf"),
		floor::data_path("fonts/DejaVuSans-Bold.ttf"),
		floor::data_path("fonts/DejaVuSans-BoldOblique.ttf")
	});
	add_font("SYSTEM_SERIF", "/System/Library/Fonts/Times.dfont");
	add_font("SYSTEM_MONOSPACE", "/System/Library/Fonts/Menlo.ttc");
#else // linux/*bsd/x11/windows/iOS
	add_font_family("SYSTEM_SANS_SERIF", vector<string> {
		floor::data_path("fonts/DejaVuSans.ttf"),
		floor::data_path("fonts/DejaVuSans-Oblique.ttf"),
		floor::data_path("fonts/DejaVuSans-Bold.ttf"),
		floor::data_path("fonts/DejaVuSans-BoldOblique.ttf")
	});
	add_font_family("SYSTEM_SERIF", vector<string> {
		floor::data_path("fonts/DejaVuSerif.ttf"),
		floor::data_path("fonts/DejaVuSerif-Italic.ttf"),
		floor::data_path("fonts/DejaVuSerif-Bold.ttf"),
		floor::data_path("fonts/DejaVuSerif-BoldItalic.ttf")
	});
	add_font_family("SYSTEM_MONOSPACE", vector<string> {
		floor::data_path("fonts/DejaVuSansMono.ttf"),
		floor::data_path("fonts/DejaVuSansMono-Oblique.ttf"),
		floor::data_path("fonts/DejaVuSansMono-Bold.ttf"),
		floor::data_path("fonts/DejaVuSansMono-BoldOblique.ttf")
	});
#endif
	
	// start actual font_manager thread after everything has been initialized
	this->set_thread_delay(50);
	this->start();
}

font_manager::~font_manager() {
	log_debug("deleting font_manager object");
	
	// delete all fonts
	for(const auto& fnt : fonts) {
		delete fnt.second;
	}
	fonts.clear();
	
	// free freetype
	if(FT_Done_FreeType(ft_library) != 0) {
		log_error("failed to free freetype library!");
	}

	log_debug("font_manager object deleted");
}

void font_manager::run() {
}

FT_Library font_manager::get_ft_library() {
	return ft_library;
}

a2e_font* font_manager::add_font(const string& identifier, const string& filename) {
	return add_font_family(identifier, vector<string> { filename });
}

a2e_font* font_manager::add_font_family(const string& identifier, vector<string>&& filenames) {
	const auto iter = fonts.find(identifier);
	if(iter != fonts.end()) {
		return iter->second;
	}
	
	const auto ret = fonts.emplace(identifier, new a2e_font(this, move(filenames)));
	return ret.first->second;
}

bool font_manager::remove_font(const string& identifier) {
	const auto iter = fonts.find(identifier);
	if(iter == fonts.end()) {
		log_error("invalid font %s!", identifier);
		return false;
	}
	
	delete iter->second;
	fonts.erase(iter);
	return true;
}

a2e_font* font_manager::get_font(const string& identifier) const {
	const auto iter = fonts.find(identifier);
	if(iter != fonts.end()) {
		return iter->second;
	}
	return nullptr;
}
