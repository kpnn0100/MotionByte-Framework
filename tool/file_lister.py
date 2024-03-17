import os
import fnmatch

def find_files(directory, pattern):
    """
    Recursively find files in a directory that match a specific pattern.

    Args:
        directory (str): The directory to start searching from.
        pattern (str): The pattern to match filenames against.

    Returns:
        List[str]: A list of absolute file paths matching the pattern.
    """
    file_list = []
    for root, _, files in os.walk(directory):
        for filename in files:
            if fnmatch.fnmatch(filename, pattern):
                file_list.append(filename)
    return file_list

def main():
    folder_path = "../MotionByte-1.0"
    if not os.path.isdir(folder_path):
        print("Invalid directory path.")
        return
    
    header_files = find_files(folder_path, "*.h")
    
    print("Header files found:")
    for file_path in header_files:
        print(file_path)

if __name__ == "__main__":
    main()
