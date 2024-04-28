#include <nrhi/texture.hpp>
#include <nrhi/resource.hpp>



namespace nrhi {

    U_texture_1d_handle H_texture::create_1d(
        TKPA_valid<A_device> device_p,
        F_initial_resource_data initial_data,
        u32 width,
        E_format format,
        u32 mip_level_count,
        E_resource_bind_flag bind_flags,
        E_resource_heap_type heap_type
    ) {

        return H_resource::create_texture_1d(
            device_p,
            initial_data,
            H_resource_desc::create_texture_1d_desc(
                width,
                format,
                mip_level_count,
                bind_flags,
                heap_type
            )
        );
    }

    U_texture_2d_handle H_texture::create_2d(
        TKPA_valid<A_device> device_p,
        F_initial_resource_data initial_data,
        u32 width,
        u32 height,
        E_format format,
        u32 mip_level_count,
        F_sample_desc sample_desc,
        E_resource_bind_flag bind_flags,
        E_resource_heap_type heap_type
    ) {

        return H_resource::create_texture_2d(
            device_p,
            initial_data,
            H_resource_desc::create_texture_2d_desc(
                width,
                height,
                format,
                mip_level_count,
                sample_desc,
                bind_flags,
                heap_type
            )
        );
    }

    U_texture_3d_handle H_texture::create_3d(
        TKPA_valid<A_device> device_p,
        F_initial_resource_data initial_data,
        u32 width,
        u32 height,
        u32 depth,
        E_format format,
        u32 mip_level_count,
        E_resource_bind_flag bind_flags,
        E_resource_heap_type heap_type
    ) {

        return H_resource::create_texture_3d(
            device_p,
            initial_data,
            H_resource_desc::create_texture_3d_desc(
                width,
                height,
                depth,
                format,
                mip_level_count,
                bind_flags,
                heap_type
            )
        );
    }

	U_texture_2d_array_handle H_texture::create_2d_array(
		TKPA_valid<A_device> device_p,
		F_initial_resource_data initial_data,
		u32 width,
		u32 height,
		u32 count,
		E_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		E_resource_bind_flag bind_flags,
		E_resource_heap_type heap_type
	) {

		return H_resource::create_texture_2d_array(
			device_p,
			initial_data,
			H_resource_desc::create_texture_2d_array_desc(
				width,
				height,
				count,
				format,
				mip_level_count,
				sample_desc,
				bind_flags,
				heap_type
			)
		);
	}

	U_texture_cube_handle H_texture::create_cube(
		TKPA_valid<A_device> device_p,
		F_initial_resource_data initial_data,
		u32 width,
		E_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		E_resource_bind_flag bind_flags,
		E_resource_heap_type heap_type
	) {

		return H_resource::create_texture_cube(
			device_p,
			initial_data,
			H_resource_desc::create_texture_cube_desc(
				width,
				format,
				mip_level_count,
				sample_desc,
				bind_flags,
				heap_type
			)
		);
	}

}