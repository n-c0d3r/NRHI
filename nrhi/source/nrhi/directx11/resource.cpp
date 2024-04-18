#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/device.hpp>



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

        d3d11_resource_p_->Release();
    }

    F_directx11_resource::F_initialize_d3d11_resource* F_directx11_resource::initialize_d3d11_resource_func_map_[
        NRHI_DRIVER_DIRECTX_11_RESOURCE_STRUCTURE_TYPE_COUNT
    ] = {
        0, // unknown
        &(F_directx11_resource::initialize_d3d11_resource_buffer),
        0,
        0,
        0,
        0,
        0
    };
    void F_directx11_resource::initialize_d3d11_resource(){

        NCPP_ASSERT(desc().type != E_resource_type::NONE) << "invalid resource structure type";

        initialize_d3d11_resource_func_map_[uint32_t(desc().type)](this);
    }
    void F_directx11_resource::initialize_d3d11_resource_buffer(F_directx11_resource* resource_p){

        ID3D11Device* d3d11_device_p = resource_p->device_p().T_cast<F_directx11_device>()->d3d11_device_p();

        const auto& desc = resource_p->desc();
        const auto& initial_data = resource_p->initial_data();

        D3D11_BUFFER_DESC d3d11_buffer_desc;
        d3d11_buffer_desc.BindFlags = D3D11_BIND_FLAG(desc.bind_flags);
        d3d11_buffer_desc.ByteWidth = desc.width;
        switch (desc.heap_type) {
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
        if(desc.stride)
            d3d11_buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        else
            d3d11_buffer_desc.MiscFlags = 0;
        d3d11_buffer_desc.StructureByteStride = desc.stride;

        D3D11_SUBRESOURCE_DATA d3d11_subresource_data;
        D3D11_SUBRESOURCE_DATA* d3d11_subresource_data_p = 0;
        if(initial_data.is_valid()) {

            d3d11_subresource_data.pSysMem = initial_data.system_mem_p;
            d3d11_subresource_data.SysMemPitch = initial_data.system_mem_pitch;
            d3d11_subresource_data.SysMemSlicePitch = initial_data.system_mem_slice_pitch;
            d3d11_subresource_data_p = &d3d11_subresource_data;
        }

        d3d11_device_p->CreateBuffer(
            &d3d11_buffer_desc,
            d3d11_subresource_data_p,
            (ID3D11Buffer**)&(resource_p->d3d11_resource_p_)
        );

        NCPP_ASSERT(resource_p->d3d11_resource_p_) << "buffer creation failed";
    }



    TU<A_resource> HD_directx11_resource::create(
        TK_valid<A_device> device_p,
        const F_initial_resource_data& initial_resource_data,
        const F_resource_desc& desc
    ){

        return TU<F_directx11_resource>()(device_p, initial_resource_data, desc);
    }

}