#pragma once

#include <string>
#include "FullscreenShader.hpp"

struct SdfRenderer_Params {
	float absorptionCoefficient = 0.5f;
};

class SdfRenderer {
public:
	SdfRenderer(const std::string& sceneShaderSrc, SdfRenderer_Params params = {});
	~SdfRenderer() = default;

	// Creates and compiles a shader that will render the scene. Takes some glsl source code implementing a "float SDF(vec3 p)" function
	void setScene(const std::string& sceneShaderSrc);
	void render();

private:
	SdfRenderer_Params _params;
	FullscreenShader _shader;
};