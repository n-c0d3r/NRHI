#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

    F_directx11_resource::F_directx11_resource(
        TK_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc
    ) :
        A_resource(device_p, initial_data, desc)
    {

        initialize_d3d11_resource();

    }
    F_directx11_resource::F_directx11_resource(
        TK_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc,
        ID3D11Resource* d3d11_resource_p
    ) :
        A_resource(device_p, initial_data, desc),
        d3d11_resource_p_(d3d11_resource_p)
    {

        initialize_d3d11_resource();

    }
    F_directx11_resource::~F_directx11_resource() {

        if(d3d11_resource_p_)
            d3d11_resource_p_->Release();
    }

    void F_directx11_resource::initialize_d3d11_resource(){

        NCPP_ASSERT(desc().type != E_resource_type::NONE) << "invalid resource structure type";

        switch (desc().type) {
            case E_resource_type::BUFFER:
                initialize_d3d11_buffer();
                break;
            case E_resource_type::STRUCTURED_BUFFER:
                initialize_d3d11_structured_buffer();
                break;
            case E_resource_type::SINGLE_ELEMENTED_BUFFER:
                initialize_d3d11_single_elemented_buffer();
                break;
        }
    }
    void F_directx11_resource::initialize_d3d11_buffer() {

        ID3D11Device* d3d11_device_p = device_p().T_cast<F_directx11_device>()->d3d11_device_p();

        const auto& desc_ = desc();
        const auto& initial_data_ = initial_data();

        D3D11_BUFFER_DESC d3d11_buffer_desc;
        d3d11_buffer_desc.BindFlags = D3D11_BIND_FLAG(desc_.bind_flags);
        d3d11_buffer_desc.ByteWidth = desc_.width;
        switch (desc_.heap_type) {
            case E_resource_heap_type::GREAD_GWRITE:
                d3d11_buffer_desc.CPUAccessFlags = 0;
                d3d11_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
                break;
            case E_resource_heap_type::GREAD_CWRITE:
                d3d11_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                d3d11_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
                break;
            case E_resource_heap_type::CREAD_GWRITE:
                d3d11_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
                d3d11_buffer_desc.Usage = D3D11_USAGE_STAGING;
                break;
        }
        d3d11_buffer_desc.StructureByteStride = H_format::stride(desc_.format);
        d3d11_buffer_desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA d3d11_subresource_data;
        D3D11_SUBRESOURCE_DATA* d3d11_subresource_data_p = 0;
        if(initial_data_.is_valid()) {

            d3d11_subresource_data.pSysMem = initial_data_.system_mem_p;
            d3d11_subresource_data.SysMemPitch = initial_data_.system_mem_pitch;
            d3d11_subresource_data.SysMemSlicePitch = initial_data_.system_mem_slice_pitch;
            d3d11_subresource_data_p = &d3d11_subresource_data;
        }

        d3d11_device_p->CreateBuffer(
            &d3d11_buffer_desc,
            d3d11_subresource_data_p,
            (ID3D11Buffer**)&d3d11_resource_p_
        );

        NCPP_ASSERT(d3d11_resource_p_) << "buffer creation failed";
    }
    void F_directx11_resource::initialize_d3d11_structured_buffer(){

        ID3D11Device* d3d11_device_p = device_p().T_cast<F_directx11_device>()->d3d11_device_p();

        const auto& desc_ = desc();
        const auto& initial_data_ = initial_data();

        D3D11_BUFFER_DESC d3d11_buffer_desc;
        d3d11_buffer_desc.BindFlags = D3D11_BIND_FLAG(desc_.bind_flags);
        d3d11_buffer_desc.ByteWidth = desc_.width;
        switch (desc_.heap_type) {
            case E_resource_heap_type::GREAD_GWRITE:
                d3d11_buffer_desc.CPUAccessFlags = 0;
                d3d11_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
                break;
            case E_resource_heap_type::GREAD_CWRITE:
                d3d11_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                d3d11_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
                break;
            case E_resource_heap_type::CREAD_GWRITE:
                d3d11_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
                d3d11_buffer_desc.Usage = D3D11_USAGE_STAGING;
                break;
        }
        d3d11_buffer_desc.StructureByteStride = desc_.stride;
        d3d11_buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

        D3D11_SUBRESOURCE_DATA d3d11_subresource_data;
        D3D11_SUBRESOURCE_DATA* d3d11_subresource_data_p = 0;
        if(initial_data_.is_valid()) {

            d3d11_subresource_data.pSysMem = initial_data_.system_mem_p;
            d3d11_subresource_data.SysMemPitch = initial_data_.system_mem_pitch;
            d3d11_subresource_data.SysMemSlicePitch = initial_data_.system_mem_slice_pitch;
            d3d11_subresource_data_p = &d3d11_subresource_data;
        }

        d3d11_device_p->CreateBuffer(
            &d3d11_buffer_desc,
            d3d11_subresource_data_p,
            (ID3D11Buffer**)&d3d11_resource_p_
        );

        NCPP_ASSERT(d3d11_resource_p_) << "structured buffer creation failed";
    }
    void F_directx11_resource::initialize_d3d11_single_elemented_buffer(){

        ID3D11Device* d3d11_device_p = device_p().T_cast<F_directx11_device>()->d3d11_device_p();

        const auto& desc_ = desc();
        const auto& initial_data_ = initial_data();

        D3D11_BUFFER_DESC d3d11_buffer_desc;
        d3d11_buffer_desc.BindFlags = D3D11_BIND_FLAG(desc_.bind_flags);
        d3d11_buffer_desc.ByteWidth = desc_.width;
        switch (desc_.heap_type) {
            case E_resource_heap_type::GREAD_GWRITE:
                d3d11_buffer_desc.CPUAccessFlags = 0;
                d3d11_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
                break;
            case E_resource_heap_type::GREAD_CWRITE:
                d3d11_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                d3d11_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
                break;
            case E_resource_heap_type::CREAD_GWRITE:
                d3d11_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
                d3d11_buffer_desc.Usage = D3D11_USAGE_STAGING;
                break;
        }
        d3d11_buffer_desc.StructureByteStride = 0;
        d3d11_buffer_desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA d3d11_subresource_data;
        D3D11_SUBRESOURCE_DATA* d3d11_subresource_data_p = 0;
        if(initial_data_.is_valid()) {

            d3d11_subresource_data.pSysMem = initial_data_.system_mem_p;
            d3d11_subresource_data.SysMemPitch = initial_data_.system_mem_pitch;
            d3d11_subresource_data.SysMemSlicePitch = initial_data_.system_mem_slice_pitch;
            d3d11_subresource_data_p = &d3d11_subresource_data;
        }

        d3d11_device_p->CreateBuffer(
            &d3d11_buffer_desc,
            d3d11_subresource_data_p,
            (ID3D11Buffer**)&d3d11_resource_p_
        );

        NCPP_ASSERT(d3d11_resource_p_) << "raw buffer creation failed";
    }



    TU<A_resource> HD_directx11_resource::create(
        TK_valid<A_device> device_p,
        const F_initial_resource_data& initial_resource_data,
        const F_resource_desc& desc
    ){

        return TU<F_directx11_resource>()(device_p, initial_resource_data, desc);
    }

}