#ifndef VECTOR_H
#define VECTOR_H


typedef struct {
  float x;
  float y;
} vector_t;

#define j_vec  {  \
  .x = 0,         \
  .y = 1          \
}

#define i_vec  {  \
  .x = 1,         \
  .y = 0          \
}


// Get Euclidian distance between two vectors |vec1 - vec2|^(1/2)
float vector_distance(vector_t* vec1, vector_t* vec2);
// Normalize vector
void vector_norm(vector_t* src);
// Scale vector by (scale)
void vector_scale(vector_t* vec, float scale);
// Dot product of vectors
float vector_dot_prod(vector_t* vec1, vector_t* vec2);
// Add to pos vector dir scaled by scale. pos += dir * scale
void vector_add(vector_t* pos, const vector_t* dir, float scale);

#endif