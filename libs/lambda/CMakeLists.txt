set(sources

)

add_library(lambda STATIC ${sources}
    report.h report.cpp
    lambda.h lambda.cpp)

target_include_directories(lambda
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
