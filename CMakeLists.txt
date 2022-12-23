# CMakeList.txt : fichier projet CMake de niveau supérieur, effectuez une configuration globale
# et incluez les sous-projets ici.
#
cmake_minimum_required (VERSION 3.8)

project ("AsyncCallQueue")

# Incluez les sous-projets.

cmake_minimum_required (VERSION 3.8)



add_executable (TestAsyncCallQueue "TestAsyncCallQueue.cpp")
IF (WIN32)
  # set stuff for windows
ELSE()
  target_link_libraries(TestAsyncCallQueue pthread)
ENDIF()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TestAsyncCallQueue PROPERTY CXX_STANDARD 20)
endif()