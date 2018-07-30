
set(test_name ${app_name}-tests)

file(GLOB src *.cpp)
add_executable(${test_name} ${src} ${game_src})

target_include_directories(${test_name} PRIVATE
    ${Boost_INCLUDE_DIRS}
    ./3rd/
    ..
)

add_test(NAME ${test_name} COMMAND ${test_name})
