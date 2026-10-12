#pragma once

enum class MaterialType {
    Default = 0,     // Default fallback material for debugging
    Unlit,           // Flat unlit shading (UI, icons, unshaded colors)
    PBR_Standard,    // Physically Based Rendering (Albedo, Roughness, Metallic, Normal)
    BlinnPhong,      // Classic Blinn-Phong shading (Diffuse, Specular, Shininess)
    Glass,           // Transparent/refractive material (requires alpha blending)
    Water,           // Animated water surface shader
    Terrain,         // Multi-layer terrain blending shader
    Skybox,          // Background cubemap/skybox shader
    Custom           // Custom user-defined shader
};