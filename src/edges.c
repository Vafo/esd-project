#include "edges.h"

#include "config.h"

#include "_glcd.h"

#include "etc.h"


edge_t m_edges[] = {
  /*left*/
  {
    .start = {
      .x = 0,
      .y = 0
    },
    .end  = {
      .x = 0,
      .y = LCD_HEIGHT
    }
  },
  /*top*/
  {
    .start = {
      .x = 0,
      .y = 0
    },
    .end = {
      .x = LCD_WIDTH,
      .y = 0
    },
  },
  /*right*/
  {
    .start = {
      .x = LCD_WIDTH-1,
      .y = 0
    },
    .end = {
      .x = LCD_WIDTH-1,
      .y = LCD_HEIGHT
    }
  },
  /*bottom*/
  {
    .start = {
      .x = 0,
      .y = LCD_HEIGHT-1
    },
    .end = {
      .x = LCD_WIDTH,
      .y = LCD_HEIGHT-1
    },
  }
};


void edges_init(edge_t* edges, size_t size) {
    size_t i;
    vector_t vec_tmp;
    float tmp;
    for(i = 0; i < size; ++i) {
        // Lateral
        vec_tmp.x = edges[i].end.x - edges[i].start.x;
        vec_tmp.y = edges[i].end.y - edges[i].start.y;
        vector_norm(&vec_tmp);
        edges[i].lat.x = vec_tmp.x;
        edges[i].lat.y = vec_tmp.y;

        // Normal
        tmp = vec_tmp.x;
        vec_tmp.x = vec_tmp.y;
        vec_tmp.y = -tmp;
        vector_norm(&vec_tmp);
        edges[i].norm.x = vec_tmp.x;
        edges[i].norm.y = vec_tmp.y;
    }
}

uint8_t dot_hits_line(vector_t* dot, float radius, pos_t* line_beg, pos_t* line_end) {
  vector_t dir = {
    .x = line_end->x - line_beg->x,
    .y = line_end->y - line_beg->y
  };
  vector_norm(&dir);

  vector_t line_point = {
    .x = line_beg->x,
    .y = line_beg->y
  };

  vector_t line_end_fl = {
    .x = line_end->x,
    .y = line_end->y
  };
  
  float distance = vector_distance(&line_point, &line_end_fl);
  int num_samples = distance / EDGE_SAMPLE_LENGTH;
  if(num_samples != 0)
    vector_scale(&dir, (distance / num_samples));
  // print_log("SAMPLES  %d\r\n", num_samples);
  do
  {
    // print_log("dot X: %f Y: %f\t", 
    //   dot_fl.x, dot_fl.y
    // );
    // print_log("line_point X: %f Y: %f\tDIST: \r\n", 
    //   line_point.x, line_point.y,
    //   vector_distance(&dot_fl, &line_point)
    // );
    
    if( vector_distance(dot, &line_point) <= radius ) {
      return 1;
    }
    vector_add(&line_point, &dir, 1);
  } while( --num_samples > 0 );
  
  return 0;
}

int8_t edges_is_hit(vector_t* pos, float radius, edge_t* edges, size_t size) {
  size_t i;
  for(i = 0; i < size; ++i) 
    if(dot_hits_line(pos, radius, &edges[i].start, &edges[i].end))
      return i;
  
  return -1;
}

uint8_t edges_check_hits(vector_t* pos, vector_t* dir, float radius, edge_t* edges, size_t size) {
  int8_t i;
  vector_t vec_res = {
    .x = 0,
    .y = 0
  };
  vector_t vec_comp;
  float scale;

  if( (i = edges_is_hit(pos, radius, edges, size)) != -1 ) {
      // print_log("HITS %d\r\n", i);

      // Norm vector
      vec_comp.x = edges[i].norm.x;
      vec_comp.y = edges[i].norm.y;
      scale = vector_dot_prod(dir, &vec_comp);
      vector_add(&vec_res, &vec_comp, -scale);
          
      // Lateral vector
      vec_comp.x = edges[i].lat.x;
      vec_comp.y = edges[i].lat.y;
      scale = vector_dot_prod(dir, &vec_comp);
      vector_add(&vec_res, &vec_comp, scale);

      dir->x = vec_res.x;
      dir->y = vec_res.y;

      return 1;
  }

  return 0;
}

void edges_render(edge_t* edges, size_t size) {
  int i;
  for(i = 0; i < size; ++i)
    GLCD_Line(
      edges[i].start.y, edges[i].start.x,
      edges[i].end.y, edges[i].end.x
    );
}

void borders_check_hits(vector_t* pos, vector_t* dir) {
    edges_check_hits(pos, dir, BORDER_EDGE_RADIUS/*radius*/, m_edges, ARR_SIZE(m_edges));
}

void borders_render() {
    edges_render(m_edges, ARR_SIZE(m_edges));
}

void borders_init() {
    edges_init(m_edges, ARR_SIZE(m_edges));
}