if(STM32)
        set(INCLUDES
                ${INCLUDES}
                ${CMAKE_CURRENT_SOURCE_DIR}
                PARENT_SCOPE
                )

else()
        idf_component_register(INCLUDE_DIRS "."
                REQUIRES "Osal")
endif()