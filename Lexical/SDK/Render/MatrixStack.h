#pragma once
#include <stack>
#include "../../Utils/Math.h"

class MatrixStack {
public:
	std::stack<glm::mat4x4> stack;
	bool isDirty;
};