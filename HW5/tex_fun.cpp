/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"

GzColor	*image = NULL;
int xs, ys;
int reset = 1;
#define	ARRAY(x,y)	(x + (y * xs))
/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
	unsigned char		pixel[3];
	unsigned char     dummy;
	char  		foo[8];
	int   		i, j;
	FILE			*fd;

	if (reset) {          /* open and load texture file */
		fd = fopen("texture", "rb");
		if (fd == NULL) {
			fprintf(stderr, "texture file not found\n");
			exit(-1);
		}
		fscanf(fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
		image = (GzColor*)malloc(sizeof(GzColor)*(xs + 1)*(ys + 1));
		if (image == NULL) {
			fprintf(stderr, "malloc for texture image failed\n");
			exit(-1);
		}

		for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
			fread(pixel, sizeof(pixel), 1, fd);
			image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
			image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
			image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
		}

		reset = 0;          /* init is done */
		fclose(fd);
	}

	/* bounds-test u,v to make sure nothing will overflow image array bounds */
	/* determine texture cell corner values and perform bilinear interpolation */
	/* set color to interpolated GzColor value and return */
	//to ([0, xs-1], [0, ys-1])
	if (u < 0)
	{
		u = 0;
	}
	else if (u > 1)
	{
		u = 1;
	}
	if (v < 0)
	{
		v = 0;
	}
	else if (v > 1)
	{
		v = 1;
	}

	float fU = u*((float)xs - 1.0);
	float fV = v*((float)ys - 1.0);
	float fs = fU - floor(fU);
	float ft = fV - floor(fV);
	int pointA[2] = { (int)floor(fU), (int)floor(fV) };
	int pointB[2] = { (int)ceil(fU), (int)floor(fV) };
	int pointC[2] = { (int)ceil(fU), (int)ceil(fV) };
	int pointD[2] = { (int)floor(fU), (int)ceil(fV) };
	color[RED] = (fs*ft*image[pointC[X] + pointC[Y] * xs][RED]) + (fs*(1 - ft)*image[pointB[X] + pointB[Y] * xs][RED]) + ((1 - fs)*(1 - ft)*image[pointA[X] + pointA[Y] * xs][RED]) + ((1 - fs)*ft*image[pointD[X] + pointD[Y] * xs][RED]);
	color[GREEN] = (fs*ft*image[pointC[X] + pointC[Y] * xs][GREEN]) + (fs*(1 - ft)*image[pointB[X] + pointB[Y] * xs][GREEN]) + ((1 - fs)*(1 - ft)*image[pointA[X] + pointA[Y] * xs][GREEN]) + ((1 - fs)*ft*image[pointD[X] + pointD[Y] * xs][GREEN]);
	color[BLUE] = (fs*ft*image[pointC[X] + pointC[Y] * xs][BLUE]) + (fs*(1 - ft)*image[pointB[X] + pointB[Y] * xs][BLUE]) + ((1 - fs)*(1 - ft)*image[pointA[X] + pointA[Y] * xs][BLUE]) + ((1 - fs)*ft*image[pointD[X] + pointD[Y] * xs][BLUE]);

	return GZ_SUCCESS;
}

int	HSV2RGB(GzColor color, double dValH, double &dValS, double &dValV)
{
	BYTE	var_R = 0, var_G = 0, var_B = 0;
	double	var_h = 0.0;
	int		var_i = 0;
	double	var_1 = 0.0, var_2 = 0.0, var_3 = 0.0;
	double	var_r = 0.0, var_g = 0.0, var_b = 0.0;
	if (0 < dValS && dValS < 0.0000001)		//HSV from 0 to 1
	{
		var_R = dValV * 255;
		var_G = dValV * 255;
		var_B = dValV * 255;
	}
	else
	{
		var_h = dValH * 6;
		if (var_h == 6) var_h = 0;      //H must be < 1
		var_i = int(var_h);           //Or ... var_i = floor( var_h )
		var_1 = dValV * (1 - dValS);
		var_2 = dValV * (1 - dValS * (var_h - var_i));
		var_3 = dValV * (1 - dValS * (1 - (var_h - var_i)));

		if (var_i == 0) { var_r = dValV; var_g = var_3; var_b = var_1; }
		else if (var_i == 1) { var_r = var_2; var_g = dValV; var_b = var_1; }
		else if (var_i == 2) { var_r = var_1; var_g = dValV; var_b = var_3; }
		else if (var_i == 3) { var_r = var_1; var_g = var_2; var_b = dValV; }
		else if (var_i == 4) { var_r = var_3; var_g = var_1; var_b = dValV; }
		else                   { var_r = dValV; var_g = var_1; var_b = var_2; }

		color[RED] = var_r;                
		color[GREEN] = var_g;
		color[BLUE] = var_b;
	}

	return	GZ_SUCCESS;
}

/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color)
{

	if (u < 0)
	{
		u = 0;
	}
	else if (u > 1)
	{
		u = 1;
	}
	if (v < 0)
	{
		v = 0;
	}
	else if (v > 1)
	{
		v = 1;
	}
	//Implement F(X) = X^2 +0.285+0.01i  N=1000 W=1 H=1
	xs = ys = 1;
	float Xo[2] = {u, v};// { (u - xs / 2) / (xs / 2), (v - ys / 2) / (ys / 2) };
	int nCnt;
	for (nCnt = 0; nCnt < 1000; nCnt++)
	{
		float fXo0 = (Xo[0] * Xo[0] - Xo[1] * Xo[1]) + 0.285;
		float fXo1 = 2*(Xo[0] * Xo[1]) + 0.01;
		Xo[0] = fXo0; Xo[1] = fXo1;
		if ((Xo[0] * Xo[0] + Xo[1] * Xo[1]) > 4)
		{
			break;
		}
	}
	double dValS = abs(Xo[0])  / sqrt(Xo[0] * Xo[0] + Xo[1] * Xo[1]);
	double dValV = 1;
	HSV2RGB(color, sqrt(Xo[0] * Xo[0] + Xo[1] * Xo[1]) / (abs(Xo[1]) + abs(Xo[0])), dValS, dValV);
	return GZ_SUCCESS;
}

/* Free texture memory */
int GzFreeTexture()
{
	if (image != NULL)
		free(image);
	return GZ_SUCCESS;
}

