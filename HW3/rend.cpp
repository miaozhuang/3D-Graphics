/* CS580 Homework 3 */
#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

int GzRotXMat(float degree, GzMatrix mat)
{
// Create rotate matrix : rotate along x axis
// Pass back the matrix using mat value
	float radius = degree*3.14 / 180;
	float Rx[4][4] = {{ 1, 0, 0, 0 }, { 0, cos(radius), -sin(radius), 0 }, { 0, sin(radius), cos(radius), 0 }, {0, 0, 0, 1}};
	float prod[4][4];
	MatrixProduct(Rx, mat, prod);
	for (int nCntRow = 0; nCntRow < 4; nCntRow++)
	{
		for (int nCntLine = 0; nCntLine < 4; nCntLine++)
		{
			mat[nCntRow][nCntLine] = prod[nCntRow][nCntLine];
		}
	}
	return GZ_SUCCESS;
}


int GzRotYMat(float degree, GzMatrix mat)
{
// Create rotate matrix : rotate along y axis
// Pass back the matrix using mat value
	float radius = degree*3.14 / 180;
	float Rx[4][4] = { { cos(radius), 0, sin(radius), 0 }, { 0, 1, 0, 0 }, { -sin(radius), 0, cos(radius), 0 }, { 0, 0, 0, 1 } };
	float prod[4][4];
	MatrixProduct(Rx, mat, prod);
	for (int nCntRow = 0; nCntRow < 4; nCntRow++)
	{
		for (int nCntLine = 0; nCntLine < 4; nCntLine++)
		{
			mat[nCntRow][nCntLine] = prod[nCntRow][nCntLine];
		}
	}
	return GZ_SUCCESS;
}


int GzRotZMat(float degree, GzMatrix mat)
{
// Create rotate matrix : rotate along z axis
// Pass back the matrix using mat value
	float radius = degree*3.14 / 180;
	float Rx[4][4] = { { cos(radius), -sin(radius), 0, 0 }, { sin(radius), cos(radius), 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
	float prod[4][4];
	MatrixProduct(Rx, mat, prod);
	for (int nCntRow = 0; nCntRow < 4; nCntRow++)
	{
		for (int nCntLine = 0; nCntLine < 4; nCntLine++)
		{
			mat[nCntRow][nCntLine] = prod[nCntRow][nCntLine];
		}
	}
	return GZ_SUCCESS;
}


int GzTrxMat(GzCoord translate, GzMatrix mat)
{
// Create translation matrix
// Pass back the matrix using mat value
	float trans[4][4] = { { 1, 0, 0, translate[0] }, { 0, 1, 0, translate[1] }, { 0, 0, 1, translate[2] }, { 0, 0, 0, 1 } };
	float prod[4][4];
	MatrixProduct(trans, mat, prod);
	for (int nCntRow = 0; nCntRow < 4; nCntRow++)
	{
		for (int nCntLine = 0; nCntLine < 4; nCntLine++)
		{
			mat[nCntRow][nCntLine] = prod[nCntRow][nCntLine];
		}
	}
	return GZ_SUCCESS;
}


int GzScaleMat(GzCoord scale, GzMatrix mat)
{
// Create scaling matrix
// Pass back the matrix using mat value
	float scal[4][4] = { { scale[0], 0, 0, 0 }, { 0, scale[1], 0, 0 }, { 0, 0, scale[2], 0 }, { 0, 0, 0, 1 } };
	float prod[4][4];
	MatrixProduct(scal, mat, prod);
	for (int nCntRow = 0; nCntRow < 4; nCntRow++)
	{
		for (int nCntLine = 0; nCntLine < 4; nCntLine++)
		{
			mat[nCntRow][nCntLine] = prod[nCntRow][nCntLine];
		}
	}
	return GZ_SUCCESS;
}


//----------------------------------------------------------
// Begin main functions

int GzNewRender(GzRender **render, GzDisplay	*display)
{
/*  
- malloc a renderer struct 
- setup Xsp and anything only done once 
- save the pointer to display 
- init default camera 
*/ 
	*render = (GzRender*)malloc(sizeof(GzRender));
	(*render)->display = display;
	if (NULL == render)
	{
		return GZ_FAILURE;
	}
	(*render)->matlevel = -1;
	(*render)->camera.FOV = DEFAULT_FOV;
	(*render)->camera.position[X] = DEFAULT_IM_X;
	(*render)->camera.position[Y] = DEFAULT_IM_Y;
	(*render)->camera.position[Z] = DEFAULT_IM_Z;
	(*render)->camera.lookat[X] = 0.0;
	(*render)->camera.lookat[Y] = 0.0;
	(*render)->camera.lookat[Z] = 0.0;
	(*render)->camera.worldup[X] = 0.0;
	(*render)->camera.worldup[Y] = 1.0;
	(*render)->camera.worldup[Z] = 0.0;
	(*render)->display = display;
	(*render)->Xsp[0][0] = (display->xres) / 2;
	(*render)->Xsp[0][1] = 0;
	(*render)->Xsp[0][2] = 0;
	(*render)->Xsp[0][3] = (display->xres) / 2;
	(*render)->Xsp[1][0] = 0;
	(*render)->Xsp[1][1] = -((display->yres) / 2);
	(*render)->Xsp[1][2] = 0;
	(*render)->Xsp[1][3] = (display->yres) / 2;
	(*render)->Xsp[2][0] = 0;
	(*render)->Xsp[2][1] = 0;
	(*render)->Xsp[2][2] = INT_MAX *tan(((DEFAULT_FOV) / 2)* (3.14 / 180));
	(*render)->Xsp[2][3] = 0;
	(*render)->Xsp[3][0] = 0;
	(*render)->Xsp[3][1] = 0;
	(*render)->Xsp[3][2] = 0;
	(*render)->Xsp[3][3] = 1;
	//memset((*render)->Ximage, 0, MATLEVELS * 16);
	return GZ_SUCCESS;
}


int GzFreeRender(GzRender *render)
{
/* 
-free all renderer resources
*/
	free(render);
	return GZ_SUCCESS;
}



int GzBeginRender(GzRender *render)
{
/*  
- setup for start of each frame - init frame buffer color,alpha,z
- compute Xiw and projection xform Xpi from camera definition 
- init Ximage - put Xsp at base of stack, push on Xpi and Xiw 
- now stack contains Xsw and app can push model Xforms when needed 
*/ 
	render->flatcolor[RED] = 0;
	render->flatcolor[GREEN] = 0;
	render->flatcolor[BLUE] = 0;
	//calculate the CL
	GzCamera *camera = &(render->camera);
	for (int nCntRow = 0; nCntRow < 4; nCntRow++)
	{
		for (int nCntLine = 0; nCntLine < 4; nCntLine++)
		{
			camera->Xpi[nCntRow][nCntLine] = 0;
			camera->Xiw[nCntRow][nCntLine] = 0;
			render->Xsp[nCntRow][nCntLine] = 0;
		}
	}
	float fovTemp = 3.14*camera->FOV / 180;
	float tempd = tan((fovTemp) / 2);
	camera->Xpi[0][0] = 1;
	camera->Xpi[1][1] = 1;
	camera->Xpi[2][2] = tempd;
	camera->Xpi[3][2] = tempd;
	camera->Xpi[3][3] = 1;
	float cl[3] = { camera->lookat[X] - camera->position[X], camera->lookat[Y] - camera->position[Y], camera->lookat[Z] - camera->position[Z] };
	float clLength = sqrt(pow(cl[X], 2) + pow(cl[Y], 2) + pow(cl[Z], 2));
	float vectorZ[3] = { cl[X] / clLength, cl[Y] / clLength, cl[Z] / clLength };
	float productUPZ = vectorZ[X] * camera->worldup[X] + vectorZ[Y] * camera->worldup[Y] + vectorZ[Z] * camera->worldup[Z];
	float upapo[3] = { camera->worldup[X] - productUPZ*vectorZ[X], camera->worldup[Y] - productUPZ*vectorZ[Y], camera->worldup[Z] - productUPZ*vectorZ[Z] };
	float upapoLength = sqrt(pow(upapo[X], 2) + pow(upapo[Y], 2) + pow(upapo[Z], 2));
	float vectorY[3] = { upapo[X] / upapoLength, upapo[Y] / upapoLength, upapo[Z] / upapoLength };
	float vectorX[3] = { vectorY[Y] * vectorZ[Z] - vectorY[Z] * vectorZ[Y], vectorY[Z] * vectorZ[X] - vectorY[X] * vectorZ[Z], vectorY[X] * vectorZ[Y] - vectorY[Y] * vectorZ[X] };
	camera->Xiw[0][0] = vectorX[0]; camera->Xiw[0][1] = vectorX[1]; camera->Xiw[0][2] = vectorX[2]; camera->Xiw[0][3] = -dotproduct(vectorX, camera->position);
	camera->Xiw[1][0] = vectorY[0]; camera->Xiw[1][1] = vectorY[1]; camera->Xiw[1][2] = vectorY[2]; camera->Xiw[1][3] = -dotproduct(vectorY, camera->position);
	camera->Xiw[2][0] = vectorZ[0]; camera->Xiw[2][1] = vectorZ[1]; camera->Xiw[2][2] = vectorZ[2]; camera->Xiw[2][3] = -dotproduct(vectorZ, camera->position);
	camera->Xiw[3][3] = 1;
	render->Xsp[0][0] = render->display->xres / 2; render->Xsp[0][3] = render->display->xres / 2;
	render->Xsp[1][1] = -render->display->yres / 2; render->Xsp[1][3] = render->display->yres / 2;
	render->Xsp[2][2] = INT_MAX;// *tempd;
	render->Xsp[3][3] = 1;
	GzPushMatrix(render, render->Xsp);
	// push Xpi
	GzPushMatrix(render, render->camera.Xpi);
	// push Xiw
	GzPushMatrix(render, render->camera.Xiw);
	return GZ_SUCCESS;
}


int GzPutCamera(GzRender *render, GzCamera *camera)
{
/*
- overwrite renderer camera structure with new camera definition
*/
	render->camera = *camera;
	return GZ_SUCCESS;	
}



int GzPushMatrix(GzRender *render, GzMatrix	matrix)
{
	/*
	- push a matrix onto the Ximage stack
	- check for stack overflow
	*/
	render->matlevel++;
	if (render->matlevel == 0)
	{
		for (int nCntRow = 0; nCntRow < 4; nCntRow++)
		{
			for (int nCntLine = 0; nCntLine < 4; nCntLine++)
			{
				render->Ximage[render->matlevel][nCntRow][nCntLine] = matrix[nCntRow][nCntLine];
			}
		}
	}
	else if (render->matlevel < MATLEVELS)
	{
		float prodTemp[4][4];
		MatrixProduct(render->Ximage[render->matlevel - 1], matrix, prodTemp);
		for (int nCntRow = 0; nCntRow < 4; nCntRow++)
		{
			for (int nCntLine = 0; nCntLine < 4; nCntLine++)
			{
				render->Ximage[render->matlevel][nCntRow][nCntLine] = prodTemp[nCntRow][nCntLine];
			}
		}
	}
	else
	{
		return GZ_FAILURE;
	}

	return GZ_SUCCESS;
}

int GzPopMatrix(GzRender *render)
{
/*
- pop a matrix off the Ximage stack
- check for stack underflow
*/
	if (NULL == render)
	{
		return GZ_FAILURE;
	}
	if (render->Ximage[0] == NULL) {
		return GZ_FAILURE;
	}
	if (render->matlevel < 0) {
		return GZ_FAILURE;
	}
	render->matlevel--;
	return GZ_SUCCESS;
}


int GzPutAttribute(GzRender	*render, int numAttributes, GzToken	*nameList, 
	GzPointer	*valueList) /* void** valuelist */
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

int GzPutTriangle(GzRender	*render, int numParts, GzToken *nameList, GzPointer	*valueList)
/* numParts : how many names and values */
{
/*  
- pass in a triangle description with tokens and values corresponding to 
      GZ_POSITION:3 vert positions in model space 
- Xform positions of verts using matrix on top of stack 
- Clip - just discard any triangle with any vert(s) behind view plane 
       - optional: test for triangles with all three verts off-screen (trivial frustum cull)
- invoke triangle rasterizer  
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
			GzMatrix topMat;
			int top = render->matlevel;
			memcpy(topMat, render->Ximage[top], sizeof(GzMatrix));
			float aCoordMat[4][4] = { { acGzCoord[0][0], acGzCoord[1][0], acGzCoord[2][0], 0 }, { acGzCoord[0][1], acGzCoord[1][1], acGzCoord[2][1], 0 }, { acGzCoord[0][2], acGzCoord[1][2], acGzCoord[2][2], 0 }, { 1, 1, 1, 0 } };
			float aCoordMatProduct[4][4];
			MatrixProduct(topMat, aCoordMat, aCoordMatProduct);
			if ((aCoordMatProduct[3][0] == 0) || (aCoordMatProduct[3][1] == 0) || (aCoordMatProduct[3][2] == 0))
			{
				continue;
			}
#if 1
			GzCoord asGzCoordX = { aCoordMatProduct[0][0] / aCoordMatProduct[3][0], aCoordMatProduct[0][1] / aCoordMatProduct[3][1], aCoordMatProduct[0][2] / aCoordMatProduct[3][2] };
			GzCoord asGzCoordY = { aCoordMatProduct[1][0] / aCoordMatProduct[3][0], aCoordMatProduct[1][1] / aCoordMatProduct[3][1], aCoordMatProduct[1][2] / aCoordMatProduct[3][2] };
			GzCoord asGzCoordZ = { aCoordMatProduct[2][0] / aCoordMatProduct[3][0], aCoordMatProduct[2][1] / aCoordMatProduct[3][1], aCoordMatProduct[2][2] / aCoordMatProduct[3][2] };
			// clipping part
			if ((asGzCoordX[X] < 0 || asGzCoordX[X] > (render->display->xres)) && (asGzCoordX[Y] < 0 || asGzCoordX[Y] > (render->display->xres)) && (asGzCoordX[Z] < 0 || asGzCoordX[Z] > (render->display->xres)))
			{
				return GZ_FAILURE;
			}

			if ((asGzCoordY[X] < 0 || asGzCoordY[X] > (render->display->yres)) && (asGzCoordY[Y] < 0 || asGzCoordY[Y] > (render->display->yres)) && (asGzCoordY[Z] < 0 || asGzCoordY[Z] > (render->display->yres)))
			{
				return GZ_FAILURE;
			}

			if (asGzCoordZ[X] < 0 && asGzCoordZ[Y] < 0 && asGzCoordZ[Z] < 0)
			{
				return GZ_FAILURE;
			}
#else 

			GzCoord asGzCoordX = { acGzCoord[0][0], acGzCoord[1][0], acGzCoord[2][0] };
			GzCoord asGzCoordY = { acGzCoord[0][1], acGzCoord[1][1], acGzCoord[2][1] };
			GzCoord asGzCoordZ = { acGzCoord[0][2], acGzCoord[1][2], acGzCoord[2][2] };
#endif
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
						if (anZ == 0 || anInterZ < anZ)
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


float dotproduct(float a[3], float b[3])
{
	float product = a[X] * b[X] + a[Y] * b[Y] + a[Z] * b[Z];
	return product;
}

void MatrixProduct(float a[4][4], float b[4][4], float prodTemp[4][4])
{
	memset(prodTemp, 0, 16);
	for (int nCntRow = 0; nCntRow < 4; nCntRow++)
	{
		for (int nCntLine = 0; nCntLine < 4; nCntLine++)
		{
			prodTemp[nCntRow][nCntLine] = a[nCntRow][0] * b[0][nCntLine] + a[nCntRow][1] * b[1][nCntLine] + a[nCntRow][2] * b[2][nCntLine] + a[nCntRow][3] * b[3][nCntLine];
		}
	}
}