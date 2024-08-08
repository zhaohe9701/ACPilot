set(COMMON_SOURCES
        Type/type.cpp
        Debug/default_debug.cpp
        )
if(STM32)
        set(INCLUDES
                ${INCLUDES}
                ${CMAKE_CURRENT_SOURCE_DIR}
                PARENT_SCOPE
                )

        set(SOURCES
                ${SOURCES}
                ${COMMON_SOURCES}
                PARENT_SCOPE
                )
else()
        idf_component_register(SRCS ${COMMON_SOURCES}
                INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
endif()