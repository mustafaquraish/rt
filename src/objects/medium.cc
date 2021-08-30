#include <objects/medium.h>
#include <core/rng.h>
#include <cassert>

#define VOLUME_STEP_SIZE 0.1

bool Medium::hit(Ray& r, HitRec& rec) {
  Ray transformed = rayTransform(r);

  HitRec r1, r2;
  float t1 = 0, t2 = 0;
  
  if (!m_obj->hit(transformed, r1)) { return false; }

  if (dot(r1.n, transformed.d) < 0) {
    t1 = r1.t;
    Ray temp_ray = Ray(transformed.at(t1), transformed.d, r.tMax);
    if (m_obj->hit(temp_ray, r2)) {
      t2 = t1 + r2.t;
    } else {
      t2 = r.tMax;
    }
  } else {
    t2 = r1.t;
  }

  for (float t = t1; t < t2; t += VOLUME_STEP_SIZE) {
    Vec3 pt = transformed.at(t);
    if (Random.rand01() < m_density->get(pt)) {
      rec.t = t;
      rec.p = r.at(t);
      rec.n = normalized(Random.randomVector());
      r.tMax = t;
      rec.uv = Vec2(0);
      if (this->bsdf) {
        rec.obj = this;
      } else {
        rec.obj = r1.obj;
      }
      return true;
    }
  }

  return false;
}


LoadedMedium::LoadedMedium(const char* filename, float scale)
  : scale(scale) {
  
  FILE* fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "Failed to open %s\n", filename);
    exit(1);
  }

  assert(fscanf(fp, "VOL\n %d %d %d", &sx, &sy, &sz) == 3);
  std::cout << "Loading Volume: dimensions: " << sx << " " << sy << " " << sz << std::endl;
  m_data = new float[sx*sy*sz];

  for (int i = 0; i < sx*sy*sz; i++) {
    assert(fscanf(fp, " %f", &m_data[i]) == 1);
  }

  fclose(fp);

  this->bounds = AABB(0, 1);
}

float LoadedMedium::density(int x, int y, int z) {
  x = min(max(x, 0), sx-1);
  y = min(max(y, 0), sy-1);
  z = min(max(z, 0), sz-1);

  float val = m_data[x + y*sx + z*sx*sy];

  size_t idx = x + y*sx + z*sx*sy;
  size_t tot = sx*sy*sz;
  if (idx > tot-1) {
    printf("%d %d %d  .... %d <> %d\n", x, y, z, idx, tot);
  }
  return val;
}

float LoadedMedium::density(const Vec3& pt) {
  Vec3 p = pt * Vec3(sx, sy, sz);
  int x = (int)p.x;
  int y = (int)p.y;
  int z = (int)p.z;

  float val = trilerp(
    p.x - x, p.y - y, p.z - z,
    density(x  , y  , z  ),
    density(x+1, y  , z  ),
    density(x  , y+1, z  ),
    density(x+1, y+1, z  ),
    density(x  , y  , z+1),
    density(x+1, y  , z+1),
    density(x  , y+1, z+1),
    density(x+1, y+1, z+1)
  );
  return val * scale;
}

static AABB localBouds = AABB(0, 1);

bool LoadedMedium::hit(Ray& r, HitRec& rec) {
  Ray transformed = rayTransform(r);

  HitRec r1, r2;
  float t1 = 0, t2 = 0;
  
  if (!localBouds.hit(transformed, t1, t2)) { return false; };

  for (float t = t1; t < t2; t += VOLUME_STEP_SIZE) {
    Vec3 pt = transformed.at(t);
    if (Random.rand01() < density(pt)) {
      rec.t = t;
      rec.p = r.at(t);
      rec.n = normalized(Random.randomVector());
      r.tMax = t;
      rec.uv = Vec2(0);
      rec.obj = this;
      return true;
    }
  }

  return false;
}

#undef VOLUME_STEP_SIZE
