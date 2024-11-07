import subprocess
import os
import json
import sys

def get_visual_studio_info():
    """Retrieve Visual Studio installation path and MSVC include directory."""
    vswhere_path = r"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
    
    # Run vswhere to get installation information
    try:
        output = subprocess.check_output([vswhere_path, '-format', 'json'], text=True)
    except subprocess.CalledProcessError as e:
        print("Error while running vswhere:", e)
        return None, None

    # Parse the output
    installations = json.loads(output)

    if not installations:
        print("No Visual Studio installations found.")
        return None, None

    # Extract the installation path
    install_path = installations[0]['installationPath']

    # Construct the path to the MSVC include directory
    msvc_include_path = os.path.join(install_path, "VC", "Tools", "MSVC")

    # Add the most recent version directory (in this case, 14.x)
    try:
        latest_version = max(os.listdir(msvc_include_path))
        msvc_include_path = os.path.join(msvc_include_path, latest_version, "include")
    except Exception as e:
        print("Error retrieving the latest MSVC version:", e)
        return None, None

    return install_path, msvc_include_path

def get_windows_sdk_include_path():
    """Retrieve the Windows SDK include directory dynamically."""
    windows_sdk_subkey = r"SOFTWARE\Microsoft\Windows Kits\Installed Roots"
    
    try:
        import winreg
        with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, windows_sdk_subkey) as reg_key:
            windows_sdk_root = winreg.QueryValueEx(reg_key, "KitsRoot10")[0]
            # Dynamically get the latest SDK version
            include_path = os.path.join(windows_sdk_root, "Include")
            if os.path.exists(include_path):
                sdk_versions = [d for d in os.listdir(include_path) if os.path.isdir(os.path.join(include_path, d))]
                if sdk_versions:
                    latest_sdk_version = max(sdk_versions)
                    return os.path.join(windows_sdk_root, "Include", latest_sdk_version, "ucrt")
    except Exception as e:
        print("Error retrieving Windows SDK include path:", e)
    
    return None

def generate_c_cpp_properties(output_dir):
    """Generate c_cpp_properties.json based on Visual Studio and Windows SDK information."""
    install_path, msvc_include_path = get_visual_studio_info()
    windows_sdk_include_path = get_windows_sdk_include_path()

    if not msvc_include_path or not windows_sdk_include_path:
        print("Could not obtain include paths.")
        return

    # Construct the JSON structure
    c_cpp_properties = {
        "configurations": [
            {
                "name": "Win32",
                "intelliSenseMode": "msvc-x64",
                "compilerPath": os.path.join(install_path, "VC", "Tools", "MSVC", max(os.listdir(os.path.join(install_path, "VC", "Tools", "MSVC"))), "bin", "Hostx64", "x64", "cl.exe"),
                "includePath": [
                    "${workspaceFolder}/**",
                    "${workspaceFolder}/../**",
                    msvc_include_path,
                    windows_sdk_include_path
                ],
                "defines": [
                    "_DEBUG",
                    "UNICODE",
                    "_UNICODE"
                ],
                "cppStandard": "c++23",
                "cStandard": "c11"
            }
        ],
        "version": 4
    }

    # Determine output path
    output_file_path = os.path.join(os.path.dirname(__file__), output_dir, 'c_cpp_properties.json')

    # Ensure the output directory exists
    os.makedirs(os.path.dirname(output_file_path), exist_ok=True)

    # Write to c_cpp_properties.json
    with open(output_file_path, 'w') as json_file:
        json.dump(c_cpp_properties, json_file, indent=4)

    print(f"c_cpp_properties.json generated successfully at {output_file_path}.")

# Execute the script
if __name__ == "__main__":
    # Check for output directory argument
    if len(sys.argv) < 2:
        print("Usage: python generator.py <output_directory>")
        sys.exit(1)

    output_directory = sys.argv[1]
    generate_c_cpp_properties(output_directory)
