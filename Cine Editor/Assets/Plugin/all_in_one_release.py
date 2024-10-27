import subprocess
import os
import platform

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
        # Make sure to change to your actual build directory structure for Unix

    # List of commands to run
    commands = [
        f"{python_cmd} generator.py",
        f"cmake --build {build_dir} --config Release",
        f"{python_cmd} copywriter.py"
    ]

    # Run each command
    for command in commands:
        run_command(command)

if __name__ == "__main__":
    main()
