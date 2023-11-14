#ifndef EDGES_H
#define EDGES_H

#include "vector.h"
#include <stdint.h>
#include <stddef.h>

typedef struct {
  int x;
  int y;
} pos_t;

typedef struct {
  pos_t start;
  pos_t end;
  
  vector_t norm; // norm
  vector_t lat; // lateral
} edge_t;


uint8_t dot_hits_line(vector_t* dot, float radius, pos_t* line_beg, pos_t* line_end);

// Initialize edges
// Set normal and lateral vectors
void edges_init(edge_t* edges, size_t size);
// Check if pos hits one of edges
// If hits, bounces off
uint8_t edges_check_hits(vector_t* pos, vector_t* dir, float radius, edge_t* edges, size_t size);
int8_t edges_is_hit(vector_t* pos, float radius, edge_t* edges, size_t size);
void edges_render(edge_t* edges, size_t size);

void borders_init();
void borders_check_hits(vector_t* pos, vector_t* dir);
void borders_render();

#endif