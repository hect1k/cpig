#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define DEFAULT_PALETTE_SIZE 5  // Default number of colors in the palette

typedef struct {
    uint8_t r, g, b;
} Color;

// Compute the Euclidean distance between two colors
float color_distance(Color a, Color b) {
    return sqrtf((a.r - b.r) * (a.r - b.r) +
                 (a.g - b.g) * (a.g - b.g) +
                 (a.b - b.b) * (a.b - b.b));
}

// Assign the nearest cluster to a pixel
int assign_cluster(Color pixel, Color *centroids, int num_clusters) {
    int best_cluster = 0;
    float min_distance = color_distance(pixel, centroids[0]);

    for (int i = 1; i < num_clusters; i++) {
        float distance = color_distance(pixel, centroids[i]);
        if (distance < min_distance) {
            min_distance = distance;
            best_cluster = i;
        }
    }
    return best_cluster;
}

// Generate a color palette using a simplified k-means clustering
void generate_palette(uint8_t *pixels, int width, int height, int channels, Color *palette, int palette_size) {
    // Initialize centroids randomly
    for (int i = 0; i < palette_size; i++) {
        int index = (rand() % (width * height)) * channels;
        palette[i].r = pixels[index];
        palette[i].g = pixels[index + 1];
        palette[i].b = pixels[index + 2];
    }

    // Perform clustering
    int *assignments = (int *)malloc(width * height * sizeof(int));
    for (int iter = 0; iter < 10; iter++) {  // 10 iterations of k-means
        // Step 1: Assign each pixel to the nearest cluster
        for (int i = 0; i < width * height; i++) {
            Color pixel = {pixels[i * channels], pixels[i * channels + 1], pixels[i * channels + 2]};
            assignments[i] = assign_cluster(pixel, palette, palette_size);
        }

        // Step 2: Update centroids
        int *counts = (int *)calloc(palette_size, sizeof(int));
        uint64_t *sum_r = (uint64_t *)calloc(palette_size, sizeof(uint64_t));
        uint64_t *sum_g = (uint64_t *)calloc(palette_size, sizeof(uint64_t));
        uint64_t *sum_b = (uint64_t *)calloc(palette_size, sizeof(uint64_t));

        for (int i = 0; i < width * height; i++) {
            int cluster = assignments[i];
            counts[cluster]++;
            sum_r[cluster] += pixels[i * channels];
            sum_g[cluster] += pixels[i * channels + 1];
            sum_b[cluster] += pixels[i * channels + 2];
        }

        for (int i = 0; i < palette_size; i++) {
            if (counts[i] > 0) {
                palette[i].r = sum_r[i] / counts[i];
                palette[i].g = sum_g[i] / counts[i];
                palette[i].b = sum_b[i] / counts[i];
            }
        }

        free(counts);
        free(sum_r);
        free(sum_g);
        free(sum_b);
    }

    free(assignments);
}

// Convert a Color to a hex code string
void color_to_hex(Color color, char *hex) {
    sprintf(hex, "#%02X%02X%02X", color.r, color.g, color.b);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <image_file> [palette_size]\n", argv[0]);
        return 1;
    }

    int palette_size = DEFAULT_PALETTE_SIZE;
    if (argc >= 3) {
        palette_size = atoi(argv[2]);
        if (palette_size <= 0) {
            printf("Invalid palette size. Using default size: %d\n", DEFAULT_PALETTE_SIZE);
            palette_size = DEFAULT_PALETTE_SIZE;
        }
    }

    int width, height, channels;
    uint8_t *image_data = stbi_load(argv[1], &width, &height, &channels, 0);

    if (!image_data) {
        printf("Failed to load image: %s\n", argv[1]);
        return 1;
    }

    printf("Image loaded: %dx%d, Channels: %d\n", width, height, channels);

    Color *palette = (Color *)malloc(palette_size * sizeof(Color));
    generate_palette(image_data, width, height, channels, palette, palette_size);

    printf("Generated Palette:\n");
    for (int i = 0; i < palette_size; i++) {
        char hex[8];
        color_to_hex(palette[i], hex);
        printf("Color %d: %s\n", i + 1, hex);
    }

    free(palette);
    stbi_image_free(image_data);
    return 0;
}
