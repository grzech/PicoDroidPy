# Create an INTERFACE library for our C module.
add_library(usermod_wheel_driver INTERFACE)

# Add our source files to the lib
target_sources(usermod_wheel_driver INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/wheel_driver.c
    ${CMAKE_CURRENT_LIST_DIR}/wheel.c
)

# Add the current directory as an include directory.
target_include_directories(usermod_wheel_driver INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_wheel_driver)
