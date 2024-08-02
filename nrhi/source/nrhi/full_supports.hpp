#pragma once

#include <nrhi/config.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_simple_resource_binding()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_simple_resource_binding()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "simple resource binding (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "simple resource binding (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_advanced_resource_binding()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_advanced_resource_binding()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource binding (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_RESOURCE_BINDING(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource binding (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...)
#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_simple_resource_management()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_simple_resource_management()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "simple resource management (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "simple resource management (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_advanced_resource_management()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_advanced_resource_management()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource management (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource management (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...)
#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_simple_work_submission()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_simple_work_submission()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "simple work submission (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "simple work submission (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_advanced_work_submission()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_advanced_work_submission()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced work submission (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_WORK_SUBMISSION(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced work submission (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...)
#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_simple_indirect_commands()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_simple_indirect_commands()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "simple indirect commands (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "simple indirect commands (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_advanced_indirect_commands()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_advanced_indirect_commands()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced indirect commands (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_SIMPLE_INDIRECT_COMMANDS(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced indirect commands (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...)
#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_advanced_resource_binding()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_advanced_resource_binding()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource binding (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource binding (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_advanced_resource_binding()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_advanced_resource_binding()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource binding (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource binding (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_BINDING(...)
#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_advanced_resource_management()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_advanced_resource_management()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource management (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource management (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_advanced_resource_management()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_advanced_resource_management()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource management (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced resource management (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_RESOURCE_MANAGEMENT(...)
#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_advanced_work_submission()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_advanced_work_submission()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced work submission (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced work submission (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_advanced_work_submission()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_advanced_work_submission()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced work submission (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced work submission (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_WORK_SUBMISSION(...)
#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_advanced_indirect_commands()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_advanced_indirect_commands()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced indirect commands (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced indirect commands (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(nrhi::H_driver::is_full_support_advanced_indirect_commands()) { \
                __VA_ARGS__ ; \
            }
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(!nrhi::H_driver::is_full_support_advanced_indirect_commands()) { \
                __VA_ARGS__ ; \
            }
#else
#define NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(false) { \
			}
#define NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) NRHI_DRIVER_IF(true) { \
            	__VA_ARGS__; \
			}
#endif // NRHI_DRIVER_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) \
			NRHI_DRIVER_ENABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced indirect commands (required by this function) is not supported by current driver"; }
#else
#ifdef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) __VA_ARGS__
#else
#define NRHI_DRIVER_REQUIRE_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...)
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef NCPP_ENABLE_ASSERT
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) \
			NRHI_DRIVER_DISABLE_IF_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(__VA_ARGS__) \
			else { NCPP_ASSERT(false) << "advanced indirect commands (forbidden by this function) is supported by current driver"; }
#else
#ifndef NRHI_DRIVER_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...) __VA_ARGS__
#else
#define NRHI_DRIVER_FORBID_FULL_SUPPORT_ADVANCED_INDIRECT_COMMANDS(...)
#endif
#endif