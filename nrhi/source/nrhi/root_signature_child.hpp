#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/device_child.hpp>
#include <nrhi/driver.hpp>



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
namespace nrhi {

	class A_device_child;
	class A_root_signature;



	class NRHI_API A_root_signature_child : public A_device_child {

	private:
		TK<A_root_signature> root_signature_p_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<A_root_signature> root_signature_p() const noexcept {

			NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
				return (TKPA_valid<A_root_signature>)root_signature_p_;
			)
		}
		NCPP_FORCE_INLINE void set_root_signature_p_unsafe(TKPA_valid<A_root_signature> value) noexcept {

			NRHI_DRIVER_REQUIRE_SUPPORT_ADVANCED_RESOURCE_BINDING(
				root_signature_p_ = value.no_requirements();
			)
		}



	protected:
		A_root_signature_child(
			TKPA_valid<A_device> device_p
		);
		A_root_signature_child(
			TKPA_valid<A_device> device_p,
			TKPA_valid<A_root_signature> root_signature_p
		);
		~A_root_signature_child();

	};

}
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING