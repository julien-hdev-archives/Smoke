#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>

	struct TagComp
	{
		std::string Tag;

		TagComp() = default;
		TagComp(const TagComp& other) = default;
		TagComp(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComp
	{
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComp() = default;
		TransformComp(const TransformComp& other) = default;
		TransformComp(const glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct MeshComp
	{
		std::vector<glm::vec4> Vertices = {};
		uint32_t VertexCount = {};

		MeshComp() = default;
		MeshComp(const MeshComp& other) = default;
		MeshComp(const std::vector<glm::vec4>& vertices)
			: Vertices(vertices), VertexCount(vertices.size()) {}

		operator std::vector<glm::vec4>& () { return Vertices; }
		operator const std::vector<glm::vec4>& () const { return Vertices; }
	};

	struct SpriteColorComp // TODO : Implement
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteColorComp() = default;
		SpriteColorComp(const SpriteColorComp& other) = default;
		SpriteColorComp(const glm::vec4& color)
			: Color(color) {}
	};
