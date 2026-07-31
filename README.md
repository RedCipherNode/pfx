# PFX

PFX is a deterministic password transformation engine written in C++.

It transforms a memorable plaintext into a deterministic, high-entropy password using Argon2id and a deterministic formatting pipeline.

## Features

- Deterministic password transformation
- Argon2id-based key derivation
- Multiple output profiles
  - Compatibility
  - Standard
  - Maximum
- Password policy validation
- Deterministic policy repair
- Modern CMake project

## Status

> Early development.

The core transformation engine is implemented and under active development.

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

## Roadmap

- [x] Core transformation engine
- [ ] Documentation
- [ ] Clipboard support
- [ ] Batch mode
- [ ] Web interface

## License

MIT License