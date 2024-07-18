#pragma once

#include <nrhi/prerequisites.hpp>



namespace nrhi {

	class A_device;



	class NRHI_API A_device_child {

	private:
		TK_valid<A_device> device_p_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<A_device> device_p() const noexcept { return device_p_; }



	protected:
		A_device_child(TKPA_valid<A_device> device_p);
		~A_device_child();

	};

}