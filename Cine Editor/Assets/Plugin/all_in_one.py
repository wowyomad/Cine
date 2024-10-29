import subprocess
import os
import platform
import argparse

def run_command(command):
    """Run a shell command and handle errors."""
    try:
        print(f"Running command: {command}")
        result = subprocess.run(command, check=True, shell=True)
        print(f"Command finished with exit code: {result.returncode}")
    except subprocess.CalledProcessError as e:
        print(f"Error occurred while running command: {command}")
        print(f"Exit code: {e.returncode}")
        exit(1)

def main():
    # Parse command-line arguments
    parser = argparse.ArgumentParser(description="Build script for CMake project.")
    parser.add_argument('--configuration', type=str, default='Debug', 
                        help="Build configuration (e.g., Debug, Release).")
    args = parser.parse_args()

    # Change the working directory to the script's directory
    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)
    print(f"Changed working directory to: {script_dir}")

    # Determine the correct path separator and CMake build directory
    if platform.system() == "Windows":
        build_dir = ".\\out\\build\\win32-msvc\\"
        python_cmd = "python"
    else:
        build_dir = "./out/build/unix/"
        python_cmd = "python3"  # Use 'python3' on Unix-like systems

    # List of commands to run
    commands = [
        f"{python_cmd} obtain_tools_paths.py ../Scripts/.vscode", 
        f"{python_cmd} generator.py",
        f"cmake --build {build_dir} --config {args.configuration}",  # Use the parameter here
        f"{python_cmd} copywriter.py"
    ]

    # Run each command
    for command in commands:
        run_command(command)

if __name__ == "__main__":
    main()
