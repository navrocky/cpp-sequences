set(TARGET sequences)

set(SOURCES
    sequences.h
    sequences/iterable_sequence.h
    sequences/mapping_sequence.h
    sequences/sequence.cpp
)

add_library(${TARGET} STATIC ${SOURCES})
#target_link_libraries(${TARGET} ${QT_LIBRARIES} ${DEPS})
