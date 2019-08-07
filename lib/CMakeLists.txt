set(TARGET sequences)

set(SOURCES
    sequences.h
    sequences/sequence.cpp
#    sequence.cpp
)

add_library(${TARGET} STATIC ${SOURCES})
#target_link_libraries(${TARGET} ${QT_LIBRARIES} ${DEPS})
