inline float* generateHeightMap(int width, int height, int thickness)
{
  float* data = (float*)malloc(width*height*sizeof(float));
  PerlinNoise generator;
  for(int y=0; y<height; y++)
  {
    for(int x=0; x<width; x++)
    {
      data[x + width * y] = (float)generator.noise((double)x/10, (double)y/10, 0) * thickness;
    }
  }
  return data;
}