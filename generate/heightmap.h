inline float* generateHeightMap(int width, int height, int thickness, float scaler = 10)
{
  float* data = (float*)malloc(width*height*sizeof(float));
  PerlinNoise generator;
  for(int y=0; y<height; y++)
  {
    for(int x=0; x<width; x++)
    {
      data[x + width * y] = (float)generator.noise((double)x/scaler, (double)y/scaler, 0) * thickness;
    }
  }
  return data;
}