#include "typedefs.c"
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <png.h>
#include <math.h>
#include <string.h>

int* isSortedCorrectly(color* a, int channel, int length) {
    int isCorrect = true;
    int* correctness = (int*)malloc(sizeof(int) * length - 1);
    for (int i = 0; i < length - 1; i++) {
        if (a[i].values[channel] > a[i + 1].values[channel]) {
            correctness[i] = false;
            isCorrect = false;
            continue;
        }
        correctness[i] = true;
    }
    correctness[length - 1] = isCorrect;
    return correctness;
}

int* areArraysEqual(color* a, color* b, int length) {
    int* results = (int*)malloc(sizeof(int) * length);
    int consecutive = false;
    int consecutiveLength = 0;
    for (int i = 0; i < length; i++) {
        if (a[i].values[0] == b[i].values[0] && a[i].values[1] == b[i].values[1] && a[i].values[2] == b[i].values[2]) {
            results[i] = 0;
            consecutive = false;
            consecutiveLength = 0;
            continue;
        }
        if (!consecutive) {
            consecutive = true;
            consecutiveLength = 1;
            results[i] = 1;
        }
        else {
            results[i - consecutiveLength] += 1;
            results[i] = 0;
            consecutiveLength += 1;
        }
    }
    return results;
}

void sequentialSortBucket(int channel, color* bucket, int length) {
    if (length == 1) {
        return;
    }
    else if (length == 2) {
        color px1 = bucket[0], px2 = bucket[1];
        if (px1.values[channel] > px2.values[channel]) {
            bucket[1] = px1;
            bucket[0] = px2;
        }
        return;
    }
    else {
        int mid = length / 2;
        sequentialSortBucket(channel, bucket, mid);
        sequentialSortBucket(channel, bucket + mid, length - mid);
        color* tempArrayLeft = (color*)malloc(sizeof(*tempArrayLeft) * (mid));
        for (int i = 0; i < mid; i++) {
            tempArrayLeft[i] = bucket[i];
        }
        color* tempArrayRight = (color*)malloc(sizeof(*tempArrayRight) * (length - mid));
        for (int i = 0; i < length - mid; i++) {
            tempArrayRight[i] = bucket[i + mid];
        }
        int i = 0, x = 0, y = 0;
        while (i < length) {
            color temp_px;
            if (x == mid) {
                bucket[i] = tempArrayRight[y];
                y++;
            }
            else if (y == length - mid) {
                bucket[i] = tempArrayLeft[x];
                x++;
            }
            else {
                if (tempArrayRight[y].values[channel] < tempArrayLeft[x].values[channel]) {
                    bucket[i] = tempArrayRight[y];
                    y++;
                }
                else {
                    bucket[i] = tempArrayLeft[x];
                    x++;
                }
            }
            i++;
        }
        free(tempArrayLeft);
        free(tempArrayRight);
    }
}

/*color getColorFromBucket(color* bucket, int bucketLength) {
    int redSum = 0, greenSum = 0, blueSum = 0;
    for (int i = 0; i < bucketLength; i++) {
        redSum += bucket[i].values[RED];
        greenSum += bucket[i].values[GREEN];
        blueSum += bucket[i].values[BLUE];
    }
    color newColor;
    newColor.values[RED] = redSum / bucketLength;
    newColor.values[GREEN] = greenSum / bucketLength;
    newColor.values[BLUE] = blueSum / bucketLength;
    return newColor;
}

color *sequentialPaletteGeneration(png_bytep* image_pointers, int height, int width) {
    color* newPalette = (color*)malloc(sizeof(*newPalette) * PALETTE_SIZE);
    // All the buckets used to determine the palette. Number of buckets = number of colors in palette.
    color** pixelBuckets = (color**)malloc(sizeof(*pixelBuckets) * PALETTE_SIZE);
    // The first bucket is (for now) going to be width * height pixels long.
    pixelBuckets[0] = (color*)malloc(sizeof(*pixelBuckets[0]) * width * height);
    // Put every pixel in the first bucket.
    for (int i = 0, k = 0; i < height; i++) {
        png_bytep row = image_pointers[i];
        for (int j = 0; j < width; j++) {
            png_bytep px = &(row[j * 4]);
            pixelBuckets[0][k].values[RED] = px[RED];
            pixelBuckets[0][k].values[GREEN] = px[GREEN];
            pixelBuckets[0][k++].values[BLUE] = px[BLUE];
        }
    }
    // Store the lenghts of each bucket and modify them as they change
    int bucketLengths[PALETTE_SIZE];
    bucketLengths[0] = width * height;
    for (int n = 0; n < PALETTE_SIZE - 1; n++) {
        //Iterate through every created bucket and find the one with the highest range of values in any channel.
        max_range mr_red, mr_blue, mr_green;
        mr_red.value = 0;
        mr_blue.value = 0;
        mr_green.value = 0;
        mr_red.bucket_index = 0;
        mr_blue.bucket_index = 0;
        mr_green.bucket_index = 0;
        for (int m = 0; m <= n; m++) {
            // Define max and min values of each channel. They will always be initialized on the first pass.
            int max_red, min_red, max_green, min_green, max_blue, min_blue;
            int isFirst = true;
            for (int i = 0; i < bucketLengths[m]; i++) {
                color px = pixelBuckets[m][i];
                if (isFirst) {
                        min_red = px.values[RED];
                        max_red = px.values[RED];
                        min_green = px.values[GREEN];
                        max_green = px.values[GREEN];
                        min_blue = px.values[BLUE];
                        max_blue = px.values[BLUE];
                        isFirst = false;
                }
                else {
                    if (px.values[RED] < min_red) {
                        min_red = px.values[RED];
                    }
                    else if (px.values[RED] > max_red) {
                        max_red = px.values[RED];
                    }
                    if (px.values[GREEN] < min_green) {
                        min_green = px.values[GREEN];
                    }
                    else if (px.values[GREEN] > max_green) {
                        max_green = px.values[GREEN];
                    }
                    if (px.values[BLUE] < min_blue) {
                        min_blue = px.values[BLUE];
                    }
                    else if (px.values[BLUE] > max_blue) {
                        max_blue = px.values[BLUE];
                    }
                }
            }
            if (max_red - min_red > mr_red.value) {
                mr_red.value = max_red - min_red;
                mr_red.bucket_index = m;
            }
            if (max_blue - min_blue > mr_blue.value) {
                mr_blue.value = max_blue - min_blue;
                mr_blue.bucket_index = m;
            }
            if (max_green - min_green > mr_green.value) {
                mr_green.value = max_green - min_green;
                mr_green.bucket_index = m;
            }
        }
        int chosenBucket = 0, chosenChannel;
        if (mr_red.value >= mr_green.value) {
            if (mr_red.value >= mr_blue.value) {
                chosenBucket = mr_red.bucket_index;
                chosenChannel = RED;
            }
            else {
                chosenBucket = mr_blue.bucket_index;
                chosenChannel = BLUE;
            }
        }
        else {
            if (mr_green.value >= mr_blue.value) {
                chosenBucket = mr_green.bucket_index;
                chosenChannel = GREEN;
            }
            else {
                chosenBucket = mr_blue.bucket_index;
                chosenChannel = BLUE;
            }
        }
        sortBucket(chosenChannel, pixelBuckets[chosenBucket], bucketLengths[chosenBucket]);
        int mid = bucketLengths[chosenBucket] / 2;
        pixelBuckets[n + 1] = (color*)malloc(sizeof(*pixelBuckets[n + 1]) * bucketLengths[chosenBucket] - mid);
        bucketLengths[n + 1] = bucketLengths[chosenBucket] - mid;
        for (int i = 0; i < bucketLengths[chosenBucket] - mid; i++) {
            pixelBuckets[n + 1][i] = pixelBuckets[chosenBucket][mid + i];
        }
        pixelBuckets[chosenBucket] = (color*)realloc(pixelBuckets[chosenBucket], mid * sizeof(*pixelBuckets[chosenBucket]));
        bucketLengths[chosenBucket] = mid;
    }
    for (int n = 0; n < PALETTE_SIZE; n++) {
        newPalette[n] = getColorFromBucket(pixelBuckets[n], bucketLengths[n]);
        free(pixelBuckets[n]);
    }
    free(pixelBuckets);
    return newPalette;
}*/