#pragma once

#include <string>
#include "FullscreenShader.hpp"

struct SdfRenderer_Params {
	float absorptionCoefficient = 0.5f;
};

class SdfRenderer {
public:
	SdfRenderer(SdfRenderer_Params params = {});
	~SdfRenderer() = default;

	// Creates and compiles a shader that will render the scene. Takes some glsl source code implementing a "float SDF(vec3 p)" function
	void setScene(const std::string& sceneSrc);
	void render();

private:
	SdfRenderer_Params _params;
	FullscreenShader _shader;
	std::string _rendererSrc;
};