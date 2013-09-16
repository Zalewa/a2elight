/*
 *  Albion 2 Engine "light"
 *  Copyright (C) 2004 - 2013 Florian Ziesche
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

#ifndef __A2E_GUI_HPP__
#define __A2E_GUI_HPP__

#include "global.hpp"
#include "threading/thread_base.hpp"
#include "rendering/rtt.hpp"
#include "core/event.hpp"
#include "gui/objects/gui_object.hpp"
#include "gui/style/gui_surface.hpp"

/*! @class gui
 *  @brief graphical user interface functions
 */

class engine;
class core;
class scene;
class font_manager;
class shader;
class shader_gl3;
typedef shared_ptr<shader_gl3> gl3shader;
class gui_theme;
class gui_window;

enum class DRAW_MODE_UI : unsigned int {
	PRE_UI,
	POST_UI
};
typedef functor<void, const DRAW_MODE_UI, rtt::fbo*> ui_draw_callback;

class A2E_API gui : public thread_base {
public:
	gui(engine* e, const string& theme_name);
	~gui();

	void draw();
	
	//
	font_manager* get_font_manager() const;
	gui_theme* get_theme() const;
	
	bool set_clipboard_text(const string& text);
	const string& get_clipboard_text() const;
	
	// misc flags
	void set_keyboard_input(const bool& state);
	bool get_keyboard_input() const;
	void set_mouse_input(const bool& state);
	bool get_mouse_input() const;
	
	//
	void set_active_object(gui_object* obj);
	gui_object* get_active_object() const;
	
	// draw callbacks
	gui_simple_callback* add_draw_callback(const DRAW_MODE_UI& mode, ui_draw_callback& cb,
										   const float2& size, const float2& offset,
										   const gui_surface::SURFACE_FLAGS flags = gui_surface::SURFACE_FLAGS::NONE);
	void delete_draw_callback(ui_draw_callback& cb);
	
	// adding/creating gui objects:
	template <class gui_class, typename... Args> gui_class* add(Args&&... args);
	template <class gui_class> void remove(gui_class* obj);
	
	// any gui object creation or deletion (or operation on the window container) should be locked
	void lock();
	bool try_lock();
	void unlock();
	
	//
	const rtt::fbo* get_fullscreen_fbo() const;

protected:
	engine* e;
	event* evt;
	rtt* r;
	shader* s;
	scene* sce;
	font_manager* fm;
	gui_theme* theme;
	
	// note: this must be ordered
	array<vector<ui_draw_callback*>, 2> draw_callbacks; // pre and post
	unordered_map<ui_draw_callback*, gui_simple_callback*> cb_surfaces;
	
	rtt::fbo main_fbo;
	rtt::fbo* aa_fbo = nullptr;
	void recreate_buffers(const size2 size);
	void delete_buffers();
	
	void reload_shaders();
	gl3shader blend_shd;
	gl3shader texture_shd;
	
	//
	gui_object* active_object = nullptr;
	deque<gui_window*> windows;
	void add_window(gui_window* wnd);
	void remove_window(gui_window* wnd);
	recursive_mutex object_lock;
	
	// thread run:
	virtual void run();
	
	// event handling
	event::handler key_handler_fnctr;
	event::handler mouse_handler_fnctr;
	event::handler shader_reload_fnctr;
	event::handler window_handler_fnctr;
	bool key_handler(EVENT_TYPE type, shared_ptr<event_object> obj);
	bool mouse_handler(EVENT_TYPE type, shared_ptr<event_object> obj);
	bool shader_reload_handler(EVENT_TYPE type, shared_ptr<event_object> obj);
	bool window_handler(EVENT_TYPE type, shared_ptr<event_object> obj);
	
	//
	vector<pair<EVENT_TYPE, shared_ptr<event_object>>> event_queue, event_processing_queue;
	mutex event_lock;
	atomic<bool> events_in_queue { false };
	string clipboard_text = "";
	
	atomic<bool> keyboard_input { true };
	atomic<bool> mouse_input { true };

};

//
template <class gui_class, typename... Args> gui_class* gui::add(Args&&... args) {
	// make sure, gui_class is inheriting from gui_object
	static_assert(is_base_of<gui_object, gui_class>::value,
				  "can't add an object that doesn't inherit from gui_object!");
	
	lock();
	gui_class* obj = new gui_class(e, forward<Args>(args)...);
	
	// "specialize" for gui_window, since we'll need to keep track of them:
	// (also: i wish there'd be a static if ...)
	if(is_same<gui_window, gui_class>::value ||
	   is_base_of<gui_window, gui_class>::value) {
		add_window((gui_window*)obj);
	}
	
	unlock();
	return obj;
}

template <class gui_class> void gui::remove(gui_class* obj) {
	if(is_same<gui_window, gui_class>::value ||
	   is_base_of<gui_window, gui_class>::value) {
		lock();
		remove_window((gui_window*)obj);
		unlock();
	}
}

#endif
