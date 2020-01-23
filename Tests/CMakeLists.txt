
set(CoelTestSources
    CoelTest.cpp
)

add_executable(CoelTest ${CoelTestSources})
target_link_libraries(CoelTest Coelengine)
