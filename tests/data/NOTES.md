# Test Dataset

This directory contains deterministic datasets used for regression testing.

## Philosophy

The dataset is designed to validate deterministic behavior, not to train or evaluate password strength.

Each test case must exist for a technical reason, such as:

- Common human input patterns
- International writing systems
- Edge case coverage
- Reproducing a confirmed bug

Avoid adding semantically different but technically equivalent inputs.

## Structure

```
deterministic/
    input.csv

v1.x/
    golden.csv
    regression/

v2.x/
    golden.csv
    regression/
```

## Dataset Groups

The deterministic dataset is organized into three groups.

| Group | Purpose |
| ------ | ------- |
| Universal | Common human input patterns |
| International | Different writing systems and scripts |
| Edge | Boundary and special input cases |

Approximate distribution:

- Universal: 65%
- International: 25%
- Edge: 10%

## Versioning

`input.csv` is append-only.

Existing test cases should never be modified or removed unless they are incorrect.

Regression outputs are historical records and must never overwrite previous results.

## Deterministic Contract

The deterministic contract guarantees that identical input always produces identical output within the same major algorithm version.

If the transformation output changes for any existing test case, even by a single character, the deterministic contract is considered broken.

Such changes require a new major algorithm version.

Example:

- v1.x.x → Output remains identical
- v2.0.0 → Output intentionally changes

## Regression

Regression files are stored as timestamped snapshots.

Example:

```
001_20260801_194512.csv
002_20260801_203041.csv
003_20260802_081330.csv
```

These files provide an audit history of previous regression runs.

## Future

Additional deterministic test cases may be added when:

- a new human input pattern should be covered,
- a new writing system is introduced,
- or a confirmed bug requires permanent regression coverage.