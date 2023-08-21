cmake_minimum_required(VERSION 3.25)

project(async_notification
    VERSION 0.1.0
    DESCRIPTION ""
    HOMEPAGE_URL https://github.com/vadimvinnik/async_notification
    LANGUAGES CXX
)

include(GNUInstallDirs)

set(ASYNC_NOTIFICATION_BASE_ALIAS ${PROJECT_NAME} CACHE INTERNAL "Base targets alias" FORCE)
set(ASYNC_NOTIFICATION_CXX_STANDART 17 CACHE INTERNAL "Project c++ standart version" FORCE)

option(async_notification_BUILD_DEMO "Enable build demo target" ON)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${ASYNC_NOTIFICATION_BASE_ALIAS}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_sources(${PROJECT_NAME}
    INTERFACE 
        FILE_SET HEADERS
            BASE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include
            FILES
                include/async_notification/optional.hpp
                include/async_notification/queue.hpp
                include/async_notification/simple_variable.hpp
                include/async_notification/unique_ptr.hpp
                include/async_notification.hpp
)

set_target_properties(${PROJECT_NAME} 
    PROPERTIES
        CXX_STANDARD ${ASYNC_NOTIFICATION_CXX_STANDART}
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS OFF
)

if (async_notification_BUILD_DEMO)
    add_subdirectory(demo)
endif()

if(NOT CMAKE_SKIP_INSTALL_RULES)
    include(CMakePackageConfigHelpers)

    install(TARGETS ${PROJECT_NAME}
        EXPORT ${ASYNC_NOTIFICATION_BASE_ALIAS}Targets
        FILE_SET HEADERS
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(EXPORT ${ASYNC_NOTIFICATION_BASE_ALIAS}Targets
        FILE ${ASYNC_NOTIFICATION_BASE_ALIAS}Targets.cmake
        NAMESPACE ${ASYNC_NOTIFICATION_BASE_ALIAS}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${ASYNC_NOTIFICATION_BASE_ALIAS}
    )

    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/${ASYNC_NOTIFICATION_BASE_ALIAS}Config.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${ASYNC_NOTIFICATION_BASE_ALIAS}"
        NO_SET_AND_CHECK_MACRO
    )

    write_basic_package_version_file("${ASYNC_NOTIFICATION_BASE_ALIAS}ConfigVersion.cmake"
        COMPATIBILITY AnyNewerVersion
    )

    install(FILES 
        ${CMAKE_CURRENT_BINARY_DIR}/${ASYNC_NOTIFICATION_BASE_ALIAS}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${ASYNC_NOTIFICATION_BASE_ALIAS}ConfigVersion.cmake
        DESTINATION 
            ${CMAKE_INSTALL_LIBDIR}/cmake/${ASYNC_NOTIFICATION_BASE_ALIAS}
    )
endif()
