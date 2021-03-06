#pragma once

#include "app/animation.h"

#include <string>

namespace animation_editor {

class animation_saver {

	public:

	std::string         to_string(const animations&, bool=false);
	void                to_file(const std::string&, const animations&, bool=false);

	private:
};

}
