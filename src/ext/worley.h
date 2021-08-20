#pragma once

union Vec2;
union Vec3;

namespace Worley {

float noise(float x, float y);
float noise(const Vec2& p);

float noise(float x, float y, float z);
float noise(const Vec3& p);

float layered(float x, float y, int octaves=8, float persistence=0.5, float lacunarity=2);
float layered(const Vec2& p, int octaves=8, float persistence=0.5, float lacunarity=2);

float layered(float x, float y, float z, int octaves=8, float persistence=0.5, float lacunarity=2);
float layered(const Vec3& p, int octaves=8, float persistence=0.5, float lacunarity=2);

}