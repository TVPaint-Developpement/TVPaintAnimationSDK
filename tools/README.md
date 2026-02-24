# tools

## tvp-merge-plugin.py

Python tool to verify and merge TVPaint `.plugin` bundles. It supports two subcommands:
- **`verify`**: validate a single plugin bundle structure
- **`merge`**: merge multiple per-platform bundles into a single universal multi-platform bundle

### Requirements

- Python 3.10 or higher (uses dataclasses and modern typing)
- No external dependencies (standard library only)

### Usage

```
python tvp-merge-plugin.py <command> [options]
```

#### Commands

| Command | Description |
|---|---|
| `verify` | Validate the structure of a single plugin bundle |
| `merge` | Merge multiple per-platform bundles into a universal bundle |

Run `python tvp-merge-plugin.py --help` or `python tvp-merge-plugin.py <command> --help` for details.

---

### `verify` — Validate a plugin bundle

```
python tvp-merge-plugin.py verify [--verbose] <plugin>
```

| Argument | Description |
|---|---|
| `plugin` | Path to the plugin bundle to validate (`.plugin` extension required) |
| `--verbose` | Show detailed messages about the validation steps |

#### Example

```bash
python tvp-merge-plugin.py verify PI_Flip-MacOS.plugin
```

Checks that the bundle has the correct directory structure, contains exactly one OS binary, and that resources are properly organized.

---

### `merge` — Merge plugin bundles

```
python tvp-merge-plugin.py merge [--verbose] [--dry-run] <output> <input1> <input2> [input3]
```

| Argument | Description |
|---|---|
| `output` | Output bundle path (must have `.plugin` extension) |
| `inputs` | 2 or 3 input bundles (one per platform, `.plugin` extension required) |
| `--dry-run` | Validate inputs and show what would be done without creating any output |
| `--verbose` | Show detailed messages about operations being performed (always enabled with `--dry-run`) |

#### Example

```bash
python tvp-merge-plugin.py merge PI_Flip-Universal.plugin \
    PI_Flip-MacOS.plugin \
    PI_Flip-Windows.plugin \
    PI_Flip-Linux.plugin
```

Creates `PI_Flip-Universal.plugin` containing binaries for all three platforms.

---

### Expected Input Bundle Structure

Each input bundle must follow this structure:

```
MyPlugin.plugin/
└── Contents/
    ├── MacOS/        # or Windows/ or Linux/
    │   └── <binary>
    ├── Resources/    # optional
    │   ├── french/
    │   ├── english/
    │   ├── chinese/
    │   └── japanese/
    └── Info.plist    # optional (macOS)
```

**Constraints:**
- Each bundle must contain exactly one platform directory (`MacOS`, `Windows`, or `Linux`)
- Each platform directory must contain exactly one binary file
- Resources may only contain language subdirectories among: `french`, `english`, `chinese`, `japanese`
- Each language directory may only contain `.txt` and `.png` files
- `.txt` files must be named exactly `strings.txt`

### Output Bundle Structure

The produced universal bundle aggregates binaries from all input platforms:

```
MyPlugin-Universal.plugin/
└── Contents/
    ├── MacOS/
    │   └── <macOS binary>
    ├── Windows/
    │   └── <Windows binary>
    ├── Linux/
    │   └── <Linux binary>
    ├── Resources/      # merged from all input bundles
    │   ├── french/
    │   └── english/
    └── Info.plist      # copied from the first bundle that contains one
```

### Validations Performed

Before creating any output, the tool checks:

- The output file does not already exist
- Each input bundle is a valid `.plugin` directory
- Each bundle targets exactly one platform (no duplicates)
- Binary names are identical across all bundles (e.g. all named `PI_Flip`)
- Resource files only use allowed types (`.txt` named `strings.txt`, or `.png`)
- Shared resources have identical SHA-256 checksums (consistency guaranteed)

If an error occurs during creation, the partially created output bundle is automatically removed.

### `--dry-run` Option

Validates the compatibility of input bundles without writing anything to disk:

```bash
python tvp-merge-plugin.py merge --dry-run PI_Flip-Universal.plugin \
    PI_Flip-MacOS.plugin \
    PI_Flip-Windows.plugin
```