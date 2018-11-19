cmake_minimum_required(VERSION 3.8)
project(Project)

set(CMAKE_CXX_STANDARD 11)
set(DOC_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/doc)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11 -O2")
set(SOURCE_FILES main.cpp ConfigFileParser.cpp Particle.cpp Cell.cpp  InitialConditions.cpp  OctoTree_Serial.cpp Writter.cpp )

add_executable(Project ${SOURCE_FILES})


#Copies files for Doxygen and test running
file(COPY "Doxyfile" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY "atom_icon_192.ico" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(MAKE_DIRECTORY ${DOC_FOLDER})
find_package(Doxygen)
add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
file(COPY ${DOC_FOLDER}      DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
