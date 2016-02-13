/*   CS580 HW1 display functions to be completed   */

#include   "stdafx.h"  
#include	"Gz.h"
#include	"disp.h"


int GzNewFrameBuffer(char** framebuffer, int width, int height)
{
	/* HW1.1 create a framebuffer for MS Windows display:
	-- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
	-- pass back pointer
	*/
	//For there is three colors for RGB
	int anColorNumber = 3;
	//if the width and height is below, the function should return GZ_FAILURE
	if ((width < 0) || (height < 0))
	{
		return GZ_FAILURE;
	}
	//declare the total color number 
	*framebuffer = (char*)malloc(anColorNumber*width*height);
	if (*framebuffer == NULL)
	{
		return GZ_FAILURE;
	}
	return GZ_SUCCESS;
}

int GzNewDisplay(GzDisplay	**display, int xRes, int yRes)
{
	/* HW1.2 create a display:
	-- allocate memory for indicated resolution
	-- pass back pointer to GzDisplay object in display
	*/
	//if the width or the height is bigger than MAX, the function should return GZ_FAILURE
	if ((xRes > MAXXRES) || (yRes > MAXYRES))
	{
		return GZ_FAILURE;
	}
	//Declare new GzDisplay object
	GzDisplay* astGzDisplay = new GzDisplay;
	//Initiliaze the new object
	*display = astGzDisplay;
	astGzDisplay->xres = xRes;
	astGzDisplay->yres = yRes;
	//Allocate new memory for the new GzDisplay object
	//Due to the fact that in the function GzNewFrameBuffer give every pixel three bytes, so in the function called below, size should devided by three
	GzNewFrameBuffer((char**)&astGzDisplay->fbuf, xRes*yRes, sizeof(GzPixel) / 3);
	//If the memory allocation fails, the function should return GZ_FAILURE
	if (astGzDisplay->fbuf == NULL)
	{
		return GZ_FAILURE;
	}
	return GZ_SUCCESS;
}


int GzFreeDisplay(GzDisplay	*display)
{
	/* HW1.3 clean up, free memory */
	//For using malloc to allocate new memory, it is necessary to use free function to release structure GzPixel in display structure
	free(display->fbuf);
	//For using new to allocate new memory of display, it is necessay to use delete to release display structure
	delete display;
	return GZ_SUCCESS;
}


int GzGetDisplayParams(GzDisplay *display, int *xRes, int *yRes)
{
	/* HW1.4 pass back values for a display */
	//If the display is null, the function should not operated
	if (display == NULL)
	{
		return GZ_FAILURE;
	}
	//Get xRes and yRes values from display structure
	*xRes = display->xres;
	*yRes = display->yres;
	return GZ_SUCCESS;
}


int GzInitDisplay(GzDisplay	*display)
{
	/* HW1.5 set everything to some default values - start a new frame */

	GzPixel* astGzPixel = display->fbuf;
	int anRes = display->xres * display->yres;
	//Set the init values for RGBAZ. Due to the fact that prof said the background could be customized, black is used.
	for (int anCnt = 0; anCnt < anRes; anCnt++)
	{
		astGzPixel->alpha = 0;
		astGzPixel->blue = 0;
		astGzPixel->green = 0;
		astGzPixel->red = 0;
		astGzPixel->z = 0;
		astGzPixel++;
	}
	return GZ_SUCCESS;
}


int GzPutDisplay(GzDisplay *display, int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
	/* HW1.6 write pixel values into the display */
	//if the x or y does fit the range of pic, the drawing of this point should be ignored
	if (i < 0)
	{
		return GZ_FAILURE;
	}
	else if (i >= display->xres)
	{
		return GZ_FAILURE;
	}
	if (j < 0)
	{
		return GZ_FAILURE;
	}
	else if (j >= display->yres)
	{
		return GZ_FAILURE;
	}
	int anRelx, anRely;
	GzPixel* auWritePixel = display->fbuf;
	anRelx = i;
	anRely = j;
	auWritePixel = auWritePixel + ARRAY(anRelx, anRely);
	//Due to the fact that the rects are using 12 bit RGB, it is necessary to transform them to 8 bit
	//Define the bounds of RGB
	if (r < 0)
	{
		r = 0;
	}
	else if (r > 4095)
	{
		r = 4095;
	}
	if (g < 0)
	{
		g = 0;
	}
	else if (g > 4095)
	{
		g = 4095;
	}
	if (b < 0)
	{
		b = 0;
	}
	else if (b > 4095)
	{
		b = 4095;
	}
	auWritePixel->red = r / 16;
	auWritePixel->green = g / 16;
	auWritePixel->blue = b / 16;
	auWritePixel->alpha = a;
	auWritePixel->z = z;
	return GZ_SUCCESS;
}


int GzGetDisplay(GzDisplay *display, int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
	/* HW1.7 pass back a pixel value to the display */
	//If the i or j is bigger than the range of pic, the function should return failure
	if ((i >= display->xres) || (j >= display->yres))
	{
		return GZ_FAILURE;
	}
	//if the display is null, the function should not be operated
	if (display == NULL)
	{
		return GZ_FAILURE;
	}
	int anCnt = 0;
	int anRes = ARRAY(i, j);
	GzPixel* apGzPixel = &(display->fbuf[anRes]);
	//Put the Display RGB into *r *g *b *a *z
		*r = apGzPixel->red;
		*g = apGzPixel->green;
		*b = apGzPixel->blue;
		*a = apGzPixel->alpha;
		*z = apGzPixel->z;
		apGzPixel++;
	return GZ_SUCCESS;
}


int GzFlushDisplay2File(FILE* outfile, GzDisplay *display)
{

	/* HW1.8 write pixels to ppm file -- "P6 %d %d 255\r" */
	if (display == NULL)
	{
		return GZ_FAILURE;
	}
	int anRes = display->xres * display->yres;
	GzPixel* astGzPixel = display->fbuf;
	fputs("P6 ", outfile);
	fprintf(outfile, "%d %d ", display->xres, display->yres);
	fputs("255\n", outfile);
	for (int anCnty = 0; anCnty < display->xres; anCnty++)
	{
		for (int anCntx = 0; anCntx < display->yres; anCntx++)
		{
			fprintf(outfile, "%c%c%c", astGzPixel->red, astGzPixel->green, astGzPixel->blue);
			astGzPixel++;
		}
	}
	return GZ_SUCCESS;
}

int GzFlushDisplay2FrameBuffer(char* framebuffer, GzDisplay *display)
{

	/* HW1.9 write pixels to framebuffer:
	- put the pixels into the frame buffer
	- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red
	- NOT red, green, and blue !!!
	*/
	//if the display pointers is null or x, y in display is over the range, the function should return failure 
	if ((framebuffer == NULL) || (display == NULL) || (display->xres >= MAXXRES) || (display->yres >= MAXYRES))
	{
		return GZ_FAILURE;
	}
	//Getting the buffer volumne
	int anRes = display->xres * display->yres;
	//put the display into framebuffer
	char* apframebufferTemp = framebuffer;
	GzPixel* astGzPixel = display->fbuf;
	for (int anCnt = 0; anCnt < anRes; anCnt++)
	{
		*apframebufferTemp = (char)astGzPixel->blue;
		*(apframebufferTemp + 1) = (char)astGzPixel->green;
		*(apframebufferTemp + 2) = (char)astGzPixel->red;
		astGzPixel++;
		apframebufferTemp = apframebufferTemp + 3;
	}
	return GZ_SUCCESS;
}