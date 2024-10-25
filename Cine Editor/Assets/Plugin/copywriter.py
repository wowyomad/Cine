import os
import shutil

def copy_files(config):
    # Define source and destination directories
    script_dir = os.path.dirname(os.path.abspath(__file__))
    source_dir = os.path.join(script_dir, 'lib', config)
    destination_dir = os.path.abspath(os.path.join(script_dir, '../../../', config))

    # Files to be copied
    dll_file = os.path.join(source_dir, 'plugin.dll')
    pdb_file = os.path.join(source_dir, 'plugin.pdb')

    # Ensure the source directory exists
    if not os.path.exists(source_dir):
        print(f"Source directory '{source_dir}' does not exist. Skipping configuration '{config}'.")
        return

    # Ensure the destination directory exists
    os.makedirs(destination_dir, exist_ok=True)

    # Copy the DLL file
    if os.path.isfile(dll_file):
        shutil.copy(dll_file, destination_dir)
        print(f"Copied '{dll_file}' to '{destination_dir}'")
    else:
        print(f"'{dll_file}' not found. Skipping DLL file for configuration '{config}'.")

    # Copy the PDB file if it exists

    #if os.path.isfile(pdb_file):
    #    shutil.copy(pdb_file, destination_dir)
    #    print(f"Copied '{pdb_file}' to '{destination_dir}'")
    #else:
    #    print(f"'{pdb_file}' not found. Skipping PDB file for configuration '{config}'.")

def main():
    # Check for available configurations in the 'lib' directory
    script_dir = os.path.dirname(os.path.abspath(__file__))
    lib_dir = os.path.join(script_dir, 'lib')

    if not os.path.exists(lib_dir):
        print(f"'lib' directory '{lib_dir}' does not exist.")
        return

    # Get all subdirectories in 'lib' (these are considered configurations)
    configs = [d for d in os.listdir(lib_dir) if os.path.isdir(os.path.join(lib_dir, d))]

    if not configs:
        print("No configurations found in 'lib' directory.")
        return

    # Copy files for each configuration
    for config in configs:
        copy_files(config)

if __name__ == "__main__":
    main()