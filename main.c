#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "vector3.h"
#include "color3.h"
struct PPM_Image{
    int width;
    int height;
    uint8_t *pixels;
};
typedef struct PPM_Image PPM_Image;

int PPM_Export(const char *filename,const PPM_Image *image){
    if (!filename || !image || !image->pixels){
        return 0;
    }
    if (image->width <= 0|| image->height <= 0){
        return 0;
    }
    
    FILE *file = fopen64(filename, "wb");
    if (!file){
        return 0;
    }
    
    // write
    fprintf(file, "P6\n");
    fprintf(file, "%d %d\n", image->width, image->height);
    fprintf(file, "255\n");

    size_t pixel_count = (size_t)image->width * image->height;

    size_t wrote = fwrite(image->pixels, 3, pixel_count, file);

    int success = (wrote == pixel_count);
    if (fclose(file) != 0){
        success = 0;
    }
    return success;
}

void SetPixel(PPM_Image *image, int x, int y, Color3 color){
    if (x < 0 || x > image->width || y < 0 ||y > image->height ){
        return;
    }

    size_t index = ((size_t)y * image->width + x)* 3;
    image->pixels[index + 0] = color.r;
    image->pixels[index + 1] = color.g;
    image->pixels[index + 2] = color.b;
}

void DrawCircle(PPM_Image *image, int cx, int cy, int radius, Color3 color){
    for (int y = cy - radius; y <= cy + radius; y++){
        for (int x = cx - radius; x <= cx + radius; x++){

            int dx = x - cx;
            int dy = y - cy;

            if (dx * dx + dy * dy <= radius * radius){
                SetPixel(image, x, y, color);
            }
        }
    }
}

int main(){
    srand((unsigned int)time(NULL));

    PPM_Image image;
    image.width = 1280;
    image.height = 720;

    size_t pixel_count = (size_t)image.width * image.height;
    size_t buffer_size = pixel_count * 3;

    image.pixels = malloc(buffer_size);
    if (!image.pixels){
        return 1;
    }

    for (size_t i = 0; i < buffer_size; i++){
        image.pixels[i] = 255;
    }
    DrawCircle(&image, image.width / 2, image.height / 2, 100, (Color3){255, 0,0});
    
    if (!PPM_Export("test.ppm", &image)){
        free(image.pixels);
        return 1;
    }
    return 0;
}