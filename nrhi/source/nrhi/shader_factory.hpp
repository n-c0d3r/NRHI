#pragma once

/** @file nrhi/shader_factory.hpp
*
*   Implement shader factory.
*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#pragma region Includes

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/prerequisites.hpp>

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/shader_type.hpp>
#include <nrhi/shader_desc.hpp>
#include <nrhi/shader.hpp>
#include <nrhi/shader_class_base.hpp>

#pragma endregion



namespace nrhi {

	class A_device;
	class A_shader;
	class A_shader_blob;
	class A_shader_class;



	class H_vertex_shader {

	public:
		static NCPP_FORCE_INLINE U_vertex_shader_handle create(TKPA_valid<A_device> device_p, const F_shader_desc& desc) {

			return H_shader::create_vertex_shader(device_p, desc);
		}
		static NCPP_FORCE_INLINE U_vertex_shader_handle create(
			TKPA_valid<A_device> device_p,
			TKPA_valid<A_shader_class> shader_class_p,
			const G_string& shader_blob_name
		) {

			return H_shader::create_vertex_shader(
				device_p,
				F_shader_desc {
					.blob_p = shader_class_p->shader_blob_p(shader_blob_name)
				}
			);
		}

	};

	class H_pixel_shader {

	public:
		static NCPP_FORCE_INLINE U_pixel_shader_handle create(TKPA_valid<A_device> device_p, const F_shader_desc& desc) {

			return H_shader::create_pixel_shader(device_p, desc);
		}
		static NCPP_FORCE_INLINE U_pixel_shader_handle create(
			TKPA_valid<A_device> device_p,
			TKPA_valid<A_shader_class> shader_class_p,
			const G_string& shader_blob_name
		) {

			return H_shader::create_pixel_shader(
				device_p,
				F_shader_desc {
					.blob_p = shader_class_p->shader_blob_p(shader_blob_name)
				}
			);
		}

	};

	class H_compute_shader {

	public:
		static NCPP_FORCE_INLINE U_compute_shader_handle create(TKPA_valid<A_device> device_p, const F_shader_desc& desc) {

			return H_shader::create_compute_shader(device_p, desc);
		}
		static NCPP_FORCE_INLINE U_compute_shader_handle create(
			TKPA_valid<A_device> device_p,
			TKPA_valid<A_shader_class> shader_class_p,
			const G_string& shader_blob_name
		) {

			return H_shader::create_compute_shader(
				device_p,
				F_shader_desc {
					.blob_p = shader_class_p->shader_blob_p(shader_blob_name)
				}
			);
		}

	};

}
