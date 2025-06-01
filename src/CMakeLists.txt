add_executable(ArchGen 
    main.cpp 
    ./core/plane_framework.cpp
    ./core/opengl_window.cpp
    ./core/opengl_shader.cpp
    ./core/opengl_glloader.cpp
    ./core/opengl_drawable.cpp
    ./core/object.cpp
    ./core/stb_image.cpp
    ./core/shodow_map.cpp
    
    ./archgen/light.cpp
    ./archgen/archgen.cpp
    ./archgen/model.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders/")

target_link_libraries(ArchGen glfw glad glm spdlog::spdlog)
