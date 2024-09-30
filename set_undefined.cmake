function(set_undefined)
    set(variable_name "${ARGV0}")

    if(NOT DEFINED${variable_name} OR NOT DEFINED CACHE${variable_name})
        set(${ARGV})
    else()
        message("The variable ${variable_name} was previusly defined")
    endif()
endfunction()