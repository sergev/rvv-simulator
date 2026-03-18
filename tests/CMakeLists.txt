set(tests unit_tests stdlib vector_examples)

foreach(name ${tests})
    add_executable(${name} "${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp")
    set_target_properties(${name} PROPERTIES FOLDER "tests")
    target_link_libraries(${name} PRIVATE GTest::gtest_main)
    gtest_discover_tests(${name})
endforeach()

target_link_libraries(unit_tests PRIVATE riscv32)
target_link_libraries(vector_examples PRIVATE riscv32)
target_link_libraries(stdlib PRIVATE riscv32 vstdlib)
