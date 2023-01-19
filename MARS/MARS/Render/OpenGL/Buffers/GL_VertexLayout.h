#pragma once

#include <vector>

namespace OpenGL
{
	enum class DataType : uint8_t
	{
		UnsignedByte,
		SignedByte,

		UnsignedInteger,
		SignedInteger,

		Float,
		Float2,
		Float3,
		Float4,
	};

	struct LayoutElement
	{
		LayoutElement(DataType type);

		uint32_t NativeType;
		uint32_t Length;
		uint32_t Size;
	};

	using VertexLayout = std::vector<LayoutElement>;

	const VertexLayout SimpleVertexLayout = { DataType::Float2 };

	const VertexLayout VertexLayoutCT = {
		DataType::Float3,
		DataType::Float3,
		DataType::Float2,
	};

	const VertexLayout VertexLayoutNT = {
		DataType::Float3, // Location
		DataType::Float3, // Normal
		DataType::Float2, // UV
	};
};