#include "Grid_Utils.h"
#include"Global_Headers.h"

void VN_initialise_grid (float ** grid, int rows, int cols);
float interpolate(float x0, float x1, float alpha);
float ** GenerateSmoothNoise(float ** original, int octave, int rows, int cols);
float **GenerateNewNoise(float ** baseNoise, int octaveCount, int rows, int cols);
int ** classify_noise (float ** matriz, int rows, int cols);
void VN_cookies_shape (float ** grid, int rows, int cols, int radius);
int ** VN_build_map (int rows, int cols);