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

#include "gui_pop_up_button.h"
#include "gui_window.h"
#include "engine.h"
#include "event.h"
#include "gui.h"
#include "task.h"
#include "font.h"

////
class gui_pop_up_window : public gui_window {
public:
	gui_pop_up_window(engine* e, const float2& size, const float2& position,
					  gui_pop_up_button* pop_up_button_,
					  const float2& button_position_abs_, const float2& button_size_abs_,
					  const vector<pair<const string, string>*>& items_)
	: gui_window(e, size, position), pop_up_button(pop_up_button_),
	button_position_abs(button_position_abs_), button_size_abs(button_size_abs_), items(items_) {
	}
	virtual ~gui_pop_up_window() {
		for(auto& cached_item : item_text_cache) {
			font::destroy_text_cache(cached_item.second.first);
		}
	}
	virtual void draw();
	
	virtual bool handle_mouse_event(const EVENT_TYPE& type, const shared_ptr<event_object>& obj, const ipnt& point);
	
protected:
	gui_pop_up_button* pop_up_button;
	const float2 button_position_abs;
	const float2 button_size_abs;
	const vector<pair<const string, string>*>& items;
	// item ptr -> <cachce, item height>
	unordered_map<pair<const string, string>*, pair<font::text_cache, float2>> item_text_cache;
	
	const float margin = 8.0f; // TODO: 4pt
	float2 overlay_size;
	float2 overlay_position;
	
};

void gui_pop_up_window::draw() {
	if(!state.visible) return;
	// TODO: only redraw when necessary
	
	start_draw();
	r->clear();
	r->start_2d_draw();
	
	// cache items
	for(const auto& item : items) {
		if(item_text_cache.find(item) == item_text_cache.end()) {
			const auto advance_map(fnt->compute_advance_map(item->second, 1));
			const float height = (advance_map.back().x < advance_map.back().y ?
								  (advance_map.back().y - advance_map.back().x) :
								  (advance_map.back().x + advance_map.back().y));
			item_text_cache.insert(make_pair(item,
											 make_pair(fnt->cache_text(item->second),
													   float2(height, 0.0f))));
		}
	}
	
	//
	const float item_margin = 8.0f; // TODO: 4pt
	const float2 text_margin(10.0f, -6.0f); // 5pt, -3pt (TODO: dpi)
	
	const auto selected_item = pop_up_button->get_selected_item();
	float total_height = margin * 2.0f;
	float selected_item_height = 0.0f;
	for(const auto& item : items) {
		if(item == selected_item) {
			selected_item_height = total_height;
		}
		total_height += ceilf(item_text_cache.at(item).second.x + item_margin * 2.0f);
	}
	
	// compute overlay position and size
	overlay_size = float2(button_size_abs.x, total_height);
	
	float2 offset(button_position_abs - float2(margin, 0.0f));
	offset.y -= selected_item_height - margin;
	// clamp to (0, window size - overlay size) to prevent drawing outside of the window
	offset.clamp(float2(0.0f), float2(buffer->width, buffer->height) - overlay_size);
	overlay_position = offset;
	
	// draw overlay rectangle
	theme->draw("pop_up_button", "overlay",
				overlay_position, overlay_size,
				false);
	
	// draw items
	const pnt mouse_pos(gui_object::e->get_event()->get_mouse_pos());
	float y_offset = margin + overlay_position.y;
	const float4 font_color(theme->get_color_scheme().get("TEXT_INVERSE"));
	const float4 font_color_active(theme->get_color_scheme().get("TEXT"));
	for(const auto& item : items) {
		//
		auto& cached_item(item_text_cache.at(item));
		const float text_height = cached_item.second.x; // text y-extent
		const float item_height = ceilf(text_height + item_margin * 2.0f);
		const float2 item_position(overlay_position.x, y_offset);
		const float2 item_size(button_size_abs.x, item_height);
		cached_item.second.y = item_height; // cache item height
		y_offset += item_height;
		
		if(mouse_pos.x > item_position.x && mouse_pos.x <= (item_position.x + item_size.x) &&
		   mouse_pos.y > item_position.y && mouse_pos.y <= (item_position.y + item_size.y)) {
			// active item
			theme->draw("pop_up_button", "item_active",
						item_position, item_size, false);
			fnt->draw_cached(cached_item.first.first.x, cached_item.first.first.y, item_position + text_margin,
							 font_color_active);
		}
		else {
			fnt->draw_cached(cached_item.first.first.x, cached_item.first.first.y, item_position + text_margin,
							 font_color);
		}
	}
	
	r->stop_2d_draw();
	stop_draw();
}

bool gui_pop_up_window::handle_mouse_event(const EVENT_TYPE& type, const shared_ptr<event_object>& obj a2e_unused, const ipnt& point) {
	// handle select
	if(type == EVENT_TYPE::MOUSE_LEFT_UP &&
	   point.x >= 0 &&
	   point.x > overlay_position.x &&
	   point.x <= (overlay_position.x + overlay_size.x)) {
		const float ypos(point.y - overlay_position.y - margin);
		float height = 0.0f;
		for(const auto& item : items) {
			const auto& cached_item(item_text_cache.at(item));
			if(ypos > height && ypos <= (height + cached_item.second.y)) {
				pop_up_button->set_selected_item(item->first);
				break;
			}
			height += cached_item.second.y;
		}
		// close will happen shortly after, but we don't it to be drawn any longer -> make invisible
		set_visible(false);
		pop_up_button->set_active(false);
		return true;
	}
	else if((type & EVENT_TYPE::__MOUSE_EVENT) == EVENT_TYPE::__MOUSE_EVENT &&
			type != EVENT_TYPE::MOUSE_MOVE &&
			type != EVENT_TYPE::MOUSE_WHEEL_DOWN &&
			type != EVENT_TYPE::MOUSE_WHEEL_UP) {
		// close overlay on all mouse events (except for move and mouse wheel events)
		set_visible(false);
		pop_up_button->set_active(false);
		return true;
	}
	return false;
}

////
gui_pop_up_button::gui_pop_up_button(engine* e_, const float2& size_, const float2& position_) :
gui_object(e_, size_, position_) {
}

gui_pop_up_button::~gui_pop_up_button() {
}

void gui_pop_up_button::draw() {
	if(!gui_object::handle_draw()) return;
	
	// TODO: handle disabled state
	theme->draw("pop_up_button", state.active ? "active" : "normal",
				position_abs, size_abs, true,
				[this](const string& str a2e_unused) -> string {
					if(selected_item == nullptr) return "";
					return selected_item->second;
				});
}


bool gui_pop_up_button::handle_mouse_event(const EVENT_TYPE& type, const shared_ptr<event_object>& obj a2e_unused, const ipnt& point a2e_unused) {
	if(!state.visible || !state.enabled) return false;
	switch(type) {
		case EVENT_TYPE::MOUSE_LEFT_DOWN:
			set_active(true);
			return true;
		// mouse up / window close will be handled by gui_pop_up_window (or gui)
		default: break;
	}
	return false;
}

void gui_pop_up_button::set_active(const bool& active_state) {
	if(active_state == state.active) return;
	gui_object::set_active(active_state);
	
	if(active_state) {
		handle(GUI_EVENT::POP_UP_BUTTON_ACTIVATION);
		open_selection_wnd();
	}
	else {
		handle(GUI_EVENT::POP_UP_BUTTON_DEACTIVATION);
		close_selection_wnd();
	}
}

void gui_pop_up_button::open_selection_wnd() {
	task::spawn([this]() {
		e->acquire_gl_context();
		ui->lock();
		selection_wnd = ui->add<gui_pop_up_window>(float2(1.0f), float2(0.0f), this,
												   rel_to_abs_position(position_abs), size_abs,
												   display_items);
		ui->unlock();
		e->release_gl_context();
	});
}

void gui_pop_up_button::close_selection_wnd() {
	if(selection_wnd == nullptr) return;
	task::spawn([this]() {
		e->acquire_gl_context();
		ui->lock();
		ui->remove(selection_wnd);
		selection_wnd = nullptr;
		ui->unlock();
		e->release_gl_context();
	});
}

void gui_pop_up_button::clear() {
	display_items.clear();
	items.clear();
	redraw();
}

void gui_pop_up_button::add_item(const string& identifier, const string& label) {
	if(items.count(identifier) > 0) {
		a2e_error("an item with the identifier \"%s\" already exists!", identifier);
		return;
	}
	const auto iter = items.insert(make_pair(identifier, label));
	display_items.push_back(&*iter.first);
	if(display_items.size() == 1) {
		// set selected item and redraw, if this is the first item
		selected_item = display_items[0];
		redraw();
	}
}

void gui_pop_up_button::remove_item(const string& identifier) {
	const auto iter = items.find(identifier);
	const auto disp_iter = find(begin(display_items), end(display_items), &*iter);
	
	if(iter == items.end()) {
		a2e_error("no item with the identifier \"%s\" found!", identifier);
		return;
	}
	if(selected_item == &*iter) {
		selected_item = nullptr;
		redraw(); // redraw, if this was the selected item
	}
	items.erase(iter);
	
	if(disp_iter == end(display_items)) {
		a2e_error("display item for identifier \"%s\" not found!", identifier);
	}
	else display_items.erase(disp_iter);
	
	if(selected_item == nullptr && !display_items.empty()) {
		selected_item = display_items[0];
	}
}

const pair<const string, string>* gui_pop_up_button::get_selected_item() const {
	return selected_item;
}

void gui_pop_up_button::set_selected_item(const string& identifier) {
	const auto iter = items.find(identifier);
	if(iter == items.end()) {
		a2e_error("no item with the identifier \"%s\" found!", identifier);
		return;
	}
	if(selected_item != &*iter) {
		handle(GUI_EVENT::POP_UP_BUTTON_SELECT);
	}
	selected_item = &*iter;
}

void gui_pop_up_button::set_selected_item(const size_t& index) {
	if(index >= display_items.size()) {
		a2e_error("index \"%u\" is greater than the amount of items!", index);
		return;
	}
	if(selected_item != display_items[index]) {
		handle(GUI_EVENT::POP_UP_BUTTON_SELECT);
	}
	selected_item = display_items[index];
}
