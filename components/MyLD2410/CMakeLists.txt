idf_component_register(SRC_DIRS .
                       INCLUDE_DIRS .
                       REQUIRES low_code system simple_uart)

target_include_directories(
    ${COMPONENT_LIB} PRIVATE ${COMPONENT_INCLUDES}
)