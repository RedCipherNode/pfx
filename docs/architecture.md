# PFX Architecture

## Overview

PFX is organized as a modular application.

Each component has a single responsibility and communicates through stable interfaces.

The transformation engine is shared across all application frontends.

---

## Components

### Engine

The engine is the core component of PFX.

Responsibilities:

- password transformation
- profile generation
- algorithm execution

The engine contains no user interface.

---

### Command Line Interface

The CLI provides terminal access to the engine.

Responsibilities:

- command parsing
- result presentation
- clipboard integration

The CLI does not implement transformation logic.

---

### Desktop Application

The desktop application provides a graphical interface.

Responsibilities:

- user interaction
- password presentation
- clipboard integration

The desktop application delegates password generation to the engine.

---

### Web Application

The web application provides browser access to the engine.

Responsibilities:

- user interaction
- password presentation

The web application does not maintain user accounts, password history, or persistent user data.

---

## Shared Components

### Clipboard

Provides platform clipboard access.

Used by application frontends.

---

## Third-Party Libraries

PFX uses external libraries where appropriate.

Current dependencies include:

- Argon2
- FLTK

---

## Project Structure

```text
apps/
docs/
include/
src/
tests/
third_party/
```

---

## Design Principles

PFX follows these architectural principles.

- Engine-first design.
- Stateless transformation.
- Thin application layers.
- Shared core implementation.
- Platform-independent engine.
- Separation of responsibilities.