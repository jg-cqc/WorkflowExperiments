
########################################
# Using the ConfigFile Approach
########################################

add_executable(qo_onboard_simple_cli_c
               qo_hexdump.c
               qo_hexdump.h
               qo_onboard_simple_cli_c.c)

target_link_libraries(qo_onboard_simple_cli_c
                      PRIVATE
                      qo_onboard_c)

# Install qo_onboard_simple_cli_c binary
install(TARGETS qo_onboard_simple_cli_c
        RUNTIME DESTINATION bin)


########################################
# Using the SetOptions Approach
########################################

add_executable(qo_onboard_simple_cli_using_setoptions_c
               qo_hexdump.c
               qo_hexdump.h
               qo_onboard_simple_cli_using_setoptions_c.c)

target_link_libraries(qo_onboard_simple_cli_using_setoptions_c
                      PRIVATE
                      qo_onboard_c)

# Install qo_onboard_simple_cli_using_setoptions_c binary
install(TARGETS qo_onboard_simple_cli_using_setoptions_c
        RUNTIME DESTINATION bin)
