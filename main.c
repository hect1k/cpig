#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_PALETTE_SIZE 5

typedef struct {
  uint8_t r, g, b;
} Color;

float calculate_color_distance(Color a, Color b) {
  return sqrtf((a.r - b.r) * (a.r - b.r) + (a.g - b.g) * (a.g - b.g) +
               (a.b - b.b) * (a.b - b.b));
}

int assign_to_nearest_cluster(Color pixel, Color *centroids, int num_clusters) {
  int best_cluster = 0;
  float min_distance = calculate_color_distance(pixel, centroids[0]);

  for (int i = 1; i < num_clusters; i++) {
    float distance = calculate_color_distance(pixel, centroids[i]);
    if (distance < min_distance) {
      min_distance = distance;
      best_cluster = i;
    }
  }
  return best_cluster;
}

void generate_color_palette(uint8_t *pixels, int width, int height,
                            int channels, Color *palette, int palette_size) {
  if (!palette || !pixels) {
    printf("Error: Invalid inputs for palette generation.\n");
    return;
  }

  for (int i = 0; i < palette_size; i++) {
    int index = (rand() % (width * height)) * channels;
    palette[i].r = pixels[index];
    palette[i].g = pixels[index + 1];
    palette[i].b = pixels[index + 2];
  }

  int *assignments = (int *)malloc(width * height * sizeof(int));
  if (!assignments) {
    printf("Error: Memory allocation failed for assignments.\n");
    return;
  }

  for (int iter = 0; iter < 10; iter++) {
    for (int i = 0; i < width * height; i++) {
      Color pixel = {pixels[i * channels], pixels[i * channels + 1],
                     pixels[i * channels + 2]};
      assignments[i] = assign_to_nearest_cluster(pixel, palette, palette_size);
    }

    int *counts = (int *)calloc(palette_size, sizeof(int));
    uint64_t *sum_r = (uint64_t *)calloc(palette_size, sizeof(uint64_t));
    uint64_t *sum_g = (uint64_t *)calloc(palette_size, sizeof(uint64_t));
    uint64_t *sum_b = (uint64_t *)calloc(palette_size, sizeof(uint64_t));

    if (!counts || !sum_r || !sum_g || !sum_b) {
      printf("Error: Memory allocation failed during clustering.\n");
      free(assignments);
      free(counts);
      free(sum_r);
      free(sum_g);
      free(sum_b);
      return;
    }

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

void convert_color_to_hex(Color color, char *hex) {
  sprintf(hex, "#%02X%02X%02X", color.r, color.g, color.b);
}

void save_palette_to_file(Color *palette, int palette_size,
                          const char *filename) {
  FILE *file = fopen(filename, "w");
  if (!file) {
    printf("Error: Unable to write to file %s\n", filename);
    return;
  }

  for (int i = 0; i < palette_size; i++) {
    char hex[8];
    convert_color_to_hex(palette[i], hex);
    fprintf(file, "%s\n", hex);
  }

  fclose(file);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <image_file> [--colors=<number>] [--output=<file>]\n",
           argv[0]);
    return 1;
  }

  char *image_file = argv[1];
  int palette_size = DEFAULT_PALETTE_SIZE;
  char *output_file = NULL;

  for (int i = 2; i < argc; i++) {
    if (strncmp(argv[i], "--colors=", 9) == 0 ||
        strncmp(argv[i], "-c", 2) == 0) {
      if (strncmp(argv[i], "-c=", 3) == 0) {
        palette_size = atoi(argv[i] + 3); // -c=6
      } else if (strncmp(argv[i], "-c", 2) == 0) {
        if (i + 1 < argc && argv[i + 1][0] != '-') {
          palette_size = atoi(argv[i + 1]); // -c 6
          i++; // Skip the next argument because it's the value for -c
        } else {
          printf("Invalid argument for palette size. Using default size: %d\n",
                 DEFAULT_PALETTE_SIZE);
        }
      } else if (strncmp(argv[i], "--colors=", 9) == 0) {
        palette_size = atoi(argv[i] + 9);
      }

      if (palette_size <= 0) {
        printf("Invalid palette size. Using default size: %d\n",
               DEFAULT_PALETTE_SIZE);
        palette_size = DEFAULT_PALETTE_SIZE;
      }
    } else if (strncmp(argv[i], "--output=", 9) == 0 ||
               strncmp(argv[i], "-o", 2) == 0) {
      if (strncmp(argv[i], "-o=", 3) == 0) {
        output_file = argv[i] + 3; // -o=file.sh
      } else if (strncmp(argv[i], "-o", 2) == 0) {
        if (i + 1 < argc && argv[i + 1][0] != '-') {
          output_file = argv[i + 1]; // -o file.sh
          i++; // Skip the next argument because it's the value for -o
        } else {
          printf("Invalid argument for output file.\n");
        }
      } else if (strncmp(argv[i], "--output=", 9) == 0) {
        output_file = argv[i] + 9;
      }
    } else {
      printf("Unknown argument: %s\n", argv[i]);
      printf("Usage: %s <image_file> [--colors=<number>] [--output=<file>]\n",
             argv[0]);
      return 1;
    }
  }

  int width, height, channels;
  uint8_t *image_data = stbi_load(image_file, &width, &height, &channels, 0);
  if (!image_data) {
    printf("Error: Failed to load image: %s\n", image_file);
    return 1;
  }

  printf("Image loaded: %dx%d, Channels: %d\n", width, height, channels);

  Color *palette = (Color *)malloc(palette_size * sizeof(Color));
  if (!palette) {
    printf("Error: Memory allocation failed for palette.\n");
    stbi_image_free(image_data);
    return 1;
  }

  generate_color_palette(image_data, width, height, channels, palette,
                         palette_size);

  if (output_file) {
    save_palette_to_file(palette, palette_size, output_file);
    printf("Palette written to file: %s\n", output_file);
  } else {
    printf("Generated Palette:\n");
    for (int i = 0; i < palette_size; i++) {
      char hex[8];
      convert_color_to_hex(palette[i], hex);
      printf("Color %d: %s\n", i + 1, hex);
    }
  }

  free(palette);
  stbi_image_free(image_data);
  return 0;
}
