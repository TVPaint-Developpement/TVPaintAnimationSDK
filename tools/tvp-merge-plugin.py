import argparse
import logging
import shutil
import sys

from dataclasses import dataclass
from hashlib import sha256
from pathlib import Path


logger = logging.getLogger(__name__)

EXCLUDE_FILES = ['.DS_Store', 'Thumbs.db', 'desktop.ini']
SUPPORTED_LANGUAGES = ['chinese', 'english', 'french', 'japanese']


class CleanupOnError:
    """
    Manages resource cleanup upon encountering an error in a context.

    This class is designed to ensure that specific resources, such as directories,
    are cleaned up (i.e., removed) if an exception occurs within a context block
    where this class is used. It provides a convenient and reliable mechanism for
    handling resource cleanup during error scenarios.

    :ivar output_path: The filesystem path to be cleaned up upon encountering an error.
    :type output_path: Path
    """
    def __init__(self, output_path: Path):
        self.output_path = output_path

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        if exc_type is not None:
            shutil.rmtree(self.output_path, ignore_errors=True)
            logger.info(f"Error occurred, removing incomplete plugin: {self.output_path}")


@dataclass
class Plugin:
    """
    Represents a plugin with its associated metadata and properties.

    This class encapsulates the properties and attributes of a plugin,
    such as its file paths, operating system, binary details, resources
    checksum, and configuration information. It is designed to provide
    structured access to plugin-related data for various application
    modules.

    :ivar path: The path where the plugin is located.
    :type path: Path
    :ivar os_name: The name of the operating system the plugin is for.
    :type os_name: str
    :ivar binary_name: The name of the plugin binary file.
    :type binary_name: str
    :ivar binary_path: The path of the plugin binary file.
    :type binary_path: Path
    :ivar resources_checksum: A dictionary mapping resource file paths
        to their respective checksums for integrity verification.
    :type resources_checksum: dict[Path, str]
    :ivar has_plist: Indicates whether the plugin contains a plist
        (property list) configuration file.
    :type has_plist: bool
    """
    path: Path
    os_name: str
    binary_name: str
    binary_path: Path
    resources_checksum: dict[Path, str]
    has_plist: bool


def validate_args(output: Path, inputs: list[Path]) -> None:
    """
    Validates the provided output and input paths according to the required rules.

    This function ensures the output path does not already exist and has the correct
    ".plugin" extension. It also validates that there are 2 or 3 input paths, all of
    which must be directories with the ".plugin" extension.

    :param output: The output file path to validate.
    :param inputs: The list of input file paths to validate.
    :raises ValueError: If any validation condition is not met, including:
        - Output file already exists.
        - Output file does not have ".plugin" extension.
        - Input file count is not exactly 2 or 3.
        - Any input file does not exist, does not have the ".plugin" extension, or is not a directory.
    """
    if output.exists():
        raise ValueError(f'Output file "{output}" already exists.')
    if output.suffix != '.plugin':
        raise ValueError(f'Output file "{output}" must have ".plugin" extension.')

    if not (2 <= len(inputs) <= 3):
        raise ValueError(f'Expected 2 or 3 input files, got {len(inputs)}.')
    for i in inputs:
        if not i.exists():
            raise ValueError(f'Input file "{i}" does not exist.')
        if i.suffix != '.plugin':
            raise ValueError(f'Input file "{i}" must have ".plugin" extension.')
        if not i.is_dir():
            raise ValueError(f'Input file "{i}" is not a directory.')


def validate_binary(binary_directory: Path, os_name: str) -> tuple[str, Path]:
    """
    Validates the given directory to ensure it contains exactly one binary file.

    This function inspects the directory specified by the input path and verifies
    that it includes exactly one binary file. If the condition is not met, an
    exception is raised. The function returns the name and the resolved absolute
    path of the binary.

    :param binary_directory: Path representing the directory containing the binary file.
    :param os_name: The name of the operating system for which the binary is intended.
    :return: A tuple containing the name of the binary file as a string and its resolved
        absolute path as a `Path` object.
    :raises ValueError: If the directory does not contain exactly one binary file.
    """
    binaries = [b for b in binary_directory.iterdir() if b.is_file() and b.name not in EXCLUDE_FILES]
    if len(binaries) != 1:
        raise ValueError(f'Expected exactly 1 binary in "{binary_directory}", got {len(binaries)}.')

    if os_name == 'Linux':
        binary_name = binaries[0].stem.removeprefix('lib')
    elif os_name == 'Windows':
        binary_name = binaries[0].stem
    else:
        binary_name = binaries[0].name

    return binary_name, binaries[0].resolve()


def collect_resources(p: Path, resources_checksum: dict[Path, str]) -> None:
    """
    Collects resources from a given directory and calculates their checksum.

    This function iterates through the provided directory path to find files
    associated with supported languages. It computes the SHA-256 checksum for
    each valid file and stores it in a dictionary with the file's path relative
    to the root directory as the key. Unsupported languages and unexpected files
    trigger an exception.

    :param p: The root directory to search for resources.
    :param resources_checksum: A mapping of file paths (relative to the root
                                directory) to their corresponding SHA-256 checksum.
    :raises ValueError: If an unexpected file is found in the directory or if an
                        unsupported language directory is encountered.
    """
    for r in p.iterdir():
        if r.is_file() and r.name in EXCLUDE_FILES:
            continue
        elif r.is_file():
            raise ValueError(f'Unexpected file "{r}" in "{p}".')

        if r.is_dir() and r.name not in SUPPORTED_LANGUAGES:
            raise ValueError(f'Unsupported language "{r.name}" in "{r}".')

        for f in r.iterdir():
            if f.is_file() and f.name in EXCLUDE_FILES:
                continue
            elif f.is_dir():
                raise ValueError(f'Unexpected directory "{f}" in "{r}".')

            resources_checksum[f.relative_to(p)] = sha256(f.read_bytes()).hexdigest()


def validate_plugin_structure(plugin_path: Path) -> Plugin:
    """
    Validates the structure of a plugin directory and retrieves critical details such as operating system type,
    binary information, resource checksums, and the presence of an Info.plist file. Ensures that the plugin
    directory adheres to the required layout and contains necessary components.

    :param plugin_path: The path to the root directory of the plugin to be validated.
    :return: An instance of the Plugin class encapsulating details about the validated plugin structure,
             including OS name, binary name and path, resource checksums, and the presence of an Info.plist file.
    :raises ValueError: If the directory structure does not meet validation requirements, such as missing a
                        "Contents" directory or containing more than one root-level directory inside the plugin
                        path.
    """
    logger.info(f'Validating plugin structure in {plugin_path}')

    contents_directory_path = [p for p in plugin_path.iterdir() if p.name not in EXCLUDE_FILES]
    if len(contents_directory_path) != 1:
        raise ValueError(f'Expected exactly 1 directory in "{plugin_path}", got {len(contents_directory_path)}.')
    if contents_directory_path[0].name != 'Contents':
        raise ValueError(f'Expected "Contents" directory in "{plugin_path}", got "{contents_directory_path[0].name}".')

    os_name = None
    os_count = 0
    binary_name = None
    binary_path = None
    resources_checksum = {}
    has_plist = False

    for p in contents_directory_path[0].iterdir():
        if p.name in {'MacOS', 'Windows', 'Linux'}:
            os_count += 1
            os_name = p.name
            binary_name, binary_path = validate_binary(p, os_name)
        if p.name == 'Resources':
            collect_resources(p, resources_checksum)
        if p.name == 'Info.plist':
            has_plist = True

    if os_count > 1:
        raise ValueError(f'Expected exactly 1 OS directory in "{plugin_path}", got {os_count}.')

    if os_name is None:
        raise ValueError(f'Expected OS directory in "{plugin_path}", got none.')

    return Plugin(plugin_path, os_name, binary_name, binary_path, resources_checksum, has_plist)


def validate_plugins_structure(inputs: list[Path]) -> list[Plugin]:
    """
    Validates the structure of multiple plugins.

    The function iterates over a list of file paths and validates the structure
    of each plugin by invoking `validate_plugin_structure` on each path. It
    returns a list of validated plugins.

    :param inputs: A list of paths to the plugin directories to be validated.
    :return: A list of validated plugins.
    """
    return [validate_plugin_structure(i) for i in inputs]


def validate_compatibility(plugins: list[Plugin]) -> None:
    """
    Validates the compatibility of a list of plugins by performing a series of checks for
    unique OS names, consistent binary names, and matching resource checksums.

    :param plugins: A list of Plugin objects to validate. Each Plugin must have properties
        such as `os_name`, `binary_name`, `path`, and `resources_checksum`.
    :raises ValueError: If duplicate OS names are found among the plugins.
    :raises ValueError: If the binary names of the plugins are inconsistent.
    :raises ValueError: If mismatches are detected in the resource checksums.
    """
    os_name_seen = set()
    for p in plugins:
        if p.os_name in os_name_seen:
            raise ValueError(f'Duplicate OS name "{p.os_name}" detected in {p.path}.')
        os_name_seen.add(p.os_name)

    binary_name_ref = plugins[0].binary_name
    for p in plugins:
        if p.binary_name != binary_name_ref:
            raise ValueError(f'Binary name mismatch detected in {p.path}: "{p.binary_name}" instead of "{binary_name_ref}".')

    resources: dict[str, set[str]] = {}
    for p in plugins:
        for k, v in p.resources_checksum.items():
            resources.setdefault(k, set()).add(v)

    for k, v in resources.items():
        if len(v) > 1:
            raise ValueError(f'Resource checksum mismatch: {k}')

    logger.info('Plugin structure and compatibility validated successfully.')


def copy_os_binary(output_path: Path, plugins: list[Plugin]) -> None:
    """
    Copies OS-specific binary files from each plugin's specified directory to the output location.

    This function iterates through a list of plugins and copies their operating system-specific binary
    directories to the corresponding location in the output path.

    :param output_path: The destination directory where OS-specific binaries will be copied.
    :param plugins: A list of Plugin instances, each containing the path and OS-specific information
                    required to locate the binaries.
    """
    for p in plugins:
        shutil.copytree(p.path / 'Contents' / p.os_name, output_path / 'Contents' / p.os_name)
        logger.info(f'Copied OS binary for {p.os_name} from {p.path}')


def merge_resources(output_path: Path, plugins: list[Plugin]) -> None:
    """
    Merges resources from multiple plugin instances into a single destination path while avoiding
    duplicates.

    :param output_path: Path to the output directory where all plugin resources will be merged.
    :param plugins: List of Plugin instances whose resources are to be merged. Each plugin contains
        information about its resources and associated checksum to ensure uniqueness.
    """
    output_resource_path = output_path / 'Contents' / 'Resources'
    output_resource_path.mkdir(parents=True, exist_ok=True)

    resources_already_copy: set[Path] = set()
    for p in plugins:
        for k, v in p.resources_checksum.items():
            if not (output_resource_path / k.parent).exists():
                output_lang_path = output_resource_path / k.parent
                output_lang_path.mkdir(parents=True, exist_ok=True)

            if k in resources_already_copy:
                continue
            resources_already_copy.add(k)

            shutil.copy2(p.path / 'Contents' / 'Resources' / k, output_path / 'Contents' / 'Resources' / k)
            logger.info(f'Copied resource "{k}" from {p.path}')


def copy_info_plist(output_path: Path, plugins: list[Plugin]) -> None:
    """
    Copies an Info.plist file from the first plugin found with an existing plist to the specified
    output path. If no plugin contains an Info.plist, the function logs a message indicating this.

    :param output_path: The destination directory where the Info.plist file will be copied.
    :param plugins: A list of plugins to search for an existing Info.plist file.
    """
    for p in plugins:
        if p.has_plist:
            shutil.copy2(p.path / 'Contents' / 'Info.plist', output_path / 'Contents')
            logger.info(f'Copied Info.plist from {p.path}')
            return
    logger.info('No Info.plist found in input plugins, skipping.')


def build_universal_plugin(output_path: Path, plugins: list[Plugin]) -> None:
    """
    Builds a universal plugin by combining multiple plugin resources and binaries into a single
    output location, ensuring proper directory structure compatible with the plugin framework.

    :param output_path: The target path where the universal plugin will be created.
    :param plugins: A list of Plugin objects to be included in the universal plugin.
    """
    contents_path = output_path / 'Contents'
    contents_path.mkdir(parents=True, exist_ok=True)

    copy_os_binary(output_path, plugins)
    merge_resources(output_path, plugins)
    copy_info_plist(output_path, plugins)


def main() -> int:
    parser = argparse.ArgumentParser(
        description='Merge multiple TVPaint .plugin bundles into a single universal bundle.',
        epilog='''
Example:
    %(prog)s PI_Flip-Universal.plugin PI_Flip-MacOS.plugin PI_Flip-Windows.plugin PI_Flip-Linux.plugin

This will create PI_Flip-Universal.plugin containing binaries for all three platforms.
        ''')

    parser.add_argument('output', type=Path, help='Output universal plugin path (e.g., MyPlugin-Universal.plugin)')
    parser.add_argument('inputs', type=Path, nargs='+', help='Input plugin paths (e.g., MyPlugin-MacOS.plugin MyPlugin-Windows.plugin MyPlugin-Linux.plugin)')
    parser.add_argument('--dry-run', action='store_true', help='Validate inputs and show what would be done without creating output')
    parser.add_argument('--verbose', action='store_true', help='Show verbose output')

    args = parser.parse_args()

    if args.dry_run: args.verbose = True

    logging.basicConfig(level=logging.INFO if args.verbose else logging.WARNING, format='%(levelname)s: %(message)s')

    try:
        validate_args(args.output, args.inputs)
        plugins = validate_plugins_structure(args.inputs)
        validate_compatibility(plugins)

        if args.dry_run:
            logger.info('Dry run completed successfully.')
            inputs_names = [n.name for n in args.inputs]
            logger.info(f'Would merge {", ".join(inputs_names)} into {args.output}.')
            return 0

        with CleanupOnError(args.output):
            build_universal_plugin(args.output, plugins)
            logger.info(f'Creation of the {args.output} plugin completed successfully.')
    except ValueError as e:
        logger.error(e)
        return 1

    return 0


if __name__ == '__main__':
    sys.exit(main())
