#pragma once
#include "../../src/Definitions.h"

class TransformationManager {
public:
	void Translate(glm::mat4& OurMatrix, glm::vec3& TranslateV);
	void Rotate(glm::mat4& OurMatrix, glm::vec3& RotateV);
	void Scale(glm::mat4& OurMatrix, glm::vec3& ScaleV);
};