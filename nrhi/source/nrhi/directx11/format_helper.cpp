#include <nrhi/directx11/format_helper.hpp>



namespace nrhi {

    static u32 directx11_format_stride_table[135];

    struct F_directx11_format_stride_setup {

        F_directx11_format_stride_setup() {

            memset(directx11_format_stride_table, 0xFF, sizeof(directx11_format_stride_table));

            directx11_format_stride_table[u32(DXGI_FORMAT_UNKNOWN)] = 0;

			directx11_format_stride_table[u32(DXGI_FORMAT_D32_FLOAT)] = 4;

            directx11_format_stride_table[u32(DXGI_FORMAT_R8G8B8A8_UNORM)] = 4;
            directx11_format_stride_table[u32(DXGI_FORMAT_R32_UINT)] = 4;
            directx11_format_stride_table[u32(DXGI_FORMAT_R32G32B32A32_FLOAT)] = 16;
            directx11_format_stride_table[u32(DXGI_FORMAT_R32G32B32_FLOAT)] = 12;
            directx11_format_stride_table[u32(DXGI_FORMAT_R32_FLOAT)] = 4;
        }
    };

    static F_directx11_format_stride_setup directx11_format_stride_setup;

    u32 HD_directx11_format::stride(E_format format) {

        if(u32(format) == 0xFFFFFFFF)
            return 4;

        NCPP_ASSERT(
            directx11_format_stride_table[u32(format)]
            != 0xFFFFFFFF
        ) << "non supported format";

        return directx11_format_stride_table[u32(format)];
    }

}