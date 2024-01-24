if (NOT TARGET blickfeld-qb2)
    include(FetchContent)
    FetchContent_Declare(blickfeld-qb2
        GIT_REPOSITORY https://github.com/Blickfeld/blickfeld-qb2.git
        GIT_TAG        main
    )
    FetchContent_MakeAvailable(blickfeld-qb2)
endif()

add_executable(authentication-example main.cpp)
target_link_libraries(authentication-example PUBLIC blickfeld-qb2)