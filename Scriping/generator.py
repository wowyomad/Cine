import os

# Configuration
scripts_dir = "Scripts"
template_plugin_file = "plugin.cpp"
output_plugin_file = "Generated/plugin_generated.cpp"
include_marker = "// [INCLUDES]"
register_marker = "// [REGISTER]"

def get_script_classes(directory):
    # Get all header files in the directory
    script_files = [f for f in os.listdir(directory) if f.endswith(".hpp")]
    # Extract class names (assuming each file represents a class with the same name)
    return [os.path.splitext(f)[0] for f in script_files]

def generate_includes_and_registers(classes):
    includes = [f'#include "{scripts_dir}/{cls}.hpp"' for cls in classes]
    registers = [f'\tRegisterComponent<{cls}>();' for cls in classes]
    return includes, registers

def update_plugin_file(template_path, output_path, includes, registers):
    with open(template_path, "r") as file:
        lines = file.readlines()

    # Create the Generated directory if it doesn't exist
    os.makedirs(os.path.dirname(output_path), exist_ok=True)

    # Find markers and replace with generated content
    with open(output_path, "w") as file:
        for line in lines:
            if include_marker in line:
                # Add generated includes after the marker
                file.write(line)
                for include in includes:
                    file.write(include + "\n")
            elif register_marker in line:
                # Add generated register calls after the marker
                file.write(line)
                for register in registers:
                    file.write(register + "\n")
            else:
                file.write(line)

def main():
    # Get list of script classes from the Scripts directory
    script_classes = get_script_classes(scripts_dir)

    # Generate the necessary include and registration lines
    includes, registers = generate_includes_and_registers(script_classes)

    # Update the plugin file using the template and output path
    update_plugin_file(template_plugin_file, output_plugin_file, includes, registers)

    print(f"{output_plugin_file} updated successfully.")

if __name__ == "__main__":
    main()
