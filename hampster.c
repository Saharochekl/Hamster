#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <strings.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


/*
 The heart of your algo!
*/
void dfs(int v, int color, int iw, int ih, int* col, unsigned char* mat)
{
  //TODO Найти компоненты связности в  графе при помощи поиска в глубину
}


/*
    Let's convert RGB image to gray using magic formula for weights.
*/
unsigned char* color_to_gray(unsigned char* MyImage, int sizeV, int sizeH, int step)
{
    int k = 0;
    unsigned char* grayImage = (unsigned char*)malloc(sizeV*sizeH*sizeof(unsigned char));
    if ( grayImage == NULL)
    {
            printf("Memory allocation error at color_to_gray(): %d, %d\n", sizeV, sizeH );
            return 1;
    }

    for (int i = 0; i < size; i = i + step) {
        MyImage[k] = 0.299*idata[i] + 0.587*idata[i + 1] + 0.114*idata[i + 2];
        k = k + 1;
    }
    return grayImage;

}

/*
  If the gray value is less then t_black (threshold for black) - let's make it black
  If the gray value is moree then t_white (threshold for white) - let's make it white
*/
unsigned char*  gray_to_bw( unsigned char* Image, int sizeV, int sizeH, int t_black, int t_white)
{
    for (int i = 2; i < sizeH-1; i++)
    {
        for (int j = 2; j < sizeV-1; j++)
        {
            if (Image[sizeV*i+j] < t_black)
                Image[sizeV*i+j] = 0;
            if (Image[sizeV*i+j] > t_white)
                Image[sizeV*i+j] = 255;
        }
    }
}


int main() {

    // строка, путь к файлу
    char* inputPath = "hampster.png";
    int iw, ih, n; //ширина, высота и количество цветовых каналов

    // Загружаем изображение, чтобы получить информацию о ширине, высоте и цветовом канале
    unsigned char *idata = stbi_load(inputPath, &iw, &ih, &n, 0);
    if (idata == NULL) {
        // Ошибка загрузки - сразу же ругаемся и выходим с ненулевым кодом ошибки
        printf("ERROR: can't read file %s\n", inputPath );
        return 1;
    }

    int i, k, j;
    k = 0;
    // Выделим память для дальнейших операций и преобразований
    unsigned char* odata = (unsigned char*)malloc(ih*iw*n*sizeof(unsigned char));
    unsigned char* newImage = (unsigned char*)malloc(ih*iw*sizeof(unsigned char));
    // Не забываем проверять корректность выделения памяти!
    if ( (odata == NULL) || (newImage == NULL) ) {
            printf("Memory allocation error at main()" );
            return 1;
        }

    MyImage = color_to_gray(idata, iw, ih, n);

    //preparation: convert gray to black-white
    int t_black = 100;
    int t_white = 160;
    MyImage = gray_to_bw(MyImage, iw, ih, t_black, t_white);
    /*

    // Implement these functions yourself if need!
    MyImage = bw_gauss(MyImage, iw, ih, t_black, t_white);
    MyImage = bw_sobel(MyImage, iw, ih, t_black, t_white);
    */
    
    /*
     Let's colorize different connectivity components on the picture
    */
    int col[iw*ih];
    for (i = 0; i < iw*ih; i++) { 
        col[i] = 0;
    }
    k = 55;
    //dfs making
    for (i = 0; i < iw*ih; i++) {
        if (col[i] == 0) {
            dfs(i, k, iw, ih, col, newImage);
            k = k + 50;
        }
    }
    
    //now have to color the colors from col
    for (i = 0; i < iw*ih; i++) {
        odata[i*n] = 78+col[i]+0.5*col[i-1];
        odata[i*n+1] = 46+col[i];
        odata[i*n+2] = 153+col[i];
        if (n == 4) odata[i*n+3] = 255;
    }
    
    char* outputPath = "output.png";
  
    stbi_write_png(outputPath, iw, ih, n, odata, 0);
    /*
        Не забываем очищать за собой ранее выделенную память!
    */
    free(newImage);
    free(MyImage);
    free(odata);
    stbi_image_free(idata);
    
    return 0;
}