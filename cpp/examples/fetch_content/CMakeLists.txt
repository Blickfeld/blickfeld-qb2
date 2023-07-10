include(FetchContent)
FetchContent_Declare(blickfeld-qb2
    GIT_REPOSITORY https://github.com/Blickfeld/blickfeld-qb2.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(blickfeld-qb2)

add_executable(example main.cpp)
target_link_libraries(example PUBLIC blickfeld-qb2)