cmake_minimum_required(VERSION 2.6)

project("main")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-std=c++17")

find_package(Boost REQUIRED COMPONENTS system program_options filesystem iostreams)


include_directories(SYSTEM ${Boost_INCLUDE_DIR})

add_executable(main
    main.cc
)

target_link_libraries(main ${Boost_INCLUDE_DIR}
    glfw vulkan X11 Xxf86vm Xrandr Xi
    # lglfw lvulkan ldl lpthread lX11 lXxf86vm lXrandr lXi
)

target_compile_options(main PRIVATE -Wall -Wextra -fopenmp -march=native -g -ffast-math -ggdb -fno-omit-frame-pointer -Ofast)
target_compile_definitions(main PRIVATE -DCROW_ENABLE_SSL)
target_link_options(main PRIVATE -fopenmp -march=native -ffast-math -ggdb -Ofast -fuse-ld=gold -L/usr/local/lib -lcrypto -lssl)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
