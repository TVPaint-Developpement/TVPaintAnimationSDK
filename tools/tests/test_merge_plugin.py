import os
import pytest
import shutil

from tvp_merge_plugin import validate_args, validate_plugin_structure, validate_plugins_structure, validate_compatibility, build_universal_plugin, CleanupOnError

RESOURCES = {"english/strings.txt": "hello world",
             "french/strings.txt": "bonjour le monde",
             "english/icon.png": b'\x89PNG fake image'}


# ==================================================================================================================================================================================
# ====================================================================== Tests for validate_args function ==========================================================================
# ==================================================================================================================================================================================
def test_validate_args_valid_case(tmp_path):
    output_plugin = tmp_path / 'output.plugin'

    input_plugin = tmp_path / 'input.plugin'
    input_plugin.mkdir()

    validate_args(output_plugin, [input_plugin, input_plugin])
    validate_args(output_plugin, [input_plugin, input_plugin, input_plugin])


def test_validate_args_output_already_exist(tmp_path):
    output_plugin = tmp_path / 'output.plugin'
    output_plugin.mkdir()

    input_plugin = tmp_path / 'input.plugin'
    input_plugin.mkdir()

    with pytest.raises(ValueError):
        validate_args(output_plugin, [input_plugin, input_plugin])


def test_validate_args_output_without_plugin_extension(tmp_path):
    output_plugin = tmp_path / 'output'

    input_plugin = tmp_path / 'input.plugin'
    input_plugin.mkdir()

    with pytest.raises(ValueError):
        validate_args(output_plugin, [input_plugin, input_plugin])


def test_validate_args_one_input(tmp_path):
    output_plugin = tmp_path / 'output.plugin'

    input_plugin = tmp_path / 'input.plugin'
    input_plugin.mkdir()

    with pytest.raises(ValueError):
        validate_args(output_plugin, [input_plugin])


def test_validate_args_four_inputs(tmp_path):
    output_plugin = tmp_path / 'output.plugin'

    input_plugin = tmp_path / 'input.plugin'
    input_plugin.mkdir()

    with pytest.raises(ValueError):
        validate_args(output_plugin, [input_plugin, input_plugin, input_plugin, input_plugin])


def test_validate_args_input_not_exist(tmp_path):
    output_plugin = tmp_path / 'output.plugin'

    input_plugin = tmp_path / 'input.plugin'
    input_plugin.mkdir()

    input_plugin_bad = tmp_path / 'input_bad.plugin'

    with pytest.raises(ValueError):
        validate_args(output_plugin, [input_plugin, input_plugin_bad])


def test_validate_args_input_without_plugin_extension(tmp_path):
    output_plugin = tmp_path / 'output.plugin'

    input_plugin = tmp_path / 'input.plugin'
    input_plugin.mkdir()

    input_plugin_bad = tmp_path / 'input_bad'
    input_plugin_bad.mkdir()

    with pytest.raises(ValueError):
        validate_args(output_plugin, [input_plugin, input_plugin_bad])


def test_validate_args_input_is_file(tmp_path):
    output_plugin = tmp_path / 'output.plugin'

    input_plugin = tmp_path / 'input.plugin'
    input_plugin.mkdir()

    input_plugin_bad = tmp_path / 'input_bad.plugin'
    input_plugin_bad.touch()

    with pytest.raises(ValueError):
        validate_args(output_plugin, [input_plugin, input_plugin_bad])


# ==================================================================================================================================================================================
# =================================================================== Tests for validate_plugin_structure ==========================================================================
# ==================================================================================================================================================================================
def test_validate_plugin_structure_valid_case(make_plugin):
    macos_plugin = make_plugin("input", "MacOS")
    linux_plugin = make_plugin("input", "Linux")
    windows_plugin = make_plugin("input", "Windows")

    validate_plugin_structure(macos_plugin)
    validate_plugin_structure(linux_plugin)
    validate_plugin_structure(windows_plugin)


def test_validate_plugin_structure_missing_contents(make_plugin):
    plugin = make_plugin("input", "MacOS")

    content_dir = plugin / 'Contents'
    shutil.rmtree(content_dir)

    with pytest.raises(ValueError):
        validate_plugin_structure(plugin)


def test_validate_plugin_structure_missing_os_dir(make_plugin):
    plugin = make_plugin("input", "MacOS")
    os_dir = plugin / 'Contents' / 'MacOS'
    shutil.rmtree(os_dir)

    with pytest.raises(ValueError):
        validate_plugin_structure(plugin)


def test_validate_plugin_structure_multiple_os_dirs(make_plugin):
    plugin = make_plugin("input", "MacOS")

    os_dir = plugin / 'Contents' / 'Linux'
    os_dir.mkdir(parents=True)

    with pytest.raises(ValueError):
        validate_plugin_structure(plugin)


def test_validate_plugin_structure_missing_binary(make_plugin):
    plugin = make_plugin("input", "MacOS")
    binary_path = plugin / 'Contents' / 'MacOS' / 'input'
    binary_path.unlink()

    with pytest.raises(ValueError):
        validate_plugin_structure(plugin)


def test_validate_plugin_structure_multiple_binaries(make_plugin):
    plugin = make_plugin("input", "MacOS")
    os_dir = plugin / 'Contents' / 'MacOS'
    (os_dir / 'input_bad').write_bytes(b'fake binary')

    with pytest.raises(ValueError):
        validate_plugin_structure(plugin)


def test_validate_plugin_structure_bad_language_file_name(make_plugin):
    plugin = make_plugin("input", "MacOS", resources=RESOURCES)
    os.rename(plugin / 'Contents' / 'Resources' / 'english' / 'strings.txt', plugin / 'Contents' / 'Resources' / 'english' / 'strings_bad.txt')

    with pytest.raises(ValueError):
        validate_plugin_structure(plugin)


def test_validate_plugin_structure_bad_resource_file_extension(make_plugin):
    plugin = make_plugin("input", "MacOS", resources=RESOURCES)
    os.rename(plugin / 'Contents' / 'Resources' / 'english' / 'icon.png', plugin / 'Contents' / 'Resources' / 'english' / 'icon.jpg')

    with pytest.raises(ValueError):
        validate_plugin_structure(plugin)


def test_validate_plugin_structure_unexpected_resource_folder(make_plugin):
    plugin = make_plugin("input", "MacOS", resources=RESOURCES)
    (plugin / 'Contents' / 'Resources' / 'german').mkdir()

    with pytest.raises(ValueError):
        validate_plugin_structure(plugin)


# ==================================================================================================================================================================================
# ====================================================================== Tests for validate_compatibility ==========================================================================
# ==================================================================================================================================================================================
def test_validate_compatibility_valid_case(make_plugin):
    macos_plugin_path = make_plugin("input", "MacOS", has_plist=True, resources=RESOURCES)
    linux_plugin_path = make_plugin("input", "Linux", resources=RESOURCES)
    windows_plugin_path = make_plugin("input", "Windows")

    macos_plugin = validate_plugin_structure(macos_plugin_path)
    linux_plugin = validate_plugin_structure(linux_plugin_path)
    windows_plugin = validate_plugin_structure(windows_plugin_path)

    validate_compatibility([macos_plugin, linux_plugin, windows_plugin])
    validate_compatibility([linux_plugin, windows_plugin])


def test_validate_compatibility_duplicate_os_name(make_plugin):
    macos_plugin_path = make_plugin("input", "MacOS")
    macos_plugin_path_bad = make_plugin("input-bad", "MacOS")

    macos_plugin = validate_plugin_structure(macos_plugin_path)
    macos_plugin_bad = validate_plugin_structure(macos_plugin_path_bad)

    with pytest.raises(ValueError):
        validate_compatibility([macos_plugin, macos_plugin_bad])


def test_validate_compatibility_binary_mismatch(make_plugin):
    macos_plugin_path = make_plugin("input", "MacOS")
    linux_plugin_path = make_plugin("input", "Linux")

    os.rename(linux_plugin_path / 'Contents' / 'Linux' / 'libinput.so', linux_plugin_path / 'Contents' / 'Linux' / 'libinput_bad.so')

    macos_plugin = validate_plugin_structure(macos_plugin_path)
    linux_plugin_bad = validate_plugin_structure(linux_plugin_path)

    with pytest.raises(ValueError):
        validate_compatibility([macos_plugin, linux_plugin_bad])


def test_validate_compatibility_resources_mismatch(make_plugin):
    macos_plugin_path = make_plugin("input", "MacOS", resources=RESOURCES)
    linux_plugin_path = make_plugin("input", "Linux", resources={"english/strings.txt": "bonjour le monde"})

    macos_plugin = validate_plugin_structure(macos_plugin_path)
    linux_plugin_bad = validate_plugin_structure(linux_plugin_path)

    with pytest.raises(ValueError):
        validate_compatibility([macos_plugin, linux_plugin_bad])


# ==================================================================================================================================================================================
# ====================================================================== Tests for build_universal_plugin ==========================================================================
# ==================================================================================================================================================================================
def test_build_universal_plugin(make_plugin, tmp_path):
    macos_plugin_path = make_plugin("input", "MacOS", has_plist=True, resources=RESOURCES)
    linux_plugin_path = make_plugin("input", "Linux", resources=RESOURCES)
    windows_plugin_path = make_plugin("input", "Windows")

    plugins = validate_plugins_structure([macos_plugin_path, linux_plugin_path, windows_plugin_path])
    validate_compatibility(plugins)

    output_path = tmp_path / 'output-universal'
    build_universal_plugin(output_path, plugins)

    assert output_path.exists()
    assert (output_path / 'Contents').exists()
    assert (output_path / 'Contents' / 'MacOS').exists()
    assert (output_path / 'Contents' / 'MacOS' / 'input').exists()
    assert (output_path / 'Contents' / 'Linux').exists()
    assert (output_path / 'Contents' / 'Linux' / 'libinput.so').exists()
    assert (output_path / 'Contents' / 'Windows').exists()
    assert (output_path / 'Contents' / 'Windows' / 'input.dll').exists()
    assert (output_path / 'Contents' / 'Resources').exists()
    assert (output_path / 'Contents' / 'Info.plist').exists()
    assert (output_path / 'Contents' / 'Resources' / 'english' / 'strings.txt').exists()
    assert (output_path / 'Contents' / 'Resources' / 'english' / 'icon.png').exists()
    assert (output_path / 'Contents' / 'Resources' / 'french' / 'strings.txt').exists()

    assert (output_path / 'Contents' / 'Info.plist').read_text() == (macos_plugin_path / 'Contents' / 'Info.plist').read_text()
    assert (output_path / 'Contents' / 'Resources' / 'english' / 'strings.txt').read_text() == (
                macos_plugin_path / 'Contents' / 'Resources' / 'english' / 'strings.txt').read_text()
    assert (output_path / 'Contents' / 'Resources' / 'english' / 'icon.png').read_bytes() == (macos_plugin_path / 'Contents' / 'Resources' / 'english' / 'icon.png').read_bytes()
    assert (output_path / 'Contents' / 'Resources' / 'french' / 'strings.txt').read_text() == (macos_plugin_path / 'Contents' / 'Resources' / 'french' / 'strings.txt').read_text()


def test_build_universal_plugin_without_macos(make_plugin, tmp_path):
    linux_plugin_path = make_plugin("input", "Linux", resources=RESOURCES)
    windows_plugin_path = make_plugin("input", "Windows")

    plugins = validate_plugins_structure([linux_plugin_path, windows_plugin_path])
    validate_compatibility(plugins)

    output_path = tmp_path / 'output-universal'
    build_universal_plugin(output_path, plugins)

    assert output_path.exists()
    assert (output_path / 'Contents').exists()
    assert (output_path / 'Contents' / 'Linux').exists()
    assert (output_path / 'Contents' / 'Linux' / 'libinput.so').exists()
    assert (output_path / 'Contents' / 'Windows').exists()
    assert (output_path / 'Contents' / 'Windows' / 'input.dll').exists()
    assert (output_path / 'Contents' / 'Resources').exists()
    assert not (output_path / 'Contents' / 'Info.plist').exists()
    assert (output_path / 'Contents' / 'Resources' / 'english' / 'strings.txt').exists()
    assert (output_path / 'Contents' / 'Resources' / 'english' / 'icon.png').exists()
    assert (output_path / 'Contents' / 'Resources' / 'french' / 'strings.txt').exists()


# ==================================================================================================================================================================================
# ============================================================================ Tests for cleanup on error ==========================================================================
# ==================================================================================================================================================================================
def test_cleanup(make_plugin, tmp_path):
    macos_plugin_path = make_plugin("input", "MacOS", has_plist=True, resources=RESOURCES)
    linux_plugin_path = make_plugin("input", "Linux", resources=RESOURCES)
    windows_plugin_path = make_plugin("input", "Windows")

    plugins = validate_plugins_structure([macos_plugin_path, linux_plugin_path, windows_plugin_path])
    validate_compatibility(plugins)

    output = tmp_path / 'output-universal'
    (macos_plugin_path / 'Contents' / 'Resources' / 'english' / 'strings.txt').unlink()

    with pytest.raises(FileNotFoundError):
        with CleanupOnError(output):
            build_universal_plugin(output, plugins)

    assert not output.exists()
