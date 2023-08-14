#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <png.h>
#include <math.h>
#include <string.h>
#include "orig.c"
#include "typedefs.c"

int threads = 1;
int blocks = 1;
int PALETTE_SIZE = 16;

__device__ int* cisSortedCorrectly(color* a, int channel, int length) {
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


__device__ void mergeChunk(int channel, color* firstChunk, int firstLength, color* secondChunk, int secondLength) {
    color* tempArrayLeft = (color*)malloc(sizeof(*tempArrayLeft) * (firstLength));
    for (int i = 0; i < firstLength; i++) {
        tempArrayLeft[i] = firstChunk[i];
    }
    color* tempArrayRight = (color*)malloc(sizeof(*tempArrayRight) * (secondLength));
    for (int i = 0; i < secondLength; i++) {
        tempArrayRight[i] = secondChunk[i];
    }
    int i = 0, x = 0, y = 0;
    while (i < firstLength + secondLength) {
        if (x == firstLength) {
            firstChunk[i] = tempArrayRight[y];
            y++;
        }
        else if (y == secondLength) {
            firstChunk[i] = tempArrayLeft[x];
            x++;
        }
        else {
            if (tempArrayRight[y].values[channel] < tempArrayLeft[x].values[channel]) {
                firstChunk[i] = tempArrayRight[y];
                y++;
            }
            else {
                firstChunk[i] = tempArrayLeft[x];
                x++;
            }
        }
        i++;
    }
    free(tempArrayLeft);
    free(tempArrayRight);
}

__device__ void mergeSortChunk(int channel, color* bucket, int length) {
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
        mergeSortChunk(channel, bucket, mid);
        mergeSortChunk(channel, bucket + mid, length - mid);
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

__device__ int getChunkLength(int firstChunk, int lastChunk, int threadAmount, int bucketLength) {
    int base = bucketLength / threadAmount;
    int remainder = bucketLength % threadAmount;
    if (lastChunk <= remainder) {
        return (lastChunk - firstChunk) * (base + 1);
    }
    if (firstChunk < remainder) {
        return (lastChunk - remainder) * base + (remainder - firstChunk) * (base + 1);
    }
    return (lastChunk - firstChunk) * base;
}

__global__ void sortBucket(int channel, color* bucket, int length) {
    // Control variable starts at the thread's id and is halved after each iteration.
    int threadAmount = gridDim.x * blockDim.x;
    int iterationControl = blockDim.x * blockIdx.x + threadIdx.x; 
    int threadIndex = iterationControl;
    if (threadIndex >= length) {
        return;
    }
    int range = 1;
    int chunkStart = getChunkLength(0, threadIndex, threadAmount, length);
    int chunkLength = getChunkLength(threadIndex, threadIndex + 1, threadAmount, length);
    mergeSortChunk(channel, bucket + chunkStart, chunkLength);
    __syncthreads();
    while (range < threadAmount) {
        if (iterationControl % 2 == 0) {
            int firstChunkLength = getChunkLength(threadIndex, threadIndex + range, threadAmount, length);
            int secondChunkLength = getChunkLength(threadIndex + range, threadIndex + 2 * range, threadAmount, length);
            mergeChunk(channel, bucket + chunkStart, firstChunkLength, bucket + chunkStart + firstChunkLength, secondChunkLength);
            iterationControl /= 2;
        }
        range *= 2;
        __syncthreads();
    }
}

color getColorFromBucket(color* bucket, int bucketLength) {
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

color *OMPPaletteGeneration(png_bytep* image_pointers, int height, int width) {
    color* newPalette = (color*)malloc(sizeof(*newPalette) * PALETTE_SIZE);
    // All the buckets used to determine the palette. Number of buckets = number of colors in palette.
    color** pixelBuckets = (color**)malloc(sizeof(*pixelBuckets) * PALETTE_SIZE);
    // The first bucket is (for now) going to be width * height pixels long.
    pixelBuckets[0] = (color*)malloc(sizeof(*pixelBuckets[0]) * width * height);
    // Put every pixel in the first bucket.
    int k = 0;
    #pragma omp parallel for firstprivate(k)
    for (int i = 0; i < height; i++) {
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
        #pragma omp parallel for
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
            #pragma omp critical
            {
            if (max_red - min_red > mr_red.value) {
                mr_red.value = max_red - min_red;
                mr_red.bucket_index = m;
            }
            }
            #pragma omp critical
            {
            if (max_blue - min_blue > mr_blue.value) {
                mr_blue.value = max_blue - min_blue;
                mr_blue.bucket_index = m;
            }
            }
            #pragma omp critical
            {
            if (max_green - min_green > mr_green.value) {
                mr_green.value = max_green - min_green;
                mr_green.bucket_index = m;
            }
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
        color* controlBucket = (color*)malloc(sizeof(color) * bucketLengths[chosenBucket]);
        memcpy(controlBucket, pixelBuckets[chosenBucket], bucketLengths[chosenBucket] * sizeof(color));
        sequentialSortBucket(chosenChannel, controlBucket, bucketLengths[chosenBucket]);
        
        dim3 numBlocks(blocks);
        dim3 threadsPerBlock(threads);
        color* gpuChosenBucket;
        cudaMalloc(&gpuChosenBucket, sizeof(*gpuChosenBucket) * bucketLengths[chosenBucket]);
        cudaMemcpy(gpuChosenBucket, pixelBuckets[chosenBucket], sizeof(*gpuChosenBucket) * bucketLengths[chosenBucket], cudaMemcpyHostToDevice);
        //sortBucket<<<numBlocks, threadsPerBlock>>>(chosenChannel, gpuChosenBucket, bucketLengths[chosenBucket]);
        cudaMemcpy(pixelBuckets[chosenBucket], gpuChosenBucket, sizeof(*gpuChosenBucket) * bucketLengths[chosenBucket], cudaMemcpyDeviceToHost);
        cudaFree(gpuChosenBucket);
        sequentialSortBucket(chosenChannel, pixelBuckets[chosenBucket], bucketLengths[chosenBucket]);
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
}

void applyPalette(png_bytep* row_pointers, color* palette, int height, int width) {
    for (int i = 0; i < height; i++) {
        png_bytep row = row_pointers[i];
        for (int j = 0; j < width; j++) {
            png_bytep px = row + 4 * j;
            // The most distant two colors can be from each other is ~443 units. Distance between colors is calculated with the following formula:
            // sqrt(red_dif^2 + green_dif^2 + blue_dif^2)
            // best_color_sim's distance is initialized to 444 (an impossible value), so as to make sure the struct is updated with a valid color from the palette on the first iteration
            color_similarity best_color_sim;
            best_color_sim.distance = 444;
            best_color_sim.color_index = -1;
            for (int k = 0; k < PALETTE_SIZE; k++) {
                color currentColor = palette[k];
                double currentDistance;
                currentDistance = sqrt(pow(currentColor.values[RED] - px[RED], 2) + pow(currentColor.values[GREEN] - px[GREEN], 2) + pow(currentColor.values[BLUE] - px[BLUE], 2));
                if (currentDistance < best_color_sim.distance) {
                    best_color_sim.color_index = k;
                    best_color_sim.distance = currentDistance;
                }
            }
            px[RED] = palette[best_color_sim.color_index].values[RED];
            px[GREEN] = palette[best_color_sim.color_index].values[GREEN];
            px[BLUE] = palette[best_color_sim.color_index].values[BLUE];
        }
    }
}

void writeFile(int width, int height, png_bytep* row_pointers, FILE* newImage) {
        png_structp png_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_write) abort();

        png_infop info_write = png_create_info_struct(png_write);
        if (!info_write) abort();

        if (setjmp(png_jmpbuf(png_write))) abort();

        png_init_io(png_write, newImage);

        png_set_IHDR(
            png_write,
            info_write,
            width, height,
            8,
            PNG_COLOR_TYPE_RGBA,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT
        );
        png_write_info(png_write, info_write);

        if (!row_pointers) abort();

        png_write_image(png_write, row_pointers);
        png_write_end(png_write, NULL);
        png_destroy_write_struct(&png_write, &info_write);
}

void preprocessing(FILE* image, int* width_ptr, int* height_ptr,
                   png_structp png, png_bytep color_type,
                   png_bytep bit_depth, png_infop info) {
    png_init_io(png, image);

    png_read_info(png, info);

    *width_ptr = png_get_image_width(png, info);
    *height_ptr = png_get_image_height(png, info);
    *color_type = png_get_color_type(png, info);
    *bit_depth = png_get_bit_depth(png, info);
    if (*bit_depth == 16) {
        png_set_strip_16(png);
    }

    if (*color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png);
    }

    if (*color_type == PNG_COLOR_TYPE_GRAY && *bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png);
    }

    if (png_get_valid(png, info, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png);
    }

    if (*color_type == PNG_COLOR_TYPE_RGB ||
        *color_type == PNG_COLOR_TYPE_GRAY ||
        *color_type == PNG_COLOR_TYPE_PALETTE) {
            png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
        }

    if (*color_type == PNG_COLOR_TYPE_GRAY || *color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png);
    }

    png_read_update_info(png, info);
    
}

void OMPProcessingLoop(char** files, int dirlen) {
    #pragma omp parallel for
    for (int i = 0; i < dirlen; i++) {
        png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png) {
            abort();
        }

        png_infop info = png_create_info_struct(png);
        if (!info) {
            abort();
        }

        if (setjmp(png_jmpbuf(png))) {
            abort();
        }
        FILE *image = fopen(files[i], "r");
        int height;
        int width;
        png_byte color_type;
        png_byte bit_depth;
        preprocessing(image, &width, &height, png, &color_type, &bit_depth, info);

        png_bytep *row_pointers = NULL;
        if (row_pointers) abort();
        row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
        for (int i = 0; i < height; i++) {
            row_pointers[i] = (png_bytep)malloc(png_get_rowbytes(png, info));
        }

        png_read_image(png, row_pointers);

        fclose(image);
        png_destroy_read_struct(&png, &info, NULL);

        color* palette = OMPPaletteGeneration(row_pointers, height, width);
        applyPalette(row_pointers, palette, height, width);

        // Write to a new image.
        char* newFileName = (char*)malloc(sizeof(char) * (strlen(files[i])));
        strcpy(newFileName, files[i]);
        strncpy(newFileName + 2, "edited", 6);
        FILE *newImage = fopen(newFileName, "wb");
        if (!newImage) abort();

        writeFile(width, height, row_pointers, newImage);

        for (int i = 0; i < height; i++) {
            free(row_pointers[i]);
        }

        free(row_pointers);
        fclose(newImage);
        free(palette);

    }
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        threads = atoi(argv[1]);
        if (argc > 2) {
            blocks = atoi(argv[2]);
            if (argc > 3) {
                PALETTE_SIZE = atoi(argv[3]);
            }
        }
    }
    char** files = (char**)malloc(0);
    DIR *dr = opendir("./images/");
    struct dirent *en;
    int dirlen = 0;
    size_t size;
    cudaDeviceSetLimit(cudaLimitStackSize, 8192);
    cudaDeviceGetLimit(&size, cudaLimitStackSize);
    cudaDeviceSetLimit(cudaLimitMallocHeapSize, 1280000000);
    printf("%d", size);

    if (dr) {
        while ((en = readdir(dr)) != NULL) {
            if (en->d_name[0] != '.') {
                files = (char**)realloc(files, (dirlen + 1) * sizeof(char*));
                files[dirlen] = (char*)malloc((strlen(en->d_name) + PATH_LEN) * sizeof(char));
                strcpy(files[dirlen], "./images/");
                strcpy(files[dirlen] + PATH_LEN, en->d_name);
                dirlen += 1;
            }
        }
        closedir(dr);
    }

    OMPProcessingLoop(files, dirlen);
}

