#include "SdfRenderer.hpp"

#include "Utility/Time.hpp"
#include "Utility/File.hpp"

SdfRenderer::SdfRenderer(SdfRenderer_Params params)
	: _params(params),
	  _rendererSrc(MyFile::LoadToString("shaders/rendering/SDF_Test1.fs.glsl")) // We don't provide choice for the rendering code used because setting the uniforms in render() depends on the render code
{}

void SdfRenderer::setScene(const std::string& sceneSrc) {
	_shader.compile("#version 330\n" + sceneSrc + _rendererSrc);
}

void SdfRenderer::render() {
	_shader.get().bind();
	_shader.get().setUniformValue("iTime", Time::Elapsed() / 1000.f); // TODO it is more the responsibility of the scene than the renderer to know whether this kind of uniform is required
	_shader.get().setUniformValue("u_AbsorptionCoefficient", _params.absorptionCoefficient);
	_shader.draw();
}