import re
import os
from lxml import etree

"""
User Modifiable Constants
"""

projectName = 'EFM8UB1_Isolated_RS485'

# File path patterns for files to ignore (exclude from slsproj file tree)
ignore_list = ['generate_slsproj.py', '.slsproj', '\\bin']

# List of files to exclude from build
excludedPath_list = []

# Project compatibility
boardCompatibility = "brd5000a"
partCompatibility = "mcu.8051.efm8.ub1"
sdkCompatibility = "com.silabs.sdk.si8051.sls:[3.0.0-4.0.0)"

# Property key/value pairs
properties = { 'category' : 'Demos' }

# Module IDs
module_ids = [
"com.silabs.sdk.si8051.external.bsp.efm8_memory_lcd",
"com.silabs.sdk.si8051.shared",
"com.silabs.sdk.si8051.external.bsp.efm8_bsp",
"com.silabs.sdk.si8051.external.efm8PeripheralDriver.spi0"
]

"""
Constants
"""

slsproj_ext = '.slsproj'
contentRoot="."

"""
Utility Functions
"""

def natural_sort(l):
    """
    Natural sort (sort numbers like 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 and not 1, 10, 2, ...)
    :param l: list
    :return: naturally sorted list
    """
    convert = lambda text: int(text) if text.isdigit() else text.lower()
    alphanum_key = lambda key: [ convert(c) for c in re.split('([0-9]+)', key) ]
    return sorted(l, key = alphanum_key)

def get_folder_contents(folder, list_folders=True, list_files=True):
    """
    Get the contents of a folder
    :param folder: full path to a folder
    :param list_folders: return folders in the list
    :param list_files: return files in the list
    :return: a list of folder contents
    """
    folder_list = []
    for item in os.listdir(folder):
        item_path = os.path.join(folder, item)
        if list_folders and os.path.isdir(item_path):
            folder_list.append(item)
        if list_files and not os.path.isdir(item_path):
            folder_list.append(item)

    return folder_list

def string_contains_phrase(text, phrase_list):
    """
    Return True if a phrase in the phrase list is found in the search text
    :param text: String to search for matching phrases
    :type text: str
    :param phrase_list: A list of substring phrases to search for
    :type phrase_list: [str]
    :return: True if a match is found in the search string text
    """
    for phrase in phrase_list:
        if text.find(phrase) != -1:
            return True

    return False

"""
Main Script Functions
"""

def build_properties(tree_node):
    for key in properties:
        child = etree.Element('property')
        child.attrib['key'] = key
        child.attrib['value'] = properties[key]
        tree_node.append(child)

def build_file_tree(tree_node, root_folder, ignore_list):
    """
    Build the slsproj file tree recursively by adding file/folder
    children to the tree node
    :param tree_node: current etree node
    :param root_folder: full path to current folder
    :param ignore_list: a list of strings found in file paths for files
    to be ignored
    """

    for folder_name in natural_sort(get_folder_contents(root_folder, list_folders=True, list_files=False)):
        folder_path = os.path.join(root_folder, folder_name)

        # Skip over ignored folders
        if string_contains_phrase(folder_path, ignore_list):
            continue

        child = etree.Element('folder')
        child.attrib['name'] = folder_name
        child.attrib['uri'] = folder_name
        child.attrib['includeAllFiles'] = 'false'
        child.attrib['includeAllFolders'] = 'false'
        build_file_tree(child, folder_path, ignore_list)
        tree_node.append(child)

    for file_name in natural_sort(get_folder_contents(root_folder, list_folders=False, list_files=True)):
        file_path = os.path.join(root_folder, file_name)

        # Skip over ignored folders
        if string_contains_phrase(file_path, ignore_list):
            continue

        child = etree.Element('file')
        child.attrib['name'] = file_name
        tree_node.append(child)

def build_module_includes(root):
    for id in module_ids:
        child = etree.Element('module')
        child.attrib['id'] = id
        root.append(child)
        
def build_exclude_paths(root, folder, excludedPath_list):
    """
    Build <excludePath> slsproj elements
    :param root: root etree node
    :param folder: root folder containing all source files
    :param excludedPath_list: list of files to exclude from the build
    """
    for excluded_path in excludedPath_list:
        child = etree.Element('excludedPath')
        child.text = excluded_path
        root.append(child)

def build_include_paths(root, folder, ignore_list):
    """
    Build <includePath> slsproj elements by searching for .h files
    :param root: root etree node
    :param folder: root folder containing all source files
    :param ignore_list: a list of strings found in file paths for files
    to be ignored
    """
    include_path_list = []
    for dirpath, dirnames, filenames in os.walk(folder):
        for filename in filenames:
            if filename.endswith('.h'):
                relative_path = os.path.relpath(dirpath, folder)
                relative_path = relative_path.replace('\\', '/') + '/'
                include_path_list.append(relative_path)
                break

    for include_path in natural_sort(include_path_list):
        child = etree.Element('includePath')
        child.attrib['languageCompatibility'] = 'c'
        child.attrib['uri'] = 'studio:/project/{0}'.format(include_path)
        root.append(child)

def generate_project(project_name, folder):
    """
    Generate an slsproj file from existing source in folder
    :param project_name: the name of the project
    :param folder: source location for files located in the slsproj
    """
    global ignore_list

    root = etree.Element('project')
    root.attrib['name'] = project_name
    root.attrib['boardCompatibility'] = boardCompatibility
    root.attrib['partCompatibility'] = partCompatibility
    root.attrib['contentRoot'] = contentRoot
    
    build_properties(root)
    build_file_tree(root, folder, ignore_list)
    build_module_includes(root)
    build_exclude_paths(root, folder, excludedPath_list)
    build_include_paths(root, folder, ignore_list)

    # Create a document around the root node and output the xml file with full xml declarations
    doc = etree.ElementTree(root)
    full_filename = os.path.join(folder, project_name + slsproj_ext)
    doc.write(full_filename, xml_declaration=True, pretty_print=True, encoding='UTF-8')

"""
Main
"""

if __name__ == "__main__":
    folder_path = os.path.abspath(os.path.curdir)

    generate_project(projectName, folder_path)
