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

#ifndef __A2E_GUI_TOGGLE_BUTTON_HPP__
#define __A2E_GUI_TOGGLE_BUTTON_HPP__

#include "gui/objects/gui_text.hpp"

class A2E_API gui_toggle_button : public gui_object {
public:
	gui_toggle_button(engine* e, const float2& size, const float2& position);
	virtual ~gui_toggle_button();
	
	virtual void draw();
	
	virtual bool handle_mouse_event(const EVENT_TYPE& type, const shared_ptr<event_object>& obj, const ipnt& point);
	
	void set_label(const string& toggled_label, const string& untoggled_label);
	pair<const string&, const string&> get_label() const;
	
	virtual void set_toggled(const bool& state);
	virtual bool is_toggled() const;
	
protected:
	string toggled_label = "", untoggled_label = "";
	
	atomic<bool> toggled { false };
	
};

#endif
