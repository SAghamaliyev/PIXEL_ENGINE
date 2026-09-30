# Contributing to Pixel Engine

Thanks for taking an interest in Pixel Engine.

This file covers the basic workflow, code style, and architectural rules used in the project. Please follow these guidelines when opening a pull request so that new code fits the existing engine instead of creating a separate way of doing things.

## Workflow

Pixel Engine uses the usual fork and pull request workflow.

### 1. Fork the repository

Fork the repository to your own GitHub account.

### 2. Clone your fork

```bash
git clone https://github.com/YOUR_USERNAME/PIXEL_ENGINE.git
cd PIXEL_ENGINE
```

### 3. Create a branch

Create a separate branch for each change. Do not work directly on `main`.

Use one of these prefixes:

* `feature/` — new functionality
* `bugfix/` — bug fixes
* `refactor/` — code or architecture changes

Example:

```bash
git checkout -b feature/new-camera-system
```

### 4. Make your changes

Keep changes focused on the purpose of the branch.

Before adding a new system, manager, dependency, or abstraction, check whether the existing architecture already has a place for it.

### 5. Commit

Write a short commit message that describes the change.

```bash
git commit -m "Add frustum culling to CameraManager"
```

### 6. Push your branch

```bash
git push origin feature/new-camera-system
```

### 7. Open a Pull Request

Open a pull request against the `main` branch.

The description should include:

* What was changed
* Why the change was needed
* Any important implementation details
* Screenshots or a short video if the change affects the editor or rendering

For architectural changes, explain why the change fits the current architecture.

Keep pull requests focused. Avoid mixing unrelated refactors with a feature or bug fix.

---

# Architecture

Pixel Engine follows a system-based architecture.

The main goal is to keep systems independent and give each part of the engine a clear responsibility. New code should fit into this structure rather than introducing another way of communicating between systems.

## Engine

`Engine` owns the main application loop and coordinates the major parts of the application.

It should not contain the actual implementation of rendering, scene management, input handling, UI, or asset management.

Avoid turning `Engine.cpp` into a place where unrelated engine logic accumulates.

---

## Systems

Each system should have one clear responsibility.

Examples include:

* `RenderSystem` — rendering
* `SceneSystem` — entities and scene state
* `AssetSystem` — loading and managing assets
* `InputManager` — collecting and processing input
* `CameraManager` — camera state and camera operations
* UI/editor code — editor interface and editor-specific interaction
* `EventSystem` — communication through engine/editor events

If a new feature belongs to an existing system, extend that system instead of creating another manager just to hold a few functions.

Create a new system only when it represents a meaningful and independent responsibility.

---

## Systems should not depend on each other's internals

Avoid direct dependencies between systems when they are not necessary.

For example, `RenderSystem` should not start managing scene state itself and should not directly modify `SceneSystem`.

If rendering needs scene information, provide the required data through the appropriate interface or shared data structure.

The same principle applies in the other direction.

The goal is to avoid architecture like:

```text
RenderSystem
    ↓
SceneSystem
    ↓
AssetSystem
    ↓
CameraManager
    ↓
UI
```

where one system slowly becomes dependent on everything else.

Prefer clear boundaries:

```text
             Engine
                |
      +---------+---------+
      |         |         |
   Scene     Render      UI
   System    System     System
      |         |
   Entities   Render
   + Data      Data

       EventSystem
             |
     Editor / Engine events
```

The exact implementation can change as the engine grows, but the separation of responsibilities should remain.

---

## Do not move responsibilities to the wrong system

When adding functionality, first ask:

> Which system owns this data or operation?

For example:

* Entity state belongs to `SceneSystem`.
* Rendering operations belong to `RenderSystem`.
* Camera state and camera calculations belong to `CameraManager`.
* Input collection belongs to `InputManager`.
* Asset loading belongs to `AssetSystem`.
* Editor interaction belongs to the editor/UI layer.
* Cross-system events belong to `EventSystem`.

Do not put functionality somewhere simply because that file is convenient to access.

Convenience is not a good reason to create an architectural dependency.

---

## Prefer data transfer over system coupling

When one system needs information from another system, prefer passing the required data instead of giving it access to the entire system.

For example, if rendering only needs a list of renderable entities, pass or expose the appropriate render data rather than making `RenderSystem` depend on the complete `SceneSystem`.

This keeps dependencies smaller and makes systems easier to change independently.

---

## Events

Use the event system when an action needs to be communicated between otherwise independent parts of the engine.

For example:

```text
Input
   ↓
EditorEvent
   ↓
EventSystem
   ↓
SceneSystem
```

Events should represent an actual event or command.

Do not use the event system for every function call. If two pieces of code have a direct ownership relationship and a normal function call is appropriate, use the function call.

---

## Shared structures

Do not create one huge `Definitions.h` containing every structure in the engine.

A structure should generally live close to the system or concept that owns it.

For example:

```text
Camera.h
    Camera
    CameraSettings

Material.h
    Material
    MaterialType

Entity.h
    Entity
    EntityData
```

If a structure is genuinely shared by several independent systems, it can be placed in a common definitions/data header.

The goal is to avoid both extremes:

* everything in one giant definitions file
* the same structure being duplicated in several places

---

## Ownership and dependencies

Before adding an `#include`, consider why the dependency is required.

Prefer forward declarations where they are sufficient, especially in headers.

For example:

```cpp
class Shader;

class RenderSystem {
public:
    void setShader(Shader* pShader);
};
```

Include the full definition where it is actually required, usually in the `.cpp` file.

Do not introduce an include dependency simply because it is convenient.

---

## Avoid unnecessary abstractions

Pixel Engine is an engine project, but that does not mean every feature needs an interface, manager, wrapper, or inheritance hierarchy.

Do not add an abstraction just because it looks more "engine-like".

A new abstraction should solve an actual problem such as:

* separating responsibilities
* removing duplicated logic
* managing ownership
* reducing coupling
* providing a stable interface
* making future extensions significantly easier

If a simple function or structure is enough, keep it simple.

---

## Keep the architecture predictable

When implementing a feature, prefer the existing architectural patterns unless there is a strong reason to change them.

A contributor should not introduce a completely different design for one subsystem without discussing it first.

For larger architectural changes, open an issue or discuss the approach in the pull request before doing a large implementation.

---

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

For pointer members, use both the member and pointer prefixes:

```cpp
Camera* mp_camera;
```

### Raw pointers

Use the `p` prefix.

```cpp
Camera* pCamera;
Shader* pActiveShader;
```

Do not use raw pointers when ownership can be expressed more clearly with references or an appropriate smart pointer.

### Constants

Use `UPPER_SNAKE_CASE`.

```cpp
#define MAX_CAMERAS 16
#define PI 3.14159f
```
---

# Formatting

## Indentation

Use **4 spaces**.

Do not use tabs.

```cpp
void MeshManager::makeMesh(uint64_t meshID) {
    if (meshID > 0) {
        // ...
    }
}
```

## Braces

Use K&R-style braces:

```cpp
if (isValid) {
    processData();
}
```

The opening brace stays on the same line as the statement.

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

## Wrapping & alignment

### Line length

The limit is **110 characters** per line. If a line does not fit, wrap it.

Before wrapping, check whether the line can be shortened with an intermediate variable
or a simpler expression.

```cpp
// Bad: long chain that needs wrapping
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

instead of unnecessarily passing ownership-like semantics through a pointer.

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

---

# Comments

Comments should explain **why**, not **what**.

Avoid:

```cpp
// Set camera position
camera.setPosition(position);
```

The code already explains what it does.

A useful comment explains something that cannot be understood directly from the code:

```cpp
// GLFW reports the cursor position relative to the window,
// so the Y value has to be inverted before applying camera rotation.
```

If something needs a large comment to explain, consider whether the code or architecture can be made clearer instead.

---

# Pull Request Checklist

Before opening a PR, check the following:

* [ ] The project builds successfully.
* [ ] The changed functionality has been tested.
* [ ] No unrelated files or changes were included.
* [ ] Naming follows the project conventions.
* [ ] Formatting uses 4 spaces.
* [ ] New dependencies are actually necessary.
* [ ] The change does not create unnecessary system coupling.
* [ ] Existing architecture is followed.
* [ ] New abstractions have a clear reason to exist.
* [ ] Generated files and local configuration are not committed.
* [ ] Screenshots or video are included when the change affects the editor or rendering.
* [ ] The PR description explains architectural changes when applicable.

Thanks for contributing to Pixel Engine.
