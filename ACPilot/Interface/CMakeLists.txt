set(LIBS_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Com/com_interface.cpp
        )
if(STM32)
        set(INCLUDES
                ${INCLUDES}
                ${CMAKE_CURRENT_SOURCE_DIR}
                PARENT_SCOPE
                )

        set(SOURCES
                ${SOURCES}
                ${LIBS_SOURCES}
                PARENT_SCOPE
                )
else()
        idf_component_register(SRCS ${LIBS_SOURCES}
                INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
                REQUIRES "Osal" "Utils")
endif()