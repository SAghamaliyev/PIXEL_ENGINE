# Contributing to Pixel Engine

Thank you for your interest in contributing to **Pixel Engine**! To maintain code quality, consistency, and stability across platforms, please adhere to the following guidelines and conventions.

---

## Table of Contents

- [Cross-Platform Compatibility](#-cross-platform-compatibility)
- [C++ Standard & Core Guidelines](#-c-standard--core-guidelines)
  - [C++ Standard](#c-standard)
  - [Dependencies](#dependencies)
  - [Error Handling & Logging](#error-handling--logging)
  - [Performance Considerations](#performance-considerations)
- [Naming Conventions](#-naming-conventions)
  - [Quick Reference](#quick-reference)
  - [Classes and Structs](#classes-and-structs)
  - [Private Class Members](#private-class-members)
  - [Functions and Methods](#functions-and-methods)
  - [Function Parameters](#function-parameters)
  - [Local Variables](#local-variables)
  - [Pointers](#pointers)
  - [Constants](#constants)
- [Formatting Rules](#-formatting-rules)
  - [Indentation](#indentation)
  - [Braces](#braces)
  - [Initialization](#initialization)
  - [Includes](#includes)
  - [Wrapping & Alignment](#wrapping--alignment)
- [C++ Best Practices](#-c-best-practices)
  - [Use Early Returns](#use-early-returns)
  - [Prefer References When Ownership is Not Involved](#prefer-references-when-ownership-is-not-involved)
  - [Avoid Unnecessary Copying](#avoid-unnecessary-copying)
  - [Namespaces](#namespaces)
  - [Comments](#comments)

---

## 🌍 Cross-Platform Compatibility

Pixel Engine is a cross-platform project. All contributions must preserve compatibility across supported operating systems (Windows, Linux, macOS).

- **Do not use platform-specific APIs** unless they are properly abstracted behind a cross-platform interface.
- **Do not rely on Visual Studio-specific** compilers, extensions, project files, or build features.
- **Code must compile** with all supported compilers (MSVC, GCC, Clang) and build configurations.
- **Use CMake** for build configuration. Do not introduce IDE-specific build dependencies.
- **Use portable C++ standard library features** instead of operating-system-specific alternatives whenever possible.
- **Isolate platform-specific code**: If platform-specific code is necessary, isolate it and provide appropriate implementations for supported platforms.

> [!IMPORTANT]
> Contributors should not assume that code working on Windows will automatically work on Linux or macOS. Always consider path separators, case-sensitive filesystems, and windowing environments (X11 / Wayland).

---

## ⚙️ C++ Standard & Core Guidelines

### C++ Standard

All contributions must follow the C++ standard currently specified by the project (**C++17**).

- Do not introduce features from newer C++ standards unless the project explicitly adopts them.
- Avoid compiler-specific extensions (e.g. MSVC `__declspec`, GCC `__attribute__` without proper portability macros).
- Prefer standard C++ features over compiler-specific alternatives.
- Code must compile without relying on non-standard language extensions.

### Dependencies

- **No unjustified dependencies**: Do not introduce new third-party dependencies without justification.
- **Prefer existing libraries**: Prefer existing project dependencies (`glm`, `GLFW`, `glad`, `imgui`) or standard library functionality when appropriate.
- **Cross-platform requirement**: New dependencies must be compatible with the project's supported platforms and build system.
- **Licensing**: Respect the licenses of all third-party libraries (prefer MIT, Apache 2.0, zlib, BSD).
- **Clean commits**: Do not commit unnecessary third-party binaries, precompiled archives, or generated dependency files.

### Error Handling & Logging

- Follow the existing error-handling conventions used in the project.
- **Do not silently ignore errors**.
- **Logging system**: Do not use `std::cout` or `std::cerr` for engine logging. Use the project's `Logger` system (`Logger::addLog(...)`).
- **Validation**: Validate external input and resource loading results where appropriate.
- **No exception-based control flow**: Avoid exceptions for ordinary control flow.
- **Contextual diagnostics**: Error handling and logs should provide enough context to quickly identify the source of a problem.

### Performance Considerations

- Avoid unnecessary memory allocations and expensive operations in frequently executed code paths (such as the main render loop or per-frame scene updates).
- Avoid unnecessary copying of large objects (meshes, texture buffers, strings).
- Prefer efficient, cache-friendly data access patterns where appropriate.
- **Balance readability and performance**: Do not sacrifice code readability for premature optimization.
- Performance-critical changes should have a clear justification and, where appropriate, profiling evidence.

---

## 🏷️ Naming Conventions

### Quick Reference

| Element | Style | Example | Notes |
| :--- | :--- | :--- | :--- |
| **Classes & Structs** | `PascalCase` | `CameraManager`, `VertexData` | Describes an object or system |
| **Private Class Members** | `m_` + `camelCase` | `m_cameraID`, `m_camera` | Prefix with `m_` |
| **Pointer Members** | `m_p` + `PascalCase` | `m_pCamera`, `m_pRenderSystem` | Combines `m_` and `p` |
| **Functions & Methods** | `camelCase` | `createCamera()`, `setPosition()` | Describes an action |
| **Function Parameters** | `camelCase` | `targetPosition`, `entityID` | Clear semantic name |
| **Pointer Parameters** | `p` + `PascalCase` | `pCamera`, `pLayout` | Prefix with `p` |
| **Local Variables** | `snake_case` | `vertex_count`, `current_speed` | Lowercase with underscores |
| **Local Pointers** | `p_` + `snake_case` | `p_camera`, `p_monitor` | Prefix with `p_` |
| **Constants & Macros** | `UPPER_SNAKE_CASE` | `MAX_CAMERAS`, `PI` | `constexpr` or `#define` |

---

### Classes and Structs

Use `PascalCase`. Names should normally describe an object, system, or piece of data:

```cpp
class CameraManager;
struct VertexData;
```

### Private Class Members

Use the `m_` prefix:

```cpp
unsigned int m_cameraID;
CameraUnit m_camera;
```

For pointer members, use both the member and pointer prefixes (`m_p`):

```cpp
Camera* m_pCamera;
```

### Functions and Methods

Use `camelCase`. Function names should describe an action:

```cpp
void createCamera();
void setPosition(glm::vec3 targetPosition);
void updateTransform();
```

### Function Parameters

Use `camelCase`:

```cpp
void setPosition(glm::vec3 targetPosition);
```

### Local Variables

Use `snake_case`:

```cpp
int vertex_count = 0;
float current_speed = 0.0f;
```

### Pointers

Add the `p` prefix to a pointer, following the naming style of its category:

```cpp
Camera* m_pCamera;                  // Member: m_p + PascalCase
void setCamera(Camera* pCamera);    // Parameter: p + PascalCase
Camera* p_camera = nullptr;         // Local variable: p_ + snake_case
```

> [!TIP]
> Do not use raw pointers when ownership can be expressed more clearly with references or an appropriate smart pointer (`std::unique_ptr`, `std::shared_ptr`).

### Constants

Use `UPPER_SNAKE_CASE`:

```cpp
constexpr int MAX_CAMERAS = 16;
constexpr float PI = 3.14159f;
```

---

## 📐 Formatting Rules

### Indentation

- Always use **4 spaces**.
- **Tabs are forbidden**.
- Configure your editor to insert spaces on Tab, and convert existing tabs before committing.

```cpp
void MeshManager::makeMesh(uint64_t meshID) {
    if (meshID == 0) {
        return;
    }
}
```

### Braces

Use **Stroustrup-style braces**: the opening brace stays on the same line as the statement; `else` and `else if` start on a new line.

```cpp
if (isValid) {
    processData();
}
else if (isRecoverable) {
    recoverData();
}
else {
    handleError();
}
```

> [!IMPORTANT]
> **Braces are mandatory** even for single-line statements:
> ```cpp
> // Bad
> if (isValid) processData();
> 
> // Good
> if (isValid) {
>     processData();
> }
> ```

### Initialization

Simple default values should be initialized where the member is declared:

```cpp
struct Color {
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
};
```

Avoid moving simple initialization into a constructor without a specific reason.

### Includes

#### Include everything the file uses
Every file must include all headers for everything it uses itself, even if they are already included indirectly through another header. Never rely on transitive includes:

```cpp
// Uses glm::vec3 and uint64_t, so it includes both,
// even though EntityDefinitions.h already brings them in.
#include <cstdint>

#include <glm/glm.hpp>

#include "Definitions/EntityDefinitions.h"
```

#### Headers vs Source files
- Put an `#include` in the `.h` file only if the header itself needs it (types in declarations, members held by value, base classes).
- If something is used only inside the `.cpp`, include it in the `.cpp`, not in the header. Use forward declarations in headers when feasible.

#### Include Order
Organize includes into four distinct groups, separated by a blank line:

1. The file's own header (in a `.cpp`)
2. C++ Standard Library headers
3. Third-party library headers (`glad`, `GLFW`, `glm`, `imgui`)
4. Project headers

```cpp
#include "MeshManager.h"

#include <cstdint>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Core/Logger/Logger.h"
```

- Always use `#pragma once` as the header guard in every header.
- Use canonical path syntax for libraries (e.g. `<glm/glm.hpp>`), never relative traversal paths (e.g. `<../../glm-1.0.3/glm/glm.hpp>`).

### Wrapping & Alignment

#### Line length & readability
If a line becomes hard to read or overly long, wrap it. Readability is paramount! Before wrapping, check whether the statement can be clarified with an intermediate variable:

```cpp
// Hard to read:
result = someObject.getManager().getMesh(entity.getComponent().meshID, entity.isActive());

// Better: clear and readable:
auto mesh_id = entity.getComponent().meshID;
result = someObject.getManager().getMesh(mesh_id, entity.isActive());
```

#### Function calls and declarations
When wrapping function calls or signatures, **align continuation lines with the first character after the opening parenthesis**:

```cpp
glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE,
                   value_ptr(active_camera.projection));

glTexImage2D(GL_TEXTURE_2D,
             0,
             internal_format,
             width,
             height,
             0,
             data_format,
             GL_UNSIGNED_BYTE,
             data);
```

#### Logical operators
When conditions with `&&` or `||` span multiple lines, place the operator at the **beginning of the next line** and align continuation lines:

```cpp
// Short condition: single line is fine
if (isActive && meshID > 0) {
    // ...
}

// Complex condition: wrap before every logical operator
if (vertices.empty()
    || indices.empty()
    || textures.size() / 2 != vertices.size() / 3) {
    return;
}
```

---

## 💡 C++ Best Practices

### Use Early Returns

Prefer guard clauses over deeply nested `if` statements. Do not write `else` after a branch that ends with `return`:

```cpp
// Avoid deeply nested checks:
if (camera != nullptr) {
    if (camera->isActive()) {
        updateCamera(camera);
    }
}

// Prefer early returns:
if (camera == nullptr) {
    return;
}

if (!camera->isActive()) {
    return;
}

updateCamera(camera);
```

### Prefer References When Ownership is Not Involved

If a function requires an object but does not take ownership of it, use a reference where appropriate:

```cpp
void updateCamera(Camera& camera);
```

Use pointers when `nullptr` has a meaningful purpose or when pointer semantics are required.

### Avoid Unnecessary Copying

Pass non-trivial objects by `const` reference:

```cpp
void renderEntity(const Entity& entity); // Good: no copy overhead
void renderEntity(Entity entity);        // Avoid: accidental copy unless intentional
```

### Namespaces

- `using namespace` is **only permitted in `.cpp` files**.
- **Never use `using namespace` in header files**: it pollutes the global namespace for any translation unit that includes the header.

```cpp
// In a header (e.g. MeshManager.h):
std::vector<float> m_vertices;    // Always qualify with std::

// In a source file (e.g. MeshManager.cpp):
using namespace std;
vector<float> vertices;           // Permitted in .cpp
```

### Comments

Comments should explain **why**, not **what**:

```cpp
// Avoid: describes what the code visibly does
camera.setPosition(position);

// Good: explains domain rationale or non-obvious constraints
// GLFW reports the cursor position relative to the window,
// so the Y value has to be inverted before applying camera rotation.
```
