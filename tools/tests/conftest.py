from pathlib import Path
import pytest
import sys

sys.path.insert(0, str(Path(__file__).parent.parent))


@pytest.fixture
def make_plugin(tmp_path):
    def _make(name, os_name, has_plist=False, resources=None):
        plugin_name = f'{name}-{os_name}.plugin'
        contents_path = tmp_path / plugin_name / 'Contents'

        os_dir = contents_path / os_name
        os_dir.mkdir(parents=True, exist_ok=True)

        if os_name == 'Linux':
            binary_name = f'lib{name}.so'
        elif os_name == 'Windows':
            binary_name = f'{name}.dll'
        else:
            binary_name = name

        (os_dir / binary_name).write_bytes(b'fake binary')

        (contents_path / 'Resources').mkdir(parents=True, exist_ok=True)
        if resources:
            for k, v in resources.items():
                resource_path = contents_path / 'Resources' / k
                resource_path.parent.mkdir(parents=True, exist_ok=True)
                if isinstance(v, str):
                    resource_path.write_text(v)
                elif isinstance(v, bytes):
                    resource_path.write_bytes(v)

        if has_plist:
            (contents_path / 'Info.plist').write_text('fake plist')

        return tmp_path / plugin_name

    return _make
