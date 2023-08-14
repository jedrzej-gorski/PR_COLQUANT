#pragma once
typedef struct _color {
    int values[3];
} color;

typedef struct _max_range {
    int value;
    int bucket_index;
} max_range;

typedef struct _color_similarity {
    double distance;
    int color_index;
} color_similarity;

#define RED 0
#define GREEN 1
#define BLUE 2
#define true 1
#define false 0
#define PATH_LEN sizeof("./images/") - 1