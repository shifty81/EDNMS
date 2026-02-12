# Contributing to EDNMS

Thank you for your interest in contributing to EDNMS! This document explains how to get involved.

## Getting Started

1. **Fork** the repository on GitHub
2. **Clone** your fork locally:
   ```bash
   git clone https://github.com/<your-username>/EDNMS.git
   cd EDNMS
   ```
3. **Build** and verify tests pass:
   ```bash
   cmake -B build
   cmake --build build
   ./build/EDNMSTests
   ```
4. **Create a branch** for your work:
   ```bash
   git checkout -b feature/your-feature-name
   ```

## Making Changes

### Code Style

- **Language**: C++17
- **Naming**: `PascalCase` for types and methods, `camelCase` for local variables, `m_` prefix for member variables, `UPPER_SNAKE_CASE` for constants
- **Headers**: Use `#pragma once` for include guards
- **Namespaces**: All engine code lives in the `ednms` namespace
- **Components**: Must be plain-old-data (POD) structs — no pointers to other components, use `EntityID` instead
- **Comments**: Keep them minimal; prefer self-documenting code. Add comments for non-obvious design decisions

### Architecture Rules

- **Simulation does NOT depend on Engine** — the simulation layer is engine-agnostic
- **Game does NOT depend on Renderer** — rendering is swappable
- **No circular dependencies** between systems
- **If it can't be saved/loaded, it doesn't exist** — all state must be serializable

### File Organization

- Engine core code goes in `Engine/`
- Simulation systems go in `Simulation/`
- Game-specific logic goes in `Game/`
- Tests go in `Tests/`

## Testing

- All new code should include tests in `Tests/`
- Use the existing test framework in `Tests/test_framework.h`
- Test naming convention: `TEST(Suite, TestName)`
- Run tests locally before submitting:
  ```bash
  ./build/EDNMSTests
  ```
- All existing tests must continue to pass

### Writing a Test

```cpp
#include "test_framework.h"
#include "Engine/ECS/ecs_registry.h"

TEST(YourSuite, YourTestName) {
    // Setup
    ednms::ECSRegistry registry;

    // Act
    ednms::EntityID e = registry.CreateEntity();

    // Assert
    EXPECT_NE(e, ednms::INVALID_ENTITY);
    return true;
}
```

## Submitting Changes

1. **Commit** your changes with a clear message:
   ```bash
   git add .
   git commit -m "Add brief description of what changed"
   ```
2. **Push** to your fork:
   ```bash
   git push origin feature/your-feature-name
   ```
3. **Open a Pull Request** against `main` on GitHub
4. **Describe** what your PR does and why

### PR Guidelines

- Keep PRs focused — one feature or fix per PR
- Include tests for new functionality
- Make sure all existing tests pass
- Update documentation if your change affects public APIs or behavior
- Reference any related issues in the PR description

## Reporting Bugs

Open a [GitHub Issue](https://github.com/shifty81/EDNMS/issues) with:
- A clear title and description
- Steps to reproduce the bug
- Expected vs actual behavior
- Your OS, compiler, and CMake version

## Feature Requests

Open a [GitHub Issue](https://github.com/shifty81/EDNMS/issues) with:
- A description of the feature
- Why it would be useful
- How it fits with the existing design (reference the design docs)

## Code of Conduct

Please follow our [Code of Conduct](CODE_OF_CONDUCT.md) in all interactions.

## Questions?

- Open a [GitHub Issue](https://github.com/shifty81/EDNMS/issues) with a `[Question]` tag
- Check existing [design documentation](README.md#-documentation) for architecture questions
