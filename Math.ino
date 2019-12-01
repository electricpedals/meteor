float rollingAverage(float *store, int size, float entry) {
  float total = 0;
  float result;
  for(int i=0; i<size-1; i++) {
    store[i] = store[i+1];
    total += store[i];
  }
  store[size-1] = entry;
  total += entry;
  result = total / size;
  return result;
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
