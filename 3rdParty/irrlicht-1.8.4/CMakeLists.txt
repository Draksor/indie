if (MSVC)  
    set(IRRLICHT_DLL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/msvc19-x64/Irrlicht.dll")
    # Copy dll file near to bomberman target
    file(COPY ${IRRLICHT_DLL_PATH} DESTINATION "${CMAKE_SOURCE_DIR}/bin")

    set(IRRLICHT_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/msvc19-x64/Irrlicht.lib" CACHE INTERNAL "")
    set(IRRLICHT_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/msvc19-x64/include" CACHE INTERNAL "") 
elseif (APPLE)
    set(IRRLICHT_FRAMEWORK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/osx-x64/IrrFramework.framework")
    # Copy framework near to bomberman target
    file (COPY ${IRRLICHT_FRAMEWORK_PATH} DESTINATION "${CMAKE_SOURCE_DIR}/bin/Frameworks")

    set(IRRLICHT_LIB_PATH "${IRRLICHT_FRAMEWORK_PATH}/IrrFramework" CACHE INTERNAL "")
    set(IRRLICHT_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/osx-x64/include" CACHE INTERNAL "")
endif ()

add_library(irrlicht STATIC IMPORTED GLOBAL)

set_target_properties(irrlicht PROPERTIES
    IMPORTED_LOCATION "${IRRLICHT_LIB_PATH}"
    INTERFACE_INCLUDE_DIRECTORIES "${IRRLICHT_INCLUDE_PATH}"
)