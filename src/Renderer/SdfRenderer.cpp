#include "SdfRenderer.hpp"

#include "Utility/Time.hpp"

SdfRenderer::SdfRenderer(const std::string& sceneShaderSrc, SdfRenderer_Params params)
	: _params(params)
{
	setScene(sceneShaderSrc);
}

void SdfRenderer::setScene(const std::string& sceneShaderSrc) {
	_shader.compile(sceneShaderSrc);
}

void SdfRenderer::render() {
	_shader.get().bind();
	_shader.get().setUniformValue("iTime", Time::Elapsed() / 1000.f); // TODO it is more the responsibility of the scene than the renderer to know whether this kind of uniform is required
	_shader.get().setUniformValue("u_AbsorptionCoefficient", _params.absorptionCoefficient);
	_shader.draw();
}