import os
import sys
import yaml


def get_cmake_command(param_file):
    source_dir = os.path.realpath(os.path.dirname(__file__))
    default_file = os.path.join(source_dir, "./parameter_defaults.yml")
    with open(default_file, "r") as f:
        configuration_options = yaml.safe_load(f)

    with open(param_file, "r") as f:
        custom_options = yaml.safe_load(f)

    command = "cmake -DCMAKE_BUILD_TYPE=Release"
    for option in configuration_options.keys():
        if option in custom_options.keys():
            value = custom_options.pop(option)
        else:
            raise Exception(f"Parameter missing {option}")
        command += " -D{0}={1}".format(option, value)

    # check that all custom options were actually used
    if len(custom_options.keys()) > 0:
        for option in custom_options.keys():
            print("Unknown option:", option)
        exit()

    return command


def main():
    if len(sys.argv) < 2:
        print("Usage: python get_cmake_command.py <input_file.yml>")
        sys.exit(1)

    input_file = sys.argv[1]
    cmd = get_cmake_command(input_file)
    print(cmd)


if __name__ == "__main__":
    main()
