#!/usr/bin/python
#
import os
import os.path
import sys
import re
from sys import platform as _platform
import operator

class Utility:
    CONSOLE_RED = '\033[91m'
    CONSOLE_BLUE = '\033[94m'
    CONSOLE_YELLOW = '\033[93m'
    CONSOLE_END = '\033[0m'

    @staticmethod
    def save_to_file(file_name, text):
        Utility.delete_file_if_exists(file_name)
        
        with open(file_name, "w") as text_file:
            text_file.write(text)

    @staticmethod
    def change_working_directory_to_script_location():
        absolutePath = os.path.abspath(__file__)
        dirName = os.path.dirname(absolutePath)
        os.chdir(dirName)
        
    @staticmethod
    def is_valid_file(file_path):
        return os.path.isfile(file_path)
    
    @staticmethod
    def is_valid_path(file_path):
        return os.path.isdir(file_path)

    @staticmethod
    def delete_file_if_exists(file_path):
        try:
            os.remove(file_path)
        except OSError:
            pass

    @staticmethod
    def write_colour_message(message, colour_code):
        if _platform == "linux" or _platform == "linux2":
            print(colour_code + message + Utility.CONSOLE_END)
        elif _platform == "win32":
            os.system("echo " + message)
            
    @staticmethod
    def write_trace_message(message):
        Utility.write_colour_message(message, Utility.CONSOLE_BLUE)

    @staticmethod
    def write_error_message(message):
        Utility.write_colour_message(message, Utility.CONSOLE_RED)

    @staticmethod
    def write_info_message(message):
        Utility.write_colour_message(message, Utility.CONSOLE_YELLOW)

    @staticmethod
    def exists_in_string_array(haystack, needle):
        ret = False

        for item in haystack:
            item_as_string = str(item)
            if item_as_string in needle:
                return True
        return ret

class Minifier:
    def __init__(self):
        self.content = ""

    def process_input_file(self, input_file_name):
        if Utility.is_valid_file(input_file_name) is False:
            Utility.write_error_message("Invalid input file :" + input_file_name )

        excluding = False

        with open(input_file_name) as fp:
            for line in fp:
                
                line = line.strip()
                
                if "MINIFIER_EXCLUDE_START" in line:
                    excluding = True
                    continue

                if "MINIFIER_EXCLUDE_END" in line:
                    excluding = False
                    continue

                if excluding is True:
                    continue

                if line.startswith("//") == True:
                    continue

                if "//" in line:
                    line = line.split("//")[0]

                self.content += line
                    
    def save_to(self, output_file_name):
        Utility.save_to_file(output_file_name, self.content)

def display_usage():
    print('usage : python minifier.py <input_file>')

def main():
    ret_val = 0
    try:
        input_file = sys.argv[1] if len(sys.argv) > 1 else "input.html"

        Utility.change_working_directory_to_script_location()

        minifier = Minifier()
        minifier.process_input_file(input_file)
        minifier.save_to("output.html")
    except ValueError as err:
        Utility.write_error_message( str(err))
        ret_val = -1
    except Exception as ex:
        Utility.write_error_message( str(ex))
        ret_val = -1    
    finally:
        return ret_val

# Entry point
if __name__ == "__main__":
    main()