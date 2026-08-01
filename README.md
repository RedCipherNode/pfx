# PFX

PFX is a deterministic password transformation engine written in C++.

Instead of generating random passwords, PFX derives deterministic, high-entropy passwords from memorable plaintext using Argon2id and a deterministic formatting pipeline.

## Features

- Deterministic password transformation
- Argon2id-based key derivation
- Multiple output profiles
  - Compatibility
  - Standard
  - Maximum
- Password policy validation
- Deterministic policy repair
- Clipboard support
- Deterministic regression testing
- Modern CMake project

## Status

> Active development.

The core transformation engine is complete.

Current development focuses on tooling, documentation, and user experience.

## Build

```bash
cmake -B build
cmake --build build
```

## Usage

```bash
pfx <plaintext>
```

Example:

```bash
pfx bebek_ganteng
```

Output:

```text
Compatibility : ****************
Standard      : ************************
Maximum       : ********************************
```

Copy the generated password to the clipboard.

```bash
pfx bebek_ganteng --copy
```

## Regression Testing

PFX includes a deterministic regression suite to ensure identical input always produces identical output within the same major algorithm version.

For dataset structure and contribution guidelines, see:

```text
tests/data/NOTES.md
```

## Roadmap

- [x] Core transformation engine
- [x] Clipboard support
- [x] Regression testing
- [ ] CLI improvements
- [ ] Documentation
- [ ] Web interface

## License

MIT License