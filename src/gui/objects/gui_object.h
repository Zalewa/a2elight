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

#ifndef __A2E_GUI_OBJECT_H__
#define __A2E_GUI_OBJECT_H__

#include "global.h"
#include "core/vector2.h"

// since all inheriting classes will need this, include it here:
#include "gui/style/gui_theme.h"
#include "gui/gui_event.h"

/*! @class gui_object
 *  @brief gui object element functions
 */

class engine;
class A2E_API gui_object {
public:
	gui_object(engine* e, const float2& size, const float2& position);
	virtual ~gui_object();
	
	virtual void draw() = 0;
	
	virtual void redraw();
	virtual bool needs_redraw() const;
	
	//
	virtual const float2& get_position() const;
	virtual const float2& get_position_abs() const;
	virtual const float2& get_size() const;
	virtual const float2& get_size_abs() const;
	virtual const rect& get_rectangle_abs() const;
	// use this for all abs values that need to be computed from normalized values
	virtual void compute_abs_values();
	
	virtual void set_position(const float2& position);
	virtual void set_size(const float2& size);
	
	//
	virtual void set_parent(gui_object* parent);
	virtual gui_object* get_parent() const;
	virtual void add_child(gui_object* child);
	virtual void remove_child(gui_object* child);
	virtual ipnt abs_to_rel_position(const ipnt& point) const;
	
	//
	void lock();
	bool try_lock();
	void unlock();
	
	//
	typedef std::function<void(GUI_EVENT, gui_object&)> handler;
	void add_handler(handler&& handler_, GUI_EVENT type);
	template<typename... event_types> void add_handler(handler&& handler_, event_types&&... types) {
		// unwind types, always call the simple add handler for each type
		unwind_add_handler(std::forward<handler>(handler_), std::forward<event_types>(types)...);
	}
	void remove_handlers(const GUI_EVENT& type);
	void remove_handlers();
	
	// must return true if event was handled, false if not!
	virtual bool handle_mouse_event(const EVENT_TYPE& type, const shared_ptr<event_object>& obj, const ipnt& point);
	virtual bool handle_key_event(const EVENT_TYPE& type, const shared_ptr<event_object>& obj);

protected:
	engine* e;
	gui_theme* theme;
	
	// returns true if object should be drawn, false if it shouldn't; also resets the redraw flag
	virtual bool handle_draw();
	
	// gui object element variables
	struct {
		atomic<bool> visible { true };
		atomic<bool> enabled { true };
		atomic<bool> active { false };
		atomic<bool> redraw { true };
	} state;
	
	float2 size; // normalized
	float2 size_abs; // absolute screen size
	float2 position; // normalized
	float2 position_abs; // absolute screen coordinate
	rect rectangle_abs;
	
	const string identifier = ""; // TODO: ?
	
	//
	gui_object* parent = nullptr;
	set<gui_object*> children;
	recursive_mutex mutex;
	
	//
	void unwind_add_handler(handler&& handler_, GUI_EVENT type) {
		add_handler(std::forward<handler>(handler_), type);
	}
	template<typename... event_types> void unwind_add_handler(handler&& handler_, GUI_EVENT type, event_types&&... types) {
		// unwind types, always call the simple add handler for each type
		add_handler(std::forward<handler>(handler_), type);
		unwind_add_handler(std::forward<handler>(handler_), std::forward<event_types>(types)...);
	}
	unordered_multimap<GUI_EVENT, handler> handlers;
	virtual void handle(const GUI_EVENT evt);

};

#endif
