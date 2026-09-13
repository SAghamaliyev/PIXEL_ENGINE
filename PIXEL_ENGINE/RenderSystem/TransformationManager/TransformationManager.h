#pragma once
#include <../../glm-1.0.3/glm/glm.hpp>
#include <../../glm-1.0.3/glm/gtc/matrix_transform.hpp>
#include <../../glm-1.0.3/glm/gtc/type_ptr.hpp>

class TransformationManager {
private:
	glm::mat4 IdenMatrix;
public:
	TransformationManager();
};