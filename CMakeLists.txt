cmake_minimum_required(VERSION 3.20)
project(My3DSProject)  # replace with your project name

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PRIVATE ctru citro2d citro3d)

set(ASSET_HEADER_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets_headers")

file(MAKE_DIRECTORY "${ASSET_HEADER_DIR}")

set(ASSET_MASTER_HEADER "${ASSET_HEADER_DIR}/assets_generated.h")

file(WRITE "${ASSET_MASTER_HEADER}"
    "// Auto-generated asset header\n"
    "#pragma once\n"
    "#include <stddef.h>\n"
    "#include <stdint.h>\n"
    "#include <unordered_map>\n"
    "#include <string>\n\n"
    "namespace AssetsRaw {\n\n"
    "struct RawAsset {\n"
    "    const uint8_t* data;\n"
    "    size_t size;\n"
    "};\n\n"
    "static std::unordered_map<std::string, RawAsset> assetMap;\n\n"
    "void initAssets() {\n"
)

set(GENERATED_SOURCES "")
set(GENERATED_HEADERS "")

foreach(asset ${ASSET_FILES})
    file(RELATIVE_PATH asset_rel_path "${ASSET_DIR}" ${asset})

    string(REPLACE "\\" "/" asset_rel_path ${asset_rel_path})

    get_filename_component(asset_name ${asset} NAME)

    string(REPLACE "." "_" sym_name ${asset_name})

    set(header "${ASSET_HEADER_DIR}/${asset_rel_path}.h")
    set(source "${ASSET_HEADER_DIR}/${asset_rel_path}.c")

    get_filename_component(header_dir ${header} DIRECTORY)

    file(MAKE_DIRECTORY "${header_dir}")

    add_custom_command(
        OUTPUT ${header} ${source}
        COMMAND bin2c -d "${header}" -o "${source}" -n ${sym_name} "${asset}"
        DEPENDS ${asset}
        COMMENT "Generating C header & source for asset: ${asset_rel_path}"
    )

    list(APPEND GENERATED_HEADERS ${header})
    list(APPEND GENERATED_SOURCES ${source})
endforeach()

target_sources(${PROJECT_NAME} PRIVATE ${GENERATED_SOURCES})

file(APPEND "${ASSET_MASTER_HEADER}" "}\n} // namespace AssetsRaw\n")

add_custom_target(assets_headers ALL
    DEPENDS ${GENERATED_HEADERS} "${ASSET_MASTER_HEADER}"
)

add_dependencies(${PROJECT_NAME} assets_headers)

target_include_directories(${PROJECT_NAME} PRIVATE "${ASSET_HEADER_DIR}")