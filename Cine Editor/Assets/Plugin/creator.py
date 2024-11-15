import os
import sys

def create_header_file(file_name):
    # Get the directory of the current script
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # Extract the base name without the extension
    base_name = os.path.splitext(file_name)[0]
    hpp_file_name = base_name + '.hpp'

    # Target directory for the file, relative to the script's directory
    target_dir = os.path.join(script_dir, "..", "Scripts")
    os.makedirs(target_dir, exist_ok=True)

    # Full path for the .hpp file
    hpp_file_path = os.path.join(target_dir, hpp_file_name)

    # Check if the file already exists (case-insensitive)
    existing_files = {f.lower() for f in os.listdir(target_dir)}
    if hpp_file_name.lower() in existing_files:
        print(f"File '{hpp_file_name}' already exists in '{target_dir}'. Aborting.")
        return

    # Class template
    class_template = f"""#pragma once
#include "include/ScriptCore.hpp"

using namespace Cine;

class {base_name} : public NativeScript
{{
    public:
        void OnCreate() override
        {{
            
        }}

        void OnUpdate(Timestep ts) override
        {{
            
        }}

        // Assign fields that you expect to be serialized (don't assign pointers or references!)
        {base_name}& operator=(const {base_name}& other)
        {{
            if (this == &other)
                return *this;
            
            NativeScript::operator=(other);
            return *this;
        }}

    private:


        SERIALIZE_CLASS({base_name},

        )
}};
"""

    # Write the template to the .hpp file
    with open(hpp_file_path, 'w') as hpp_file:
        hpp_file.write(class_template)

    print(f"Header file '{hpp_file_path}' created successfully.")

if __name__ == "__main__":
    # Check if a file name is provided as an argument
    if len(sys.argv) < 2:
        print("Usage: python create_header.py <FileName>")
        sys.exit(1)

    # Get the file name from the command line argument
    file_name = sys.argv[1]
    create_header_file(file_name)
