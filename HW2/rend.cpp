#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"


int GzNewRender(GzRender **render, GzDisplay *display)
{
	/*
	- malloc a renderer struct
	- span interpolator needs pointer to display for pixel writes
	*/
	*render = (GzRender*)malloc(sizeof(GzRender));
	(*render)->display = display;
	if (NULL == render)
	{
		return GZ_FAILURE;
	}
	return GZ_SUCCESS;
}


int GzFreeRender(GzRender *render)
{
	/*
	-free all renderer resources
	*/
	//free(render->display);
	free(render);
	return GZ_SUCCESS;
}


int GzBeginRender(GzRender	*render)
{
	/*
	- set up for start of each frame - init frame buffer
	*/
	render->flatcolor[RED] = 0;
	render->flatcolor[GREEN] = 0;
	render->flatcolor[BLUE] = 0;
	return GZ_SUCCESS;
}


int GzPutAttribute(GzRender	*render, int numAttributes, GzToken	*nameList,
	GzPointer *valueList) /* void** valuelist */
{
	/*
	- set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
	- later set shaders, interpolaters, texture maps, and lights
	*/
	if ((NULL == render) || (NULL == nameList) || (NULL == valueList))
	{
		return GZ_FAILURE;
	}
	for (int nCnt = 0; nCnt < numAttributes; nCnt++)
	{
		if (nameList[nCnt] == GZ_RGB_COLOR)
		{
			GzColor* acColor = (GzColor*)valueList[nCnt];
			//set the boundry of the color
			if ((*acColor)[0] < 0)
			{
				(*acColor)[0] = 0;
			}
			else if ((*acColor)[0] > 4095)
			{
				(*acColor)[0] = 4095;
			}
			if ((*acColor)[1] < 0)
			{
				(*acColor)[1] = 0;
			}
			else if ((*acColor)[1] > 4095)
			{
				(*acColor)[1] = 4095;
			}
			if ((*acColor)[2] < 0)
			{
				(*acColor)[2] = 0;
			}
			else if ((*acColor)[2] > 4095)
			{
				(*acColor)[2] = 4095;
			}
			// Set the flatcolor value
			render->flatcolor[RED] = (*acColor)[0];
			render->flatcolor[GREEN] = (*acColor)[1];
			render->flatcolor[BLUE] = (*acColor)[2];
		}
	}
	return GZ_SUCCESS;
}


int GzPutTriangle(GzRender *render, int	numParts, GzToken *nameList,
	GzPointer *valueList)
	/* numParts - how many names and values */
{
	/*
	- pass in a triangle description with tokens and values corresponding to
	GZ_NULL_TOKEN:		do nothing - no values
	GZ_POSITION:		3 vert positions
	- Invoke the scan converter and return an error code
	*/
	if ((NULL == render) || (NULL == nameList) || (NULL == valueList))
	{
		return GZ_FAILURE;
	}
	for (int nCnt = 0; nCnt < numParts; nCnt++)
	{
		if (nameList[nCnt] == GZ_POSITION)
		{
			GzCoord* acGzCoord;
			acGzCoord = (GzCoord*)(valueList[nCnt]);
			GzCoord asGzCoordX = { acGzCoord[0][0], acGzCoord[1][0], acGzCoord[2][0] };
			GzCoord asGzCoordY = { acGzCoord[0][1], acGzCoord[1][1], acGzCoord[2][1] };
			GzCoord asGzCoordZ = { acGzCoord[0][2], acGzCoord[1][2], acGzCoord[2][2] };
			//Sort the matrix according to Y
			for (int anSortYCnt = 1; anSortYCnt < 3; anSortYCnt++)
			{
				if (asGzCoordY[anSortYCnt - 1] > asGzCoordY[anSortYCnt])
				{
					float afSortTemmp;
					afSortTemmp = asGzCoordX[anSortYCnt - 1];
					asGzCoordX[anSortYCnt - 1] = asGzCoordX[anSortYCnt];
					asGzCoordX[anSortYCnt] = afSortTemmp;
					afSortTemmp = asGzCoordY[anSortYCnt - 1];
					asGzCoordY[anSortYCnt - 1] = asGzCoordY[anSortYCnt];
					asGzCoordY[anSortYCnt] = afSortTemmp;
					afSortTemmp = asGzCoordZ[anSortYCnt - 1];
					asGzCoordZ[anSortYCnt - 1] = asGzCoordZ[anSortYCnt];
					asGzCoordZ[anSortYCnt] = afSortTemmp;
				}
			}
			float afGzCoordY_Min = asGzCoordY[0];			// find min y 
			float afGzCoordY_Max = asGzCoordY[2];			// find max y
			float tmp;
			for (int anSortYCnt = 1; anSortYCnt < 3; anSortYCnt++)
			{
				if ((asGzCoordY[anSortYCnt - 1] == asGzCoordY[anSortYCnt]) && (asGzCoordX[anSortYCnt - 1] > asGzCoordX[anSortYCnt]))
				{
					float afSortTemmp;
					afSortTemmp = asGzCoordX[anSortYCnt - 1];
					asGzCoordX[anSortYCnt - 1] = asGzCoordX[anSortYCnt];
					asGzCoordX[anSortYCnt] = afSortTemmp;
					afSortTemmp = asGzCoordY[anSortYCnt - 1];
					asGzCoordY[anSortYCnt - 1] = asGzCoordY[anSortYCnt];
					asGzCoordY[anSortYCnt] = afSortTemmp;
					afSortTemmp = asGzCoordZ[anSortYCnt - 1];
					asGzCoordZ[anSortYCnt - 1] = asGzCoordZ[anSortYCnt];
					asGzCoordZ[anSortYCnt] = afSortTemmp;
				}
			}
			float afCoefA = asGzCoordY[2] - asGzCoordY[0];
			float afCoefB = asGzCoordX[0] - asGzCoordX[2];
			float afCoefC = -afCoefB*asGzCoordY[0] - afCoefA * asGzCoordX[0];
			float acTestX = -(afCoefB*asGzCoordY[1] + afCoefC) / afCoefA;
			if (acTestX < asGzCoordX[1])
			{
				tmp = asGzCoordY[2];
				asGzCoordY[2] = asGzCoordY[1];
				asGzCoordY[1] = tmp;
				tmp = asGzCoordX[2];
				asGzCoordX[2] = asGzCoordX[1];
				asGzCoordX[1] = tmp;
				tmp = asGzCoordZ[2];
				asGzCoordZ[2] = asGzCoordZ[1];
				asGzCoordZ[1] = tmp;
			}
			//find the max and min for X and Y
			float afMaxX, afMinX, afMaxY, afMinY;
			afMaxX = asGzCoordX[0];
			afMinX = asGzCoordX[0];
			afMaxY = asGzCoordY[0];
			afMinY = asGzCoordY[0];
			for (int anCntMinMaxCnt = 1; anCntMinMaxCnt < 3; anCntMinMaxCnt++)
			{
				if (afMaxX <= asGzCoordX[anCntMinMaxCnt])
				{
					afMaxX = asGzCoordX[anCntMinMaxCnt];
				}
				if (afMinX >= asGzCoordX[anCntMinMaxCnt])
				{
					afMinX = asGzCoordX[anCntMinMaxCnt];
				}
				if (afMaxY <= asGzCoordY[anCntMinMaxCnt])
				{
					afMaxY = asGzCoordY[anCntMinMaxCnt];
				}
				if (afMinY >= asGzCoordY[anCntMinMaxCnt])
				{
					afMinY = asGzCoordY[anCntMinMaxCnt];
				}
			}
			//set the boundary of x and y
			if (afMinX < 0)
			{
				afMinX = 0;
			}
			else if (afMinX > 255)
			{
				afMinX = 255;
			}
			if (afMaxX < 0){
				afMaxX = 0;
			}
			else if (afMaxX > 255)
			{
				afMaxX = 255;
			}
			if (afMinY < 0){
				afMinY = 0;
			}
			else if (afMinY > 255)
			{
				afMinY = 255;
			}
			if (afMaxY < 0){
				afMaxY = 0;
			}
			else if (afMaxY > 255)
			{
				afMaxY = 255;
			}
			//set the variants of vertex Ax + By + C = 0
			float afCoefA1, afCoefA2, afCoefA3, afCoefB1, afCoefB2, afCoefB3, afCoefC1, afCoefC2, afCoefC3;
			float AfDiff1, AfDiff2, AfDiff3;
			float afVar1, afVar2, afVar3, afVar4;
			GzIntensity asR, asG, asB, asAlpha;
			GzDepth anZ;
			int anInterZ;
			afCoefA1 = asGzCoordY[1] - asGzCoordY[0];
			afCoefA2 = asGzCoordY[2] - asGzCoordY[1];
			afCoefA3 = asGzCoordY[0] - asGzCoordY[2];
			afCoefB1 = -(asGzCoordX[1] - asGzCoordX[0]);
			afCoefB2 = -(asGzCoordX[2] - asGzCoordX[1]);
			afCoefB3 = -(asGzCoordX[0] - asGzCoordX[2]);
			afCoefC1 = ((-(afCoefB1)*asGzCoordY[1]) - (afCoefA1 * asGzCoordX[1]));
			afCoefC2 = ((-(afCoefB2)*asGzCoordY[2]) - (afCoefA2 * asGzCoordX[2]));
			afCoefC3 = ((-(afCoefB3)*asGzCoordY[0]) - (afCoefA3 * asGzCoordX[0]));
			afVar1 = (((asGzCoordY[1] - asGzCoordY[0])*(asGzCoordZ[2] - asGzCoordZ[0])) - ((asGzCoordY[2] - asGzCoordY[0])*(asGzCoordZ[1] - asGzCoordZ[0])));
			afVar2 = (((asGzCoordX[2] - asGzCoordX[0])*(asGzCoordZ[1] - asGzCoordZ[0])) - ((asGzCoordX[1] - asGzCoordX[0])*(asGzCoordZ[2] - asGzCoordZ[0])));
			afVar3 = (((asGzCoordX[1] - asGzCoordX[0])*(asGzCoordY[2] - asGzCoordY[0])) - ((asGzCoordX[2] - asGzCoordX[0])*(asGzCoordY[1] - asGzCoordY[0])));
			afVar4 = -((asGzCoordY[0] * (afVar2)) + (asGzCoordX[0] * (afVar1)) + (asGzCoordZ[0] * (afVar3)));
			for (int anCntRangeX = int(afMinX); (float)anCntRangeX < (afMaxX); anCntRangeX++){
				for (int anCntRangeY = int(afMinY); (float)anCntRangeY < (afMaxY); anCntRangeY++){
					AfDiff1 = (afCoefA1*anCntRangeX) + (afCoefB1*anCntRangeY) + afCoefC1;
					AfDiff2 = (afCoefA2*anCntRangeX) + (afCoefB2*anCntRangeY) + afCoefC2;
					AfDiff3 = (afCoefA3*anCntRangeX) + (afCoefB3*anCntRangeY) + afCoefC3;
					anInterZ = -(afVar1*anCntRangeX + afVar2*anCntRangeY + afVar4) / afVar3;
					if (!(AfDiff1 < 0 || AfDiff2 < 0 || AfDiff3 < 0)){
						asR = 0;
						asG = 0;
						asB = 0;
						asAlpha = 0;
						anZ = 0;
						if (GZ_FAILURE == GzGetDisplay(render->display, anCntRangeX, anCntRangeY, &asR, &asG, &asB, &asAlpha, &anZ))
						{
							return GZ_FAILURE;
						}
						if (anZ == 0||anInterZ < anZ)
						{
							if (GZ_FAILURE == GzPutDisplay(render->display, anCntRangeX, anCntRangeY, ctoi(render->flatcolor[RED]), ctoi(render->flatcolor[GREEN]), ctoi(render->flatcolor[BLUE]), asAlpha, anInterZ))
							{
								return GZ_FAILURE;
							}
						}
					}
				}
			}
		}
		else if (nameList[nCnt] == GZ_NULL_TOKEN)
		{
			continue;
		}
	}
	return GZ_SUCCESS;
}

/* NOT part of API - just for general assistance */

short	ctoi(float color)		/* convert float color to GzIntensity short */
{
	return(short)((int)(color * ((1 << 12) - 1)));
}

