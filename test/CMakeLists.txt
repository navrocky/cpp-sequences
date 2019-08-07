set(TARGET tests)

set(SOURCES
    main.cpp
    sequence_test.cpp
)

add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} sequences)
