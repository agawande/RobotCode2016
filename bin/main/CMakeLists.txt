add_executable (robot robot.cxx)

target_link_libraries (robot LINK_PUBLIC cage coupler conveyor messageformatter i2cdispatcher sorter)
