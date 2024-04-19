#include <nrhi/directx11/format_helper.hpp>



namespace nrhi {

    static u32 directx11_format_stride_table[135];

    struct F_directx11_format_stride_setup {

        F_directx11_format_stride_setup() {

            directx11_format_stride_table[u32(DXGI_FORMAT_UNKNOWN)] = 0;

            directx11_format_stride_table[u32(DXGI_FORMAT_R8G8B8A8_UNORM)] = 4;
            directx11_format_stride_table[u32(DXGI_FORMAT_R32_UINT)] = 4;
        }
    };

    static F_directx11_format_stride_setup directx11_format_stride_setup;

    u32 HD_directx11_format::stride(E_format format) {

        if(u32(format) == 0xFFFFFFFF)
            return 4;

        return directx11_format_stride_table[u32(format)];
    }

}