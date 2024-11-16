import os
import sys
import time

# Constants
RETRY_COUNT = 3
RETRY_DELAY = 1  # seconds

# Get the directory of the current script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Use the script directory to set up relative paths
scripts_dir = os.path.join(script_dir, "..", "Scripts")  # Directory where scripts are stored
template_plugin_file = os.path.join(script_dir, "plugin.cpp")  # Template file for the plugin
output_plugin_file = os.path.join(script_dir, "Generated", "plugin_generated.cpp")  # Output file for the generated plugin
include_marker = "// [INCLUDES]"  # Marker for includes
register_marker = "// [REGISTER]"  # Marker for registration

def get_script_classes(directory):
    # List all .hpp files in the directory and extract class names
    script_files = [f for f in os.listdir(directory) if f.endswith(".hpp")]
    return [os.path.splitext(f)[0] for f in script_files]

def generate_includes_and_registers(classes):
    # Generate include statements and component registration lines
    includes = [f'#include "{scripts_dir}/{cls}.hpp"' for cls in classes]
    registers = [f'\tRegisterComponent<{cls}>();' for cls in classes]
    return includes, registers

def update_plugin_file(template_path, output_path, includes, registers):
    for attempt in range(RETRY_COUNT):
        try:
            # Read the template plugin file
            with open(template_path, "r") as file:
                lines = file.readlines()

            # Ensure the output directory exists
            os.makedirs(os.path.dirname(output_path), exist_ok=True)

            # Write the updated content to the output plugin file
            with open(output_path, "w") as file:
                for line in lines:
                    if include_marker in line:
                        file.write(line)  # Write the include marker line
                        for include in includes:
                            file.write(include + "\n")  # Write includes
                    elif register_marker in line:
                        file.write(line)  # Write the register marker line
                        for register in registers:
                            file.write(register + "\n")  # Write registrations
                    else:
                        file.write(line)  # Write other lines unchanged
            
            print(f"{output_plugin_file} updated successfully.")  # Print success message
            return
        
        except PermissionError:
            print(f"Permission error occurred while updating {output_plugin_file}, retrying...")
            time.sleep(RETRY_DELAY)
    
    print(f"Failed to update {output_plugin_file} after {RETRY_COUNT} attempts.")

def main():
    script_classes = get_script_classes(scripts_dir)  # Get class names from scripts

    includes, registers = generate_includes_and_registers(script_classes)  # Generate includes and registers

    update_plugin_file(template_plugin_file, output_plugin_file, includes, registers)  # Update the plugin file

if __name__ == "__main__":
    main()  # Run the main function
