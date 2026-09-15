#pragma once
#include "../../src/Definitions.h"

class TransformManager {
private:
	void ResetMatrix(glm::mat4& OurMatrix);
	void Translate(glm::mat4& OurMatrix, const glm::vec3& TranslateV);
	void Rotate(glm::mat4& OurMatrix, const glm::vec3& RotateV);
	void Scale(glm::mat4& OurMatrix, const glm::vec3& ScaleV);
public:
	void ReTransformEntity(Transform& TransformInfo);
};