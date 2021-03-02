#include "core/integrator.h"
#include "util/timer.h"
#include "util/progress.h"
#include "core/tile_manager.h"

void Integrator::render(Scene *scene) {
  int total_samples = min(maxSamples, params.get<int>("samples"));

  // Create Image
  int sx = params.get<int>("width");
  int sy = params.get<int>("height");
  Image im = Image(sx, sy);

  // Set up tile manager, timer and progress bar
  TileManager tileManager = TileManager(params);
  ProgressBar progress = ProgressBar(tileManager.numTiles, "Render").start();
  Timer timer = Timer("Render").start();

  #pragma omp parallel for schedule(dynamic, 1)
  for (int t = 0; t < tileManager.numTiles; t++) {

    Tile tile = tileManager.getTile(t);
    RNG rng = RNG(t);
    
    for (int x = tile.xBegin; x < tile.xEnd; x++) {
      for (int y = tile.yBegin; y < tile.yEnd; y++) {
        Colour col = 0;
        for (int sample = 0; sample < total_samples; sample++) {
          Ray ray = (enableRNG) ? scene->cam.getRay(x, y, rng) 
                                : scene->cam.getRay(x, y);
          col += Li(ray, scene, rng) / total_samples;
        }
        im.set(x, y, col);
      }
    }
    progress.update();
  }

  timer.stopAndDisplay();

  char *output = params.get<char *>("output");
  im.save(output, gammaCorrect);
  return;
}
