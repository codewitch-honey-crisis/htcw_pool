# Copyright (c) 2023 honey the codewitch
#
# SPDX-License-Identifier: Apache-2.0

if(CONFIG_HTCW_POOL)

zephyr_library()

#include_directories(${ZEPHYR_BASE}/lib/???/htcw_pool)
zephyr_include_directories(..)
zephyr_library_include_directories(..)
zephyr_interface_library_named(HTCW_POOL)


#zephyr_library_sources_ifdef(CONFIG_PARAM_DEF, file.cpp)
zephyr_library_link_libraries(HTCW_POOL)
#target_link_libraries(HTCW_POOL INTERFACE zephyr_interface)

endif()