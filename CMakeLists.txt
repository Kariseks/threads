cmake_minimum_required(VERSION 3.16)

project(threads LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# Główny target aplikacji
add_executable(threads main.cpp
    mutex_example.cpp
    quicksort.h)

# Target testowy dla Catch2
add_executable(tests test.cpp
    mutex_example.cpp)

# Znajdź i dołącz Catch2 (zakładam, że zainstalowane w systemie)
find_package(Catch2 REQUIRED)

# Linkuj Catch2 do targetu testowego
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)



include(GNUInstallDirs)
install(TARGETS threads
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
