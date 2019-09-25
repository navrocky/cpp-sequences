set(TARGET sequences)

set(SOURCES
    sequences.h
    common/sequence.h
    common/sequence.cpp
    sequences/args_sequence.h
    sequences/iterable_sequence.h
    mutators/mapping_sequence.h
)

add_library(${TARGET} STATIC ${SOURCES})
#target_link_libraries(${TARGET} ${QT_LIBRARIES} ${DEPS})
