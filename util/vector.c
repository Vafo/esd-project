#include "vector.h"
#include <math.h>

float vector_distance(vector_t* vec1, vector_t* vec2) {
  // print_log("vec1 (%f, %f) \tvec2 (%f, %f)\r\n", 
  //   vec1->x, vec1->y,
  //   vec2->x, vec2->y
  // );
  float x = vec1->x - vec2->x;
  float y = vec1->y - vec2->y;
  return sqrtf(x*x + y*y);
}

void vector_norm(vector_t* src) {
  float rev_norm = src->x * src->x + src->y * src->y;
  rev_norm = sqrtf(rev_norm);
  
  if(rev_norm == 0)
    return;
  
  src->x = src->x / rev_norm;
  src->y = src->y / rev_norm;
}

void vector_scale(vector_t* vec, float scale) {
  vec->x *= scale;
  vec->y *= scale;
}

float vector_dot_prod(vector_t* vec1, vector_t* vec2) {
  return vec1->x * vec2->x + vec1->y * vec2->y;
}

void vector_add(vector_t* pos, const vector_t* dir, float scale) {
  pos->x += dir->x * scale;
  pos->y += dir->y * scale;
}