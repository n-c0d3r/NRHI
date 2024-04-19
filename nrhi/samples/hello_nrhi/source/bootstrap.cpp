
#include <nrhi/.hpp>

using namespace nrhi;



NCPP_ENTRY_POINT() {

    NCPP_INFO()
        << "Hello "
        << E_log_color::V_FOREGROUND_BRIGHT_MAGNETA
        << "NRHI "
        << E_log_color::V_FOREGROUND_BRIGHT_BLUE
        << NRHI_VERSION_STR
        << E_log_color::RESET;



    // initialize nrhi and deferrered release it
    nrhi::initialize_system();
    NCPP_DEFER(nrhi::release_system());



    // get adapter and create device
    const auto& adapter_p_vector = H_adapter::adapter_p_vector();
    NCPP_ASSERT(adapter_p_vector.size());

    auto device_p = H_device::create(adapter_p_vector[0]);



    // create command queue and command list
    auto command_list_type = E_command_list_type::DIRECT;

    auto command_queue_p = H_command_queue::create(
        NCPP_FOREF_VALID(device_p),
        F_command_queue_desc {
            command_list_type
        }
    );
    auto command_list_p = H_command_list::create(
        NCPP_FOREF_VALID(device_p),
        F_command_list_desc {
            command_list_type
        }
    );



    // create surface manager and surface
    F_surface_manager surface_manager;
    auto surface_p = surface_manager.create_surface({
        .title = G_wstring(L"Hello NRHI ") + TEXT(NRHI_VERSION_STR),
        .size = { 1200, 700 }
    });



    //create swapchain
    auto swapchain_p = H_swapchain::create(
        NCPP_FOREF_VALID(command_queue_p),
        surface_p,
        F_swapchain_desc {
        }
    );



    TG_vector<F_vector4> vertices(128);
    U_buffer_handle vbuffer_p = H_buffer::T_create_structured<F_vector4>({
        .device_p = NCPP_FOREF_VALID(device_p),
        .data = vertices,
        .bind_flags = E_resource_bind_flag::SRV
    });

    U_srv_handle vbuffer_srv_p = H_resource_view::create_srv(
        NCPP_FOREF_VALID(device_p),
        NCPP_FHANDLE_VALID(vbuffer_p),
        {}
    );

    F_vector4 cdata;
    U_buffer_handle cbuffer_p = H_buffer::T_create_single_elemented<F_vector4>({
        .device_p = NCPP_FOREF_VALID(device_p),
        .data = &cdata,
        .bind_flags = E_resource_bind_flag::CBV
    });



    // run app
    surface_manager.T_run([](F_surface_manager& surface_manager){
    });

	return 0;
}
