#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include<strings.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mm_malloc.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>


void dfs(int v, int color, int len, int hight, int* col, unsigned char* mat)
{
    int q,w,e,r;
    col[v] = color;
    if (v+len+1 < len * hight)
    {
        q = mat[v+len+1];
        if ((col[v+len+1] == 0) && (abs(mat[v] - q) < 5))
        {
            dfs(v+len+1, color, len, hight, col, mat);
        }
    }

    if (v-len-1 > 0)
    {
        w = mat[v-len-1];
        if ((col[v-len-1] == 0) && (abs(mat[v] - w) < 5))
        {
            dfs(v-len-1, color, len, hight, col, mat);
        }
    }


    if (v-len+1 > 0)
    {
        e = mat[v-len+1];
        if ((col[v-len+1] == 0) && (abs(mat[v] - e) < 5))
        {
            dfs(v-len+1, color, len, hight, col, mat);
        }
    }

    if (v+len-1 < hight * len)
    {
        r = mat[v+len-1];
        if ((col[v+len-1] == 0) && (abs(mat[v] - r) < 5))
        {
            dfs(v+len-1, color, len, hight, col, mat);
        }
    }

}


int RGB_to_gray(char r, char g, char b)
{
    return (r*11+g*16+5*b)/32;
}

unsigned char*  gray_to_bw( unsigned char* Image, int sizeV, int sizeH, int t_black, int t_white) //добавление контраста и чёткое разделение границ между белым и чёрным
{
    for (int i = 2; i < sizeH-1; i++)
    {
        for (int j = 2; j < sizeV-1; j++)
        {
            if (Image[sizeV*i+j] < t_black)
            {
                Image[sizeV*i+j] = 0;
            }
            if (Image[sizeV*i+j] > t_white)
            {
                Image[sizeV*i+j] = 255;
            }
        }
    }
    return Image;
}

unsigned char* bw_gauss(unsigned char* Image, int ln, int hight, unsigned char* odata)
{
    for (int i = 1; i < hight - 1; ++i)
    {
        for (int j = 2; j < ln - 1; ++j)
        {
            Image[ln * i + j] = 0.15 * odata[ln * i + j] + 0.12 * odata[ln * (i + 1) + j] + 0.12 * odata[ln * (i - 1) + j];
            Image[ln * i + j] = Image[ln * i + j] + 0.12 * odata[ln * i + (j + 1)] + 0.12 * odata[ln * i + (j - 1)];
            Image[ln * i + j] = Image[ln * i + j] + 0.09 * odata[ln * (i + 1) + (j + 1)] + 0.09 * odata[ln * (i + 1) + (j - 1)];
            Image[ln * i + j] = Image[ln * i + j] + 0.09 * odata[ln * (i - 1) + (j + 1)] + 0.09 * odata[ln * (i - 1) + (j - 1)];
        }
    }
    return Image;
}

unsigned char* bw_sobel(unsigned char* Image, int len, int hight, unsigned char* data)
{
    unsigned char f,b,k;
    for(int i = 2; i < hight - 1; i++)
    {
        for(int j = 2; j < len - 1; j++)
        {
            f = Image[len * (i - 1) + (j - 1)] - Image[len * i + j];
            b = Image[len * (i - 1) + j] - Image[len * i + (j + 1)];
            k = sqrt(pow(f, 2) - pow(b, 2));
            data[len * i + j] = k;
        }
    }
    return Image;
}

unsigned char* coloring( unsigned char* graph, int len, int hight, int col[])
{
    for(int i = 0; i < len * hight; ++i)
    {
        graph[i*4] = 80 + col[i] + 0.5 * col[len * hight];
        graph[i*4+1] = 50 + col[i];
        graph[i*4+2] = 150 + col[i];
        graph[i*4+3] = 255;
    }
    return graph;
}

int main()
{
    char* inPath = "hampster.png";
    int len, hight, n, i, s, tmp = 0;
    unsigned char * data1 = stbi_load(inPath, &len, &hight, &n, 0);
    if (data1 == NULL)
    {
        printf("ERROR: can't read file \n");
        return 1;
    }
    printf("len = %d,  hight = %d, len*hight =  %d", len, hight, len * hight);
    unsigned char* graph = (unsigned char*)malloc(len * hight * n * (sizeof(unsigned char)));
    if (graph == NULL)
    {
        printf("ERROR: can't allocate memory for graph \n");
        return 1;
    }
    int* col = (int*)malloc(len * hight * (sizeof(int)));
    unsigned char* data2=malloc(len * hight * (sizeof(unsigned char)));
    if (data2 == NULL)
    {
        printf("ERROR: can't allocate memory for output \n");
        return 1;
    }
    unsigned char* Filter = (unsigned char*)malloc(hight * len * sizeof(unsigned char));

    for(i = 0; i < len * hight * n; i+= n)
    {
        char r = data1[i];
        char g = data1[i + 1];
        char b = data1[i + 2];
        data2[tmp]= RGB_to_gray (r, g, b);tmp+=1;
    }

    gray_to_bw(data2, len, hight, 100, 160);
    bw_gauss(Filter, len, hight, data2);
    bw_sobel(Filter, len, hight, data2);

    for(s = 0; s < len * hight; ++s)
    {
        col[s] = 0;
    }
    int d = 150;
    for(i = 0; i < len * hight; ++i)
    {
        if(col[i] == 0)
        {
            dfs(i, d, len, hight, col, Filter);
            d += 150;
        }
    }
    coloring(graph, len, hight, col);

    char *outputPath = "output.png";
    char *filteredPath = "filtered.png";
    free(col);
    stbi_write_png(filteredPath, len, hight, 1, Filter, 0);
    free(Filter);
    stbi_write_png(outputPath, len, hight, n, graph, 0);
    stbi_image_free(data1);
    free(data2);
    free(graph);
    return 0;
}
