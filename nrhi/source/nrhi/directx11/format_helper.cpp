#include <nrhi/directx11/format_helper.hpp>



namespace nrhi
{

	static u32 directx11_format_stride_table[135];

	struct F_directx11_format_stride_setup
	{

		F_directx11_format_stride_setup()
		{
			memset(directx11_format_stride_table, 0xFF, sizeof(directx11_format_stride_table));

			directx11_format_stride_table[u32(DXGI_FORMAT_UNKNOWN)] = 0;

			directx11_format_stride_table[u32(DXGI_FORMAT_R32G32B32A32_FLOAT)] = 16;
			directx11_format_stride_table[u32(DXGI_FORMAT_R32G32B32_FLOAT)] = 12;
			directx11_format_stride_table[u32(DXGI_FORMAT_R32G32_FLOAT)] = 8;
			directx11_format_stride_table[u32(DXGI_FORMAT_R32_FLOAT)] = 4;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16G16B16A16_FLOAT)] = 8;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16G16_FLOAT)] = 4;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16_FLOAT)] = 2;

			directx11_format_stride_table[u32(DXGI_FORMAT_D32_FLOAT)] = 4;

			directx11_format_stride_table[u32(DXGI_FORMAT_R16G16B16A16_UNORM)] = 8;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16G16_UNORM)] = 4;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16_UNORM)] = 2;
			directx11_format_stride_table[u32(DXGI_FORMAT_R8G8B8A8_UNORM)] = 4;
			directx11_format_stride_table[u32(DXGI_FORMAT_R8G8_UNORM)] = 2;
			directx11_format_stride_table[u32(DXGI_FORMAT_R8_UNORM)] = 1;

			directx11_format_stride_table[u32(DXGI_FORMAT_R16G16B16A16_SNORM)] = 8;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16G16_SNORM)] = 4;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16_SNORM)] = 2;
			directx11_format_stride_table[u32(DXGI_FORMAT_R8G8B8A8_SNORM)] = 4;
			directx11_format_stride_table[u32(DXGI_FORMAT_R8G8_SNORM)] = 2;
			directx11_format_stride_table[u32(DXGI_FORMAT_R8_SNORM)] = 1;

			directx11_format_stride_table[u32(DXGI_FORMAT_R32G32B32A32_UINT)] = 16;
			directx11_format_stride_table[u32(DXGI_FORMAT_R32G32B32_UINT)] = 12;
			directx11_format_stride_table[u32(DXGI_FORMAT_R32G32_UINT)] = 8;
			directx11_format_stride_table[u32(DXGI_FORMAT_R32_UINT)] = 4;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16G16B16A16_UINT)] = 8;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16G16_UINT)] = 4;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16_UINT)] = 2;
			directx11_format_stride_table[u32(DXGI_FORMAT_R8G8B8A8_UINT)] = 4;
			directx11_format_stride_table[u32(DXGI_FORMAT_R8G8_UINT)] = 2;
			directx11_format_stride_table[u32(DXGI_FORMAT_R8_UINT)] = 1;

			directx11_format_stride_table[u32(DXGI_FORMAT_R32G32B32A32_SINT)] = 16;
			directx11_format_stride_table[u32(DXGI_FORMAT_R32G32B32_SINT)] = 12;
			directx11_format_stride_table[u32(DXGI_FORMAT_R32G32_SINT)] = 8;
			directx11_format_stride_table[u32(DXGI_FORMAT_R32_SINT)] = 4;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16G16B16A16_SINT)] = 8;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16G16_SINT)] = 4;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16_SINT)] = 2;
			directx11_format_stride_table[u32(DXGI_FORMAT_R8G8B8A8_SINT)] = 4;
			directx11_format_stride_table[u32(DXGI_FORMAT_R8G8_SINT)] = 2;
			directx11_format_stride_table[u32(DXGI_FORMAT_R8_SINT)] = 1;

			directx11_format_stride_table[u32(DXGI_FORMAT_R32G32B32A32_TYPELESS)] = 16;
			directx11_format_stride_table[u32(DXGI_FORMAT_R32G32B32_TYPELESS)] = 12;
			directx11_format_stride_table[u32(DXGI_FORMAT_R32G32_TYPELESS)] = 8;
			directx11_format_stride_table[u32(DXGI_FORMAT_R32_TYPELESS)] = 4;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16G16B16A16_TYPELESS)] = 8;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16G16_TYPELESS)] = 4;
			directx11_format_stride_table[u32(DXGI_FORMAT_R16_TYPELESS)] = 2;
			directx11_format_stride_table[u32(DXGI_FORMAT_R8G8B8A8_TYPELESS)] = 4;
			directx11_format_stride_table[u32(DXGI_FORMAT_R8G8_TYPELESS)] = 2;
			directx11_format_stride_table[u32(DXGI_FORMAT_R8_TYPELESS)] = 1;
		}
	};

	static F_directx11_format_stride_setup directx11_format_stride_setup;

	u32 HD_directx11_format::stride(E_format format)
	{

		if (u32(format) == 0xFFFFFFFF)
			return 4;

		NCPP_ASSERT(
			directx11_format_stride_table[u32(format)]
				!= 0xFFFFFFFF
		) << "non supported format";

		return directx11_format_stride_table[u32(format)];
	}

	u32 HD_directx11_format::aligned_stride(E_format format) {

		return (
			1
			<< u32(ceil(
				log(stride(format))
				/ log(2.0f)
			))
		);
	}

}