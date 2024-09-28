function(set_undefined)
    set(variable_name "${ARGV0}")

    if(NOT DEFINED ${variable_name})
        set(${ARGV})
    endif()
endfunction()