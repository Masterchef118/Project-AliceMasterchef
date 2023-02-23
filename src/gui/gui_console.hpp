#pragma once

#include "gui_element_types.hpp"

namespace ui {

class console_edit : public edit_box_element_base {
public:
	virtual void edit_box_enter(sys::state& state, std::string_view s) noexcept;
};

class console_list_entry : public listbox_row_element_base<std::string> {
private:
	simple_text_element_base* entry_text_box = nullptr;

public:
	std::unique_ptr<element_base> make_child(sys::state& state, std::string_view name, dcon::gui_def_id id) noexcept override {
		if(name == "console_text") {
			auto ptr = make_element_by_type<simple_text_element_base>(state, id);
			entry_text_box = ptr.get();
			entry_text_box->set_text(state, "");
			return ptr;
		} else {
			return nullptr;
		}
	}

	void update(sys::state& state) noexcept override {
		entry_text_box->set_text(state, content);
	}
};

class console_list : public listbox_element_base<console_list_entry, std::string> {
protected:
	std::string_view get_row_element_name() override {
		return "console_entry_wnd";
	}

	bool is_reversed() override {
		return true;
	}
};

class console_window : public window_element_base {
private:
	console_list* console_output_list = nullptr;

public:
	std::unique_ptr<element_base> make_child(sys::state& state, std::string_view name, dcon::gui_def_id id) noexcept override {
		if(name == "console_list") {
			auto ptr = make_element_by_type<console_list>(state, id);
			console_output_list = ptr.get();
			return ptr;
		} else if(name == "console_edit") {
			return make_element_by_type<console_edit>(state, id);
		} else {
			return nullptr;
		}
	}

	message_result get(sys::state& state, Cyto::Any& payload) noexcept override {
		if(payload.holds_type<std::string>()) {
			auto entry = any_cast<std::string>(payload);
			console_output_list->row_contents.push_back(entry);
			console_output_list->update(state);
			return message_result::consumed;
		} else {
			return message_result::unseen;
		}
	}

	static void show_toggle(sys::state& state);
};
}
