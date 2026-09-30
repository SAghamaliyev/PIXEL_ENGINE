# Code Style

## Naming

### Classes and structs

Use `PascalCase`.

Names should normally describe an object, system, or piece of data.

```cpp
class CameraManager;
struct VertexData;
```

### Private members of Class

Use the `m_` prefix.

```cpp
unsigned int m_cameraID;
CameraUnit m_camera;
```

For pointer members, use both the member and pointer prefixes:

```cpp
Camera* m_pCamera;
```

### Functions and methods

Use `camelCase`.

Function names should describe an action.

```cpp
void createCamera();
void setPosition(...);
void updateTransform();
```

### Function parameters

Use `camelCase`.

```cpp
void setPosition(glm::vec3 targetPosition);
```

### Local variables

Use `snake_case`.

```cpp
int vertex_count = 0;
float current_speed = 0.0f;
```

### Raw pointers

Add the `p` prefix to a pointer, following the naming style of its category:

```cpp
Camera* m_pCamera;                  // member: m_ + camelCase
void setCamera(Camera* pCamera);    // parameter: camelCase
Camera* p_camera = nullptr;         // local variable: snake_case
```

Do not use raw pointers when ownership can be expressed more clearly with references
or an appropriate smart pointer.

### Constants

Use `UPPER_SNAKE_CASE`.

```cpp
constexpr int MAX_CAMERAS = 16;
constexpr float PI = 3.14159f;
```

---

# Formatting

## Indentation

Always use **4 spaces**. Tabs are **forbidden**.

Configure your editor to insert spaces when you press Tab, and convert existing tabs
to spaces before committing.

```cpp
void MeshManager::makeMesh(uint64_t meshID) {
    if (meshID == 0) {
        return;
    }
}
```

## Braces

Use Stroustrup-style braces: the opening brace stays on the same line as the statement,
`else` and `else if` start on a new line.

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

Braces are required even for a single statement:

```cpp
// Bad
if (isValid) processData();

// Good
if (isValid) {
    processData();
}
```

## Initialization

Simple default values should be initialized where the member is declared when possible.

```cpp
struct Color {
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
};
```

Avoid moving simple initialization into a constructor without a reason.

## Includes

### Include everything the file uses

Every file includes all headers for everything it uses itself, even if they are
already included indirectly through another header.

Never rely on an include made by another file. If that file changes its includes,
your file would stop compiling.

```cpp
// Uses glm::vec3 and uint64_t, so it includes both,
// even though EntityDefinitions.h already brings them in.
#include <cstdint>

#include <glm/glm.hpp>

#include "Definitions/EntityDefinitions.h"
```

### Headers and source files

Put an include in the `.h` file only if the header itself needs it
(types in declarations, members held by value, base classes).

If something is used only inside the `.cpp`, include it in the `.cpp`, not in the header.

### Order

1. The file's own header (in a `.cpp`)
2. Standard library
3. Third-party libraries (`glm`, `GLFW`, `glad`)
4. Project headers

Separate the groups with a blank line.

```cpp
#include "MeshManager.h"

#include <cstdint>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Logger.h"
```

Use `#pragma once` in every header.
Use `<glm/glm.hpp>` style paths for libraries, never relative paths like `<../../glm-1.0.3/glm/glm.hpp>`.

## Wrapping & alignment

### Line length

There is no fixed limit. If a line becomes hard to read or too long, wrap it.
Readability matters more than any number.

Before wrapping, check whether the line can be shortened with an intermediate variable
or a simpler expression.

```cpp
// Hard to read
result = someObject.getManager().getMesh(entity.getComponent().meshID, entity.isActive());

// Better: shorter and easier to read
auto mesh_id = entity.getComponent().meshID;
result = someObject.getManager().getMesh(mesh_id, entity.isActive());
```

### Function calls and declarations

When wrapping function calls, declarations, or conditions, **align continuation lines
with the first character after the opening bracket**.

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

Wrap after a comma between arguments. If arguments have different meaning and are hard to tell
apart (many literals, enums, zeros), put each argument on its own line, as in `glTexImage2D`.

### Logical operators

Almost always wrap conditions with `||`, `&&` and similar operators if they are not short.

Put the operator at the **beginning** of the next line and align the continuation
with the first character after the opening bracket of `if`.

```cpp
// Short condition: one line is fine
if (isActive && meshID > 0) {
    // ...
}

// Not short: wrap before every logical operator
if (vertices.empty()
    || indices.empty()
    || textures.size() / 2 != vertices.size() / 3) {
    return;
}
```

---

# C++ Practices

## Use early returns

Prefer guard clauses over deeply nested `if` statements.
Do not write `else` after a branch that ends with `return`.

Instead of:

```cpp
if (camera != nullptr) {
    if (camera->isActive()) {
        updateCamera(camera);
    }
}
```

Prefer:

```cpp
if (camera == nullptr) {
    return;
}

if (!camera->isActive()) {
    return;
}

updateCamera(camera);
```

## Prefer references when ownership is not involved

If a function requires an object but does not take ownership of it, use a reference where appropriate.

```cpp
void updateCamera(Camera& camera);
```

Use pointers when `nullptr` has a meaningful purpose or when the architecture requires pointer semantics.

## Avoid unnecessary copying

Pass larger objects by reference when they do not need to be copied.

```cpp
void renderEntity(const Entity& entity);
```

rather than:

```cpp
void renderEntity(Entity entity);
```

unless a copy is intentional.

## Namespaces

`using namespace` is allowed only in `.cpp` files.

Never use it in headers: it leaks into every file that includes the header
and can cause name conflicts that are hard to trace.

```cpp
// MeshManager.h
std::vector<float> m_vertices;    // always write std:: in headers

// MeshManager.cpp
using namespace std;
vector<float> vertices;           // allowed here
```

---

# Comments

Comments should explain **why**, not **what**.

Avoid:

```cpp
// Set camera position
camera.setPosition(position);
```

A useful comment explains something that cannot be understood directly from the code:

```cpp
// GLFW reports the cursor position relative to the window,
// so the Y value has to be inverted before applying camera rotation.
```
