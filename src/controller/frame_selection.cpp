#include "controller/frame_selection.h"
#include "input/input.h"
#include "app/definitions.h"

#include <ldv/bitmap_representation.h>
#include <ldv/box_representation.h>
#include <ldv/ttf_representation.h>

using namespace controller;

frame_selection::frame_selection(
	lm::logger& _log,
	ldtools::ttf_manager& _ttf_manager,
	const animation_editor::visuals& _visuals,
	ldv::rect _display,
	unsigned int _margin,
	unsigned int _size
)
	:log{_log},
	ttf_manager{_ttf_manager},
	visuals{_visuals},
	display_rect{_display},
	frame_list(_display.w, _display.w, _size, _size)
{

	frame_list.set_margin_w(_margin)
		.set_margin_h(_margin);
}

void frame_selection::awake(
	dfw::input& /*input*/
) {

	frame_list.clear();
	for(const auto& frame : visuals.get_table()) {

		frame_list.insert(frame);
	}

	intra_success=false;

}

void frame_selection::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	if(_input().is_exit_signal()) {
		set_leave(true);
		return;
	}

	if(_input.is_input_down(input::escape)) {

		pop_state();
		return;
	}

	if(_input.is_input_down(input::enter)) {

		intra_success=true;
		pop_state();
		return;
	}

	if(_input.is_input_down(input::left)) {

		frame_list.previous();
		return;
	}
	else if(_input.is_input_down(input::right)) {

		frame_list.next();
		return;
	}

	if(_input.is_input_down(input::up)) {

		frame_list.previous_row();
		return;
	}
	else if(_input.is_input_down(input::down)) {

		frame_list.next_row();
		return;
	}

	if(_input.is_input_down(input::pageup)) {

		frame_list.previous_page();
		return;
	}
	else if(_input.is_input_down(input::pagedown)) {

		frame_list.next_page();
		return;
	}
}

void frame_selection::draw(
	ldv::screen& _screen,
	int /*fps*/
) {
	_screen.clear(ldv::rgba8(0, 0, 0, 255));

	for(const auto& item : frame_list.get_page()) {

		if(item.index==frame_list.get_current_index()) {

			ldv::box_representation border{
				{item.x-1, item.y-1, frame_list.get_item_w()+2, frame_list.get_item_h()+2},
				ldv::rgba8(255,255,255,255),
				ldv::polygon_representation::type::line
			};

			border.draw(_screen);
		}

		ldv::bitmap_representation bmp(
			visuals.get_texture(),
			{item.x, item.y, frame_list.get_item_w(), frame_list.get_item_h()},
			item.item.second.box
		);

		bmp.set_blend(ldv::representation::blends::alpha);
		bmp.draw(_screen);

		ldv::ttf_representation txt(
			ttf_manager.get(animation_editor::definitions::main_font_name, animation_editor::definitions::main_font_size),
			ldv::rgba8(255,255,255, item.index==frame_list.get_current_index() ? 255 : 128),
			std::to_string(item.item.first)
		);

		txt.align(
			bmp,
			ldv::representation_alignment{
				ldv::representation_alignment::h::center,
				ldv::representation_alignment::v::outer_bottom,
				0,
				4
			}
		);

		txt.draw(_screen);
	}
}
