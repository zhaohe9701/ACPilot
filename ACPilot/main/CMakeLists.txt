if(STM32)
set(MAIN_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/stm_main.c
        )

set(INCLUDES
        ${INCLUDES}
        ${CMAKE_CURRENT_SOURCE_DIR}
        PARENT_SCOPE
        )

set(SOURCES
        ${SOURCES}
        ${MAIN_SOURCES}
        PARENT_SCOPE
        )
else()
set(MAIN_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/esp_main.c
        )

set(INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

idf_component_register(SRCS ${MAIN_SOURCES}
        INCLUDE_DIRS ${INCLUDES})
endif()
