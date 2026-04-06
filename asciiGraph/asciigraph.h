#ifndef INC_ASCIIGRAPH_H
#define INC_ASCIIGRAPH_H

#include <stdint.h>

#define GRAPH_VAL_TYPE int32_t

typedef struct
{
    uint32_t row_count;
    float percent_per_row;

    uint32_t value_count; 
    GRAPH_VAL_TYPE min_value; /// Corresponds to first column
    GRAPH_VAL_TYPE max_value;  /// Corresponds to last column
    
    char **graph;
} AsciiGraph_t;


#endif /* INC_ASCIIGRAPH_H */
