# PFX Algorithm

## Overview

PFX is a deterministic password transformation algorithm.

It transforms a memorable plaintext into a high-entropy password using a cryptographic Key Derivation Function (KDF) and a deterministic transformation pipeline.

The same plaintext always produces the same password within the same algorithm version.

---

## Design Goals

The algorithm is designed to:

- Produce deterministic output.
- Generate high-entropy passwords.
- Accept memorable plaintext as input.
- Enforce modern password policies.
- Preserve long-term compatibility.

---

## Input

The input is a UTF-8 plaintext.

The algorithm processes the input exactly as provided.

No transformation is performed.

This includes:

- case conversion
- whitespace trimming
- Unicode normalization
- character correction

Every byte is significant.

Examples:

```text
hello
Hello
hello_
hello🐔
```

Each input is treated as a different value.

---

## Key Derivation Function

PFX uses Argon2id as its cryptographic Key Derivation Function (KDF).

The KDF transforms the input plaintext into fixed-length high-entropy bytes.

PFX does not implement its own cryptographic primitive.

Argon2id provides the cryptographic foundation of the algorithm.

---

## Transformation Pipeline

The algorithm consists of the following stages.

```text
Plaintext
      │
      ▼
Argon2id
      │
      ▼
Entropy
      │
      ▼
Schedule
      │
      ▼
Character Mapping
      │
      ▼
Password Profile
      │
      ▼
Policy Validation
      │
      ▼
Policy Repair
      │
      ▼
Password
```

---

## Character Mapping

Entropy bytes are mapped deterministically into the PFX character set.

This stage produces reproducible password characters.

---

## Password Profiles

PFX provides three output profiles.

- Compatibility
- Standard
- Maximum

Each profile uses the same algorithm but generates a different output length.

---

## Password Policy

Every generated password must contain at least:

- one uppercase letter
- one lowercase letter
- one digit
- one symbol

---

## Deterministic Policy Repair

If a generated password does not satisfy the required policy, PFX performs deterministic policy repair.

The password is modified using deterministic values derived from the existing entropy.

The KDF is never executed again during this process.

---

## Determinism

PFX is deterministic.

Identical input always produces identical output.

Any change to the input, including a single character, produces a different password.

---

## Algorithm Versioning

Algorithm versions are defined by output compatibility.

The algorithm version changes only when the generated password changes.

Software updates do not change the algorithm version unless they modify deterministic output.

Examples of non-algorithm changes include:

- GUI
- CLI
- documentation
- performance optimization
- refactoring
- bug fixes that do not affect output

---

## Compatibility

Each algorithm version guarantees deterministic compatibility.

The same plaintext always generates the same password within the same algorithm version.

Previous algorithm versions remain available to reproduce previously generated passwords.