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
	render->numlights = 0;
	render->interp_mode = 0;
	render->spec = 0;
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
	GzMatrix	identityMatrix = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	if (render->matlevel == 0)
	{
		for (int nCntRow = 0; nCntRow < 4; nCntRow++)
		{
			for (int nCntLine = 0; nCntLine < 4; nCntLine++)
			{
				render->Ximage[render->matlevel][nCntRow][nCntLine] = matrix[nCntRow][nCntLine];
				render->Xnorm[render->matlevel][nCntRow][nCntLine] = identityMatrix[nCntRow][nCntLine];
			}
		}
	}
	else if (render->matlevel < MATLEVELS)
	{
		float prodTemp[4][4], prodTemp2[4][4];
		GzMatrix matrix2;
		MatrixProduct(render->Ximage[render->matlevel - 1], matrix, prodTemp);
		MatrixProduct(identityMatrix, matrix, matrix2);
		matrix2[0][3] = matrix2[1][3] = matrix2[2][3] = 0;
		GzNormalizeMatrix(matrix2);
		MatrixProduct(render->Xnorm[render->matlevel - 1], matrix2, prodTemp2);
		for (int nCntRow = 0; nCntRow < 4; nCntRow++)
		{
			for (int nCntLine = 0; nCntLine < 4; nCntLine++)
			{
				render->Ximage[render->matlevel][nCntRow][nCntLine] = prodTemp[nCntRow][nCntLine];
				if (render->matlevel == 1)
				{
					render->Xnorm[render->matlevel][nCntRow][nCntLine] = identityMatrix[nCntRow][nCntLine];
				}
				else
				{
					render->Xnorm[render->matlevel][nCntRow][nCntLine] = prodTemp2[nCntRow][nCntLine];
				}
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
		else if (nameList[nCnt] == GZ_DIRECTIONAL_LIGHT)
		{
			GzLight* acLights = (GzLight*)valueList[nCnt];
			render->lights[render->numlights] = *acLights;
			render->numlights++;
		}
		else if (nameList[nCnt] == GZ_AMBIENT_LIGHT)
		{
			GzLight* acAmLight = (GzLight*)valueList[nCnt];
			render->ambientlight = *acAmLight;
		}
		else if (nameList[nCnt] == GZ_DIFFUSE_COEFFICIENT)
		{
			GzColor* acDiffuseColor = (GzColor*)valueList[nCnt];
			render->Kd[RED] = (*acDiffuseColor)[RED];
			render->Kd[GREEN] = (*acDiffuseColor)[GREEN];
			render->Kd[BLUE] = (*acDiffuseColor)[BLUE];
		}
		else if (nameList[nCnt] == GZ_AMBIENT_COEFFICIENT)
		{
			GzColor* acAmbientColor = (GzColor*)valueList[nCnt];
			render->Ka[RED] = (*acAmbientColor)[RED];
			render->Ka[GREEN] = (*acAmbientColor)[GREEN];
			render->Ka[BLUE] = (*acAmbientColor)[BLUE];
		}
		else if (nameList[nCnt] == GZ_SPECULAR_COEFFICIENT)
		{
			GzColor* acSpecularColor = (GzColor*)valueList[nCnt];
			render->Ks[RED] = (*acSpecularColor)[RED];
			render->Ks[GREEN] = (*acSpecularColor)[GREEN];
			render->Ks[BLUE] = (*acSpecularColor)[BLUE];
		}
		else if (nameList[nCnt] == GZ_DISTRIBUTION_COEFFICIENT)
		{
			float* acSpecPower = (float*)valueList[nCnt];
			render->spec = *acSpecPower;
		}
		else if (nameList[nCnt] == GZ_INTERPOLATE)
		{
			int* acSpecPower = (int*)valueList[nCnt];
			render->interp_mode = *acSpecPower;
		}
	}
	return GZ_SUCCESS;
}

int GzPutTriangle1(GzRender	*render, int numParts, GzToken *nameList, GzPointer	*valueList)
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
	for (int nCnt = 0; nCnt <numParts; nCnt++)
	{
		if (nameList[nCnt] == GZ_POSITION)
		{
			GzCoord* acGzCoord;
			acGzCoord = (GzCoord*)(valueList[0]);
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
			GzCoord* acGzCoordNorm;
			acGzCoordNorm = (GzCoord*)(valueList[1]);
			GzCoord acGzEye = { 0, 0, -1 };
			GzCoord afarVertexNorm[3];
			GzColor afarVertexcolor[3];
			afarVertexNorm[0][0] = (render->Xnorm[render->matlevel][0][0] * acGzCoordNorm[0][0] + render->Xnorm[render->matlevel][0][1] * acGzCoordNorm[0][1] + render->Xnorm[render->matlevel][0][2] * acGzCoordNorm[0][2]) + 0 * render->Xnorm[render->matlevel][0][3];
			afarVertexNorm[0][1] = (render->Xnorm[render->matlevel][1][0] * acGzCoordNorm[0][0] + render->Xnorm[render->matlevel][1][1] * acGzCoordNorm[0][1] + render->Xnorm[render->matlevel][1][2] * acGzCoordNorm[0][2]) + 0 * render->Xnorm[render->matlevel][1][3];
			afarVertexNorm[0][2] = (render->Xnorm[render->matlevel][2][0] * acGzCoordNorm[0][0] + render->Xnorm[render->matlevel][2][1] * acGzCoordNorm[0][1] + render->Xnorm[render->matlevel][2][2] * acGzCoordNorm[0][2]) + 0 * render->Xnorm[render->matlevel][2][3];
			//{ render->camera.Xiw[0][0] * acGzCoordNorm[0][0] / render->camera.Xiw[0][4], render->camera.Xiw[0][1] * acGzCoordNorm[0][1] / render->camera.Xiw[0][4], render->camera.Xiw[0][2] * acGzCoordNorm[0][2] / render->camera.Xiw[0][4] };
			GzNormalizeVector(afarVertexNorm[0]);
			afarVertexNorm[1][0] = (render->Xnorm[render->matlevel][0][0] * acGzCoordNorm[1][0] + render->Xnorm[render->matlevel][0][1] * acGzCoordNorm[1][1] + render->Xnorm[render->matlevel][0][2] * acGzCoordNorm[1][2]) + 0 * render->Xnorm[render->matlevel][0][3];
			afarVertexNorm[1][1] = (render->Xnorm[render->matlevel][1][0] * acGzCoordNorm[1][0] + render->Xnorm[render->matlevel][1][1] * acGzCoordNorm[1][1] + render->Xnorm[render->matlevel][1][2] * acGzCoordNorm[1][2]) + 0 * render->Xnorm[render->matlevel][1][3];
			afarVertexNorm[1][2] = (render->Xnorm[render->matlevel][2][0] * acGzCoordNorm[1][0] + render->Xnorm[render->matlevel][2][1] * acGzCoordNorm[1][1] + render->Xnorm[render->matlevel][2][2] * acGzCoordNorm[1][2]) + 0 * render->Xnorm[render->matlevel][2][3];
			//{ render->camera.Xiw[1][0] * acGzCoordNorm[1][0] / render->camera.Xiw[1][4], render->camera.Xiw[1][1] * acGzCoordNorm[1][1] / render->camera.Xiw[1][4], render->camera.Xiw[1][2] * acGzCoordNorm[1][2] / render->camera.Xiw[1][4] };
			GzNormalizeVector(afarVertexNorm[1]);
			afarVertexNorm[2][0] = (render->Xnorm[render->matlevel][0][0] * acGzCoordNorm[2][0] + render->Xnorm[render->matlevel][0][1] * acGzCoordNorm[2][1] + render->Xnorm[render->matlevel][0][2] * acGzCoordNorm[2][2]) + 0 * render->Xnorm[render->matlevel][0][3];
			afarVertexNorm[2][1] = (render->Xnorm[render->matlevel][1][0] * acGzCoordNorm[2][0] + render->Xnorm[render->matlevel][1][1] * acGzCoordNorm[2][1] + render->Xnorm[render->matlevel][1][2] * acGzCoordNorm[2][2]) + 0 * render->Xnorm[render->matlevel][1][3];
			afarVertexNorm[2][2] = (render->Xnorm[render->matlevel][2][0] * acGzCoordNorm[2][0] + render->Xnorm[render->matlevel][2][1] * acGzCoordNorm[2][1] + render->Xnorm[render->matlevel][2][2] * acGzCoordNorm[2][2]) + 0 * render->Xnorm[render->matlevel][2][3];
			//{ render->camera.Xiw[2][0] * acGzCoordNorm[2][0] / render->camera.Xiw[2][4], render->camera.Xiw[2][1] * acGzCoordNorm[2][1] / render->camera.Xiw[2][4], render->camera.Xiw[2][2] * acGzCoordNorm[2][2] / render->camera.Xiw[2][4] };
			GzNormalizeVector(afarVertexNorm[2]);
			if (render->interp_mode == GZ_COLOR)
			{
				//afarVertexcolorA, afarVertexcolorB, afarVertexcolorC;
				GzShadingEquation(render, afarVertexcolor[0], afarVertexNorm[0]);
				GzShadingEquation(render, afarVertexcolor[1], afarVertexNorm[1]);
				GzShadingEquation(render, afarVertexcolor[2], afarVertexNorm[2]);
			}
			//Sort the matrix according to Y
			for (int anSortYCnt = 0; anSortYCnt < 3; anSortYCnt++)
			{
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
						//Swap Color,  Norm and Vertex
						GzCoord afarVertexNormTemp;
						GzColor afarVertexcolorTemp;
						GzCoord afarVertexTemp;
						//may have problems
						for (int anNormCnt = 0; anNormCnt < 3; anNormCnt++)
						{
							afarVertexNormTemp[anNormCnt] = afarVertexNorm[anSortYCnt - 1][anNormCnt];
							afarVertexNorm[anSortYCnt - 1][anNormCnt] = afarVertexNorm[anSortYCnt][anNormCnt];
							afarVertexNorm[anSortYCnt][anNormCnt] = afarVertexNormTemp[anNormCnt];
						}

						for (int anColorCnt = 0; anColorCnt < 3; anColorCnt++)
						{
							afarVertexcolorTemp[anColorCnt] = afarVertexcolor[anSortYCnt - 1][anColorCnt];
							afarVertexcolor[anSortYCnt - 1][anColorCnt] = afarVertexcolor[anSortYCnt][anColorCnt];
							afarVertexcolor[anSortYCnt][anColorCnt] = afarVertexcolorTemp[anColorCnt];
						}
					}
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
					//Swap Color,  Norm and Vertex
					GzCoord afarVertexNormTemp;
					GzColor afarVertexcolorTemp;
					GzCoord afarVertexTemp;
					//may have problems
					for (int anNormCnt = 0; anNormCnt < 3; anNormCnt++)
					{
						afarVertexNormTemp[anNormCnt] = afarVertexNorm[anSortYCnt - 1][anNormCnt];
						afarVertexNorm[anSortYCnt - 1][anNormCnt] = afarVertexNorm[anSortYCnt][anNormCnt];
						afarVertexNorm[anSortYCnt][anNormCnt] = afarVertexNormTemp[anNormCnt];
					}

					for (int anColorCnt = 0; anColorCnt < 3; anColorCnt++)
					{
						afarVertexcolorTemp[anColorCnt] = afarVertexcolor[anSortYCnt - 1][anColorCnt];
						afarVertexcolor[anSortYCnt - 1][anColorCnt] = afarVertexcolor[anSortYCnt][anColorCnt];
						afarVertexcolor[anSortYCnt][anColorCnt] = afarVertexcolorTemp[anColorCnt];
					}

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
				//Swap Color,  Norm and Vertex
				GzCoord afarVertexNormTemp;
				GzColor afarVertexcolorTemp;
				GzCoord afarVertexTemp;
				//may have problems
				for (int anNormCnt = 0; anNormCnt < 3; anNormCnt++)
				{
					afarVertexNormTemp[anNormCnt] = afarVertexNorm[2][anNormCnt];
					afarVertexNorm[2][anNormCnt] = afarVertexNorm[1][anNormCnt];
					afarVertexNorm[1][anNormCnt] = afarVertexNormTemp[anNormCnt];
				}

				for (int anColorCnt = 0; anColorCnt < 3; anColorCnt++)
				{
					afarVertexcolorTemp[anColorCnt] = afarVertexcolor[2][anColorCnt];
					afarVertexcolor[2][anColorCnt] = afarVertexcolor[1][anColorCnt];
					afarVertexcolor[1][anColorCnt] = afarVertexcolorTemp[anColorCnt];
				}

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
			float afCoVariantRA, afCoVariantRB, afCoVariantRC, afCoVariantRD;	//	RGB is not the same
			float afCoVariantGA, afCoVariantGB, afCoVariantGC, afCoVariantGD;	//	
			float afCoVariantBA, afCoVariantBB, afCoVariantBC, afCoVariantBD;	//	
			GzCoord afarVertex[3];
			for (int anCnt = 0; anCnt < 3; anCnt++)
			{
				afarVertex[anCnt][X] = asGzCoordX[anCnt];
				afarVertex[anCnt][Y] = asGzCoordY[anCnt];
			}
			if (render->interp_mode == GZ_COLOR)
			{
				//R
				afCoVariantRA = ((afarVertexcolor[2][0] - afarVertexcolor[0][0])*(afarVertex[1][Y] - afarVertex[0][Y]) - (afarVertexcolor[1][0] - afarVertexcolor[0][0])*(afarVertex[2][Y] - afarVertex[0][Y]));
				afCoVariantRB = ((afarVertexcolor[1][0] - afarVertexcolor[0][0])*(afarVertex[2][X] - afarVertex[0][X]) - (afarVertexcolor[2][0] - afarVertexcolor[0][0])*(afarVertex[1][X] - afarVertex[0][X]));
				afCoVariantRC = ((afarVertex[1][X] - afarVertex[0][X])*(afarVertex[2][Y] - afarVertex[0][Y]) - (afarVertex[1][Y] - afarVertex[0][Y])*(afarVertex[2][X] - afarVertex[0][X]));
				afCoVariantRD = -(afCoVariantRB*afarVertex[0][Y] + afCoVariantRA*afarVertex[0][X] +afCoVariantRC*afarVertexcolor[0][0]);

				//G
				afCoVariantGA = ((afarVertexcolor[2][1] - afarVertexcolor[0][1])*(afarVertex[1][Y] - afarVertex[0][Y]) - (afarVertexcolor[1][1] - afarVertexcolor[0][1])*(afarVertex[2][Y] - afarVertex[0][Y]));
				afCoVariantGB = ((afarVertexcolor[1][1] - afarVertexcolor[0][1])*(afarVertex[2][X] - afarVertex[0][X]) - (afarVertexcolor[2][1] - afarVertexcolor[0][1])*(afarVertex[1][X] - afarVertex[0][X]));
				afCoVariantGC = ((afarVertex[1][X] - afarVertex[0][X])*(afarVertex[2][Y] - afarVertex[0][Y]) - (afarVertex[1][Y] - afarVertex[0][Y])*(afarVertex[2][X] - afarVertex[0][X]));
				afCoVariantGD = -(afCoVariantGA*afarVertex[0][X] + afCoVariantGB*afarVertex[0][Y] + afCoVariantGC*afarVertexcolor[0][1]);

				//B
				afCoVariantBA = ((afarVertexcolor[2][2] - afarVertexcolor[0][2])*(afarVertex[1][Y] - afarVertex[0][Y]) - (afarVertexcolor[1][2] - afarVertexcolor[0][2])*(afarVertex[2][Y] - afarVertex[0][Y]));
				afCoVariantBB = ((afarVertexcolor[1][2] - afarVertexcolor[0][2])*(afarVertex[2][X] - afarVertex[0][X]) - (afarVertexcolor[2][2] - afarVertexcolor[0][2])*(afarVertex[1][X] - afarVertex[0][X]));
				afCoVariantBC = ((afarVertex[1][X] - afarVertex[0][X])*(afarVertex[2][Y] - afarVertex[0][Y]) - (afarVertex[1][Y] - afarVertex[0][Y])*(afarVertex[2][X] - afarVertex[0][X]));
				afCoVariantBD = -(afCoVariantBA*afarVertex[0][X] + afCoVariantBB*afarVertex[0][Y] + afCoVariantBC*afarVertexcolor[0][2]);
			}
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
							int anCoefficientR, anCoefficientG, anCoefficientB;
							if (render->interp_mode == GZ_FLAT)
							{
								anCoefficientR = ctoi(render->flatcolor[RED]);
								anCoefficientG = ctoi(render->flatcolor[GREEN]);
								anCoefficientB = ctoi(render->flatcolor[BLUE]);
							}
							else if (render->interp_mode == GZ_COLOR)
							{

								//R=-((x+by+d)/c)
								float temp = -((afCoVariantRA*anCntRangeX + afCoVariantRB * anCntRangeY + afCoVariantRD) / afCoVariantRC);
								temp = VerifyColor(temp);
								anCoefficientR = ctoi(temp);
								//anCoefficientR = anCoefficientR * 255 / 4095;
								//int test = ctoi(anCoefficientR);
								//G=-((x+by+d)/c)
								float temp1 = -((afCoVariantGA*anCntRangeX + afCoVariantGB * anCntRangeY + afCoVariantGD) / afCoVariantGC);
								temp1 = VerifyColor(temp1);
								anCoefficientG = ctoi(temp1);
								//B=-((x+by+d)/c)
								float temp2 = -((afCoVariantBA*anCntRangeX + afCoVariantBB * anCntRangeY + afCoVariantBD) / afCoVariantBC);
								temp2 = VerifyColor(temp2);
								anCoefficientB = ctoi(temp2);

							}
							else if (render->interp_mode == GZ_NORMALS)
							{
								//node need to calculate from outside the for loop
								//Change color to norm
								//R for X
								afCoVariantRA = ((afarVertexNorm[2][0] - afarVertexNorm[0][0])*(afarVertex[1][Y] - afarVertex[0][Y]) - (afarVertexNorm[1][0] - afarVertexNorm[0][0])*(afarVertex[2][Y] - afarVertex[0][Y]));
								afCoVariantRB = ((afarVertexNorm[1][0] - afarVertexNorm[0][0])*(afarVertex[2][X] - afarVertex[0][X]) - (afarVertexNorm[2][0] - afarVertexNorm[0][0])*(afarVertex[1][X] - afarVertex[0][X]));
								afCoVariantRC = ((afarVertex[1][X] - afarVertex[0][X])*(afarVertex[2][Y] - afarVertex[0][Y]) - (afarVertex[1][Y] - afarVertex[0][Y])*(afarVertex[2][X] - afarVertex[0][X]));
								afCoVariantRD = -(afCoVariantRB*afarVertex[0][Y] + afCoVariantRA*afarVertex[0][X] + afCoVariantRC*afarVertexNorm[0][0]);

								//G for Y
								afCoVariantGA = ((afarVertexNorm[2][1] - afarVertexNorm[0][1])*(afarVertex[1][Y] - afarVertex[0][Y]) - (afarVertexNorm[1][1] - afarVertexNorm[0][1])*(afarVertex[2][Y] - afarVertex[0][Y]));
								afCoVariantGB = ((afarVertexNorm[1][1] - afarVertexNorm[0][1])*(afarVertex[2][X] - afarVertex[0][X]) - (afarVertexNorm[2][1] - afarVertexNorm[0][1])*(afarVertex[1][X] - afarVertex[0][X]));
								afCoVariantGC = ((afarVertex[1][X] - afarVertex[0][X])*(afarVertex[2][Y] - afarVertex[0][Y]) - (afarVertex[1][Y] - afarVertex[0][Y])*(afarVertex[2][X] - afarVertex[0][X]));
								afCoVariantGD = -(afCoVariantGA*afarVertex[0][X] + afCoVariantGB*afarVertex[0][Y] + afCoVariantGC*afarVertexNorm[0][1]);

								//B for Z
								afCoVariantBA = ((afarVertexNorm[2][2] - afarVertexNorm[0][2])*(afarVertex[1][Y] - afarVertex[0][Y]) - (afarVertexNorm[1][2] - afarVertexNorm[0][2])*(afarVertex[2][Y] - afarVertex[0][Y]));
								afCoVariantBB = ((afarVertexNorm[1][2] - afarVertexNorm[0][2])*(afarVertex[2][X] - afarVertex[0][X]) - (afarVertexNorm[2][2] - afarVertexNorm[0][2])*(afarVertex[1][X] - afarVertex[0][X]));
								afCoVariantBC = ((afarVertex[1][X] - afarVertex[0][X])*(afarVertex[2][Y] - afarVertex[0][Y]) - (afarVertex[1][Y] - afarVertex[0][Y])*(afarVertex[2][X] - afarVertex[0][X]));
								afCoVariantBD = -(afCoVariantBA*afarVertex[0][X] + afCoVariantBB*afarVertex[0][Y] + afCoVariantBC*afarVertexNorm[0][2]);

								float temp = -((afCoVariantRA*anCntRangeX + afCoVariantRB * anCntRangeY + afCoVariantRD) / afCoVariantRC);//norm X
								float temp1 = -((afCoVariantGA*anCntRangeX + afCoVariantGB * anCntRangeY + afCoVariantGD) / afCoVariantGC);//norm Y
								float temp2 = -((afCoVariantBA*anCntRangeX + afCoVariantBB * anCntRangeY + afCoVariantBD) / afCoVariantBC);//norm Z
								GzCoord asPointNorm = { temp , temp1, temp2};
								GzColor asPintColor;
								GzShadingEquation(render, asPintColor, asPointNorm);

								anCoefficientR = ctoi(VerifyColor(asPintColor[0]));
								anCoefficientG = ctoi(VerifyColor(asPintColor[1]));
								anCoefficientB = ctoi(VerifyColor(asPintColor[2]));
							}
							if (GZ_FAILURE == GzPutDisplay(render->display, anCntRangeX, anCntRangeY, anCoefficientR, anCoefficientG, anCoefficientB, asAlpha, anInterZ))
							{
								return GZ_FAILURE;
							}
						}
					}
				}
			}
		}
		else if (nameList[nCnt] == GZ_NORMAL)
		{
			

			/*float afDiffuseLightX, afDiffuseLightY, afDiffuseLightZ, afAmbientLightX, afAmbientLightY, afAmbientLightZ;
			afAmbientLightX = afAmbientLightY = afAmbientLightZ = afDiffuseLightX = afDiffuseLightY = afDiffuseLightZ = 0;
			//How can I get E? It is acGzEye
			//To ensure NL and NE is the same positive or nagetive
			for (int anLights = 0; anLights < render->numlights; anLights)
			{
				float afResANL = render->lights[anLights].direction[X] * afarVertexNormalA[0] + render->lights[anLights].direction[Y] * afarVertexNormalA[1] + render->lights[anLights].direction[Z] * afarVertexNormalA[2];
				float afResANE = render->camera.lookat[0] * afarVertexNormalA[0] + render->camera.lookat[1] * afarVertexNormalA[1] + render->camera.lookat[2] * afarVertexNormalA[2];
				if (afResANL*afResANE < 0)
				{
					continue;
				}
				else if ((afResANL < 0) && (afResANE < 0))
				{
				
				}
				afDiffuseLightX += render->Kd[X] * render->lights[anLights].color[X] * (render->lights[anLights].direction[X] * afVar1 + render->lights[anLights].direction[Y] * afVar2 + render->lights[anLights].direction[Z] * afVar3);
				afDiffuseLightY += render->Kd[Y] * render->lights[anLights].color[Y] * (render->lights[anLights].direction[X] * afVar1 + render->lights[anLights].direction[Y] * afVar2 + render->lights[anLights].direction[Z] * afVar3);
				afDiffuseLightZ += render->Kd[Z] * render->lights[anLights].color[Z] * (render->lights[anLights].direction[X] * afVar1 + render->lights[anLights].direction[Y] * afVar2 + render->lights[anLights].direction[Z] * afVar3);
				GzCoord avR;
				avR[X] = (render->lights[anLights].direction[X] * afVar1 + render->lights[anLights].direction[Y] * afVar2 + render->lights[anLights].direction[Z] * afVar3) * 2 * afVar1 - render->lights[anLights].direction[X];
				avR[Y] = (render->lights[anLights].direction[X] * afVar1 + render->lights[anLights].direction[Y] * afVar2 + render->lights[anLights].direction[Z] * afVar3) * 2 * afVar2 - render->lights[anLights].direction[Y];
				avR[X] = (render->lights[anLights].direction[X] * afVar1 + render->lights[anLights].direction[Y] * afVar2 + render->lights[anLights].direction[Z] * afVar3) * 2 * afVar3 - render->lights[anLights].direction[Z];
			}
			afAmbientLightX = render->Ka[0] * render->ambientlight.color[0]; afAmbientLightY = render->Ka[1] * render->ambientlight.color[1]; afAmbientLightZ = render->Ka[2] * render->ambientlight.color[2];*/
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

void GzShadingEquation(GzRender *render, GzColor color, GzCoord norm) 
{
	float afLength = sqrt(norm[X] * norm[X] + norm[Y] * norm[Y] + norm[Z] * norm[Z]);
	norm[X] = norm[X] / afLength;norm[Y] = norm[Y] / afLength;norm[Z] = norm[Z] / afLength;
	GzCoord Eye = {0, 0, -1};
	GzCoord* light = new GzCoord[render->numlights];
	int* flag = new int[render->numlights];
	//calculate the direction to dicide the norm direction
	float afDotE = norm[X] * Eye[X] + norm[Y] * Eye[Y] + norm[Z] * Eye[Z];
	float afDotL;
	for (int anCnt = 0; anCnt < render->numlights; anCnt++) 
	{
		float afDotL = render->lights[anCnt].direction[X] * norm[X] + render->lights[anCnt].direction[Y] * norm[Y] + render->lights[anCnt].direction[Z] * norm[Z];
		if (afDotL >= 0 && afDotE >= 0) 
		{
			light[anCnt][X] = 2 * afDotL*norm[X] - render->lights[anCnt].direction[X];
			light[anCnt][Y] = 2 * afDotL*norm[Y] - render->lights[anCnt].direction[Y];
			light[anCnt][Z] = 2 * afDotL*norm[Z] - render->lights[anCnt].direction[Z];
			afLength = sqrt(light[anCnt][X] * light[anCnt][X] + light[anCnt][Y] * light[anCnt][Y] + light[anCnt][Z] * light[anCnt][Z]);
			light[anCnt][X] = light[anCnt][X] / afLength;
			light[anCnt][Y] = light[anCnt][Y] / afLength;
			light[anCnt][Z] = light[anCnt][Z] / afLength;
			flag[anCnt] = 1;
		}
		else if (afDotL < 0 && afDotE < 0) 
		{
			light[anCnt][X] = 2 * afDotL*(-norm[X]) - render->lights[anCnt].direction[X];
			light[anCnt][Y] = 2 * afDotL*(-norm[Y]) - render->lights[anCnt].direction[Y];
			light[anCnt][Z] = 2 * afDotL*(-norm[Z]) - render->lights[anCnt].direction[Z];
			afLength = sqrt(light[anCnt][X] * light[anCnt][X] + light[anCnt][Y] * light[anCnt][Y] + light[anCnt][Z] * light[anCnt][Z]);
			light[anCnt][X] = light[anCnt][X] / afLength;
			light[anCnt][Y] = light[anCnt][Y] / afLength;
			light[anCnt][Z] = light[anCnt][Z] / afLength;
			flag[anCnt] = -1;
		}
		else {
			flag[anCnt] = 0;
			continue;
		}
	}
	GzColor specLightSum = { 0, 0, 0 };
	for (int anCnt = 0; anCnt < render->numlights; anCnt++) {
		if (flag[anCnt] == 0)
		{
			continue;
		}
		float afdotR = light[anCnt][X] * Eye[X] + light[anCnt][Y] * Eye[Y] + light[anCnt][Z] * Eye[Z];
		if (afdotR < 0)
		{
			afdotR = 0;
		}
		if (afdotR > 1)
		{
			afdotR = 1;
		}
		specLightSum[0] += render->lights[anCnt].color[0] * pow(afdotR, render->spec);
		specLightSum[1] += render->lights[anCnt].color[1] * pow(afdotR, render->spec);
		specLightSum[2] += render->lights[anCnt].color[2] * pow(afdotR, render->spec);
	}
	GzColor diffLightSum = { 0, 0, 0 };
	for (int anCnt = 0; anCnt < render->numlights; anCnt++)
	{
		if (flag[anCnt] == 0) continue;
		if (flag[anCnt] == 1) {
			float afDiffCoVariant = render->lights[anCnt].direction[X] * norm[X] + render->lights[anCnt].direction[Y] * norm[Y] + render->lights[anCnt].direction[Z] * norm[Z];
			diffLightSum[0] += render->lights[anCnt].color[0] * afDiffCoVariant;
			diffLightSum[1] += render->lights[anCnt].color[1] * afDiffCoVariant;
			diffLightSum[2] += render->lights[anCnt].color[2] * afDiffCoVariant;
		}
		else if (flag[anCnt] == -1) 
		{
			GzCoord negN = { -norm[X], -norm[Y], -norm[Z] };
			float afDiffCoVariant = render->lights[anCnt].direction[X] * negN[X] + render->lights[anCnt].direction[Y] * negN[Y] + render->lights[anCnt].direction[Z] * negN[Z];
			diffLightSum[0] += render->lights[anCnt].color[0] * afDiffCoVariant;
			diffLightSum[1] += render->lights[anCnt].color[1] * afDiffCoVariant;
			diffLightSum[2] += render->lights[anCnt].color[2] * afDiffCoVariant;
		}
	}
	GzColor specCoVariant, diffCoVariant, ambCoVariant;
	specCoVariant[0] = render->Ks[0] * specLightSum[0];
	specCoVariant[1] = render->Ks[1] * specLightSum[1];
	specCoVariant[2] = render->Ks[2] * specLightSum[2];
	diffCoVariant[0] = render->Kd[0] * diffLightSum[0];
	diffCoVariant[1] = render->Kd[1] * diffLightSum[1];
	diffCoVariant[2] = render->Kd[2] * diffLightSum[2];
	ambCoVariant[0] = render->Ka[0] * render->ambientlight.color[0];
	ambCoVariant[1] = render->Ka[1] * render->ambientlight.color[1];
	ambCoVariant[2] = render->Ka[2] * render->ambientlight.color[2];
	color[0] = specCoVariant[0] + diffCoVariant[0] + ambCoVariant[0];
	color[1] = specCoVariant[1] + diffCoVariant[1] + ambCoVariant[1];
	color[2] = specCoVariant[2] + diffCoVariant[2] + ambCoVariant[2];
}

int GzNormalizeVector(GzCoord vec) 
{
	if (vec == NULL) {
		return GZ_FAILURE;
	}

	float norm = sqrt((vec[X] * vec[X]) + (vec[Y] * vec[Y]) + (vec[Z] * vec[Z]));

	vec[X] = vec[X] / norm;
	vec[Y] = vec[Y] / norm;
	vec[Z] = vec[Z] / norm;

	return GZ_SUCCESS;
}

int GzNormalizeMatrix(GzMatrix mat)
{
	if (mat == NULL) {
		return GZ_FAILURE;
	}

	for (int anLine = 0; anLine < 4; anLine++)
	{
		float length = sqrt((mat[anLine][0] * mat[anLine][0]) + (mat[anLine][1] * mat[anLine][1]) + (mat[anLine][2] * mat[anLine][2]) + (mat[anLine][3] * mat[anLine][3]));
		for (int anRow = 0; anRow < 4; anRow++)
		{
			mat[anLine][anRow] = mat[anLine][anRow] / length;
		}
	}
	return GZ_SUCCESS;
}

float VerifyColor(float temp)
{
	if (temp > 1)
	{
		temp = 1;
	}
	else if (temp < 0)
	{
		temp = 0;
	}
	return temp;
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
	for (int nCnt = 0; nCnt <numParts; nCnt++)
	{
		if (nameList[nCnt] == GZ_POSITION)
		{
			GzCoord* acGzCoord;
			acGzCoord = (GzCoord*)(valueList[0]);
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
			GzCoord* acGzCoordNorm;
			acGzCoordNorm = (GzCoord*)(valueList[1]);
			GzCoord acGzEye = { 0, 0, -1 };
			GzCoord afarVertexNorm[3];
			GzColor afarVertexcolor[3];
			afarVertexNorm[0][0] = (render->Xnorm[render->matlevel][0][0] * acGzCoordNorm[0][0] + render->Xnorm[render->matlevel][0][1] * acGzCoordNorm[0][1] + render->Xnorm[render->matlevel][0][2] * acGzCoordNorm[0][2]) + 0 * render->Xnorm[render->matlevel][0][3];
			afarVertexNorm[0][1] = (render->Xnorm[render->matlevel][1][0] * acGzCoordNorm[0][0] + render->Xnorm[render->matlevel][1][1] * acGzCoordNorm[0][1] + render->Xnorm[render->matlevel][1][2] * acGzCoordNorm[0][2]) + 0 * render->Xnorm[render->matlevel][1][3];
			afarVertexNorm[0][2] = (render->Xnorm[render->matlevel][2][0] * acGzCoordNorm[0][0] + render->Xnorm[render->matlevel][2][1] * acGzCoordNorm[0][1] + render->Xnorm[render->matlevel][2][2] * acGzCoordNorm[0][2]) + 0 * render->Xnorm[render->matlevel][2][3];
			//{ render->camera.Xiw[0][0] * acGzCoordNorm[0][0] / render->camera.Xiw[0][4], render->camera.Xiw[0][1] * acGzCoordNorm[0][1] / render->camera.Xiw[0][4], render->camera.Xiw[0][2] * acGzCoordNorm[0][2] / render->camera.Xiw[0][4] };
			GzNormalizeVector(afarVertexNorm[0]);
			afarVertexNorm[1][0] = (render->Xnorm[render->matlevel][0][0] * acGzCoordNorm[1][0] + render->Xnorm[render->matlevel][0][1] * acGzCoordNorm[1][1] + render->Xnorm[render->matlevel][0][2] * acGzCoordNorm[1][2]) + 0 * render->Xnorm[render->matlevel][0][3];
			afarVertexNorm[1][1] = (render->Xnorm[render->matlevel][1][0] * acGzCoordNorm[1][0] + render->Xnorm[render->matlevel][1][1] * acGzCoordNorm[1][1] + render->Xnorm[render->matlevel][1][2] * acGzCoordNorm[1][2]) + 0 * render->Xnorm[render->matlevel][1][3];
			afarVertexNorm[1][2] = (render->Xnorm[render->matlevel][2][0] * acGzCoordNorm[1][0] + render->Xnorm[render->matlevel][2][1] * acGzCoordNorm[1][1] + render->Xnorm[render->matlevel][2][2] * acGzCoordNorm[1][2]) + 0 * render->Xnorm[render->matlevel][2][3];
			//{ render->camera.Xiw[1][0] * acGzCoordNorm[1][0] / render->camera.Xiw[1][4], render->camera.Xiw[1][1] * acGzCoordNorm[1][1] / render->camera.Xiw[1][4], render->camera.Xiw[1][2] * acGzCoordNorm[1][2] / render->camera.Xiw[1][4] };
			GzNormalizeVector(afarVertexNorm[1]);
			afarVertexNorm[2][0] = (render->Xnorm[render->matlevel][0][0] * acGzCoordNorm[2][0] + render->Xnorm[render->matlevel][0][1] * acGzCoordNorm[2][1] + render->Xnorm[render->matlevel][0][2] * acGzCoordNorm[2][2]) + 0 * render->Xnorm[render->matlevel][0][3];
			afarVertexNorm[2][1] = (render->Xnorm[render->matlevel][1][0] * acGzCoordNorm[2][0] + render->Xnorm[render->matlevel][1][1] * acGzCoordNorm[2][1] + render->Xnorm[render->matlevel][1][2] * acGzCoordNorm[2][2]) + 0 * render->Xnorm[render->matlevel][1][3];
			afarVertexNorm[2][2] = (render->Xnorm[render->matlevel][2][0] * acGzCoordNorm[2][0] + render->Xnorm[render->matlevel][2][1] * acGzCoordNorm[2][1] + render->Xnorm[render->matlevel][2][2] * acGzCoordNorm[2][2]) + 0 * render->Xnorm[render->matlevel][2][3];
			//{ render->camera.Xiw[2][0] * acGzCoordNorm[2][0] / render->camera.Xiw[2][4], render->camera.Xiw[2][1] * acGzCoordNorm[2][1] / render->camera.Xiw[2][4], render->camera.Xiw[2][2] * acGzCoordNorm[2][2] / render->camera.Xiw[2][4] };
			GzNormalizeVector(afarVertexNorm[2]);
			if (render->interp_mode == GZ_COLOR)
			{
				//afarVertexcolorA, afarVertexcolorB, afarVertexcolorC;
				GzShadingEquation(render, afarVertexcolor[0], afarVertexNorm[0]);
				GzShadingEquation(render, afarVertexcolor[1], afarVertexNorm[1]);
				GzShadingEquation(render, afarVertexcolor[2], afarVertexNorm[2]);
			}
			//Sort the matrix according to Y
			for (int anSortYCnt = 0; anSortYCnt < 3; anSortYCnt++)
			{
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
						//Swap Color,  Norm and Vertex
						GzCoord afarVertexNormTemp;
						GzColor afarVertexcolorTemp;
						GzCoord afarVertexTemp;
						//may have problems
						for (int anNormCnt = 0; anNormCnt < 3; anNormCnt++)
						{
							afarVertexNormTemp[anNormCnt] = afarVertexNorm[anSortYCnt - 1][anNormCnt];
							afarVertexNorm[anSortYCnt - 1][anNormCnt] = afarVertexNorm[anSortYCnt][anNormCnt];
							afarVertexNorm[anSortYCnt][anNormCnt] = afarVertexNormTemp[anNormCnt];
						}

						for (int anColorCnt = 0; anColorCnt < 3; anColorCnt++)
						{
							afarVertexcolorTemp[anColorCnt] = afarVertexcolor[anSortYCnt - 1][anColorCnt];
							afarVertexcolor[anSortYCnt - 1][anColorCnt] = afarVertexcolor[anSortYCnt][anColorCnt];
							afarVertexcolor[anSortYCnt][anColorCnt] = afarVertexcolorTemp[anColorCnt];
						}
					}
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
					//Swap Color,  Norm and Vertex
					GzCoord afarVertexNormTemp;
					GzColor afarVertexcolorTemp;
					GzCoord afarVertexTemp;
					//may have problems
					for (int anNormCnt = 0; anNormCnt < 3; anNormCnt++)
					{
						afarVertexNormTemp[anNormCnt] = afarVertexNorm[anSortYCnt - 1][anNormCnt];
						afarVertexNorm[anSortYCnt - 1][anNormCnt] = afarVertexNorm[anSortYCnt][anNormCnt];
						afarVertexNorm[anSortYCnt][anNormCnt] = afarVertexNormTemp[anNormCnt];
					}

					for (int anColorCnt = 0; anColorCnt < 3; anColorCnt++)
					{
						afarVertexcolorTemp[anColorCnt] = afarVertexcolor[anSortYCnt - 1][anColorCnt];
						afarVertexcolor[anSortYCnt - 1][anColorCnt] = afarVertexcolor[anSortYCnt][anColorCnt];
						afarVertexcolor[anSortYCnt][anColorCnt] = afarVertexcolorTemp[anColorCnt];
					}

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
				//Swap Color,  Norm and Vertex
				GzCoord afarVertexNormTemp;
				GzColor afarVertexcolorTemp;
				GzCoord afarVertexTemp;
				//may have problems
				for (int anNormCnt = 0; anNormCnt < 3; anNormCnt++)
				{
					afarVertexNormTemp[anNormCnt] = afarVertexNorm[2][anNormCnt];
					afarVertexNorm[2][anNormCnt] = afarVertexNorm[1][anNormCnt];
					afarVertexNorm[1][anNormCnt] = afarVertexNormTemp[anNormCnt];
				}

				for (int anColorCnt = 0; anColorCnt < 3; anColorCnt++)
				{
					afarVertexcolorTemp[anColorCnt] = afarVertexcolor[2][anColorCnt];
					afarVertexcolor[2][anColorCnt] = afarVertexcolor[1][anColorCnt];
					afarVertexcolor[1][anColorCnt] = afarVertexcolorTemp[anColorCnt];
				}

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
			float afCoVariantRA, afCoVariantRB, afCoVariantRC, afCoVariantRD;	//	RGB is not the same
			float afCoVariantGA, afCoVariantGB, afCoVariantGC, afCoVariantGD;	//	
			float afCoVariantBA, afCoVariantBB, afCoVariantBC, afCoVariantBD;	//	
			GzCoord afarVertex[3];
			for (int anCnt = 0; anCnt < 3; anCnt++)
			{
				afarVertex[anCnt][X] = asGzCoordX[anCnt];
				afarVertex[anCnt][Y] = asGzCoordY[anCnt];
			}
			if (render->interp_mode == GZ_COLOR)
			{
				//R
				afCoVariantRA = ((afarVertexcolor[2][0] - afarVertexcolor[0][0])*(afarVertex[1][Y] - afarVertex[0][Y]) - (afarVertexcolor[1][0] - afarVertexcolor[0][0])*(afarVertex[2][Y] - afarVertex[0][Y]));
				afCoVariantRB = ((afarVertexcolor[1][0] - afarVertexcolor[0][0])*(afarVertex[2][X] - afarVertex[0][X]) - (afarVertexcolor[2][0] - afarVertexcolor[0][0])*(afarVertex[1][X] - afarVertex[0][X]));
				afCoVariantRC = ((afarVertex[1][X] - afarVertex[0][X])*(afarVertex[2][Y] - afarVertex[0][Y]) - (afarVertex[1][Y] - afarVertex[0][Y])*(afarVertex[2][X] - afarVertex[0][X]));
				afCoVariantRD = -(afCoVariantRB*afarVertex[0][Y] + afCoVariantRA*afarVertex[0][X] + afCoVariantRC*afarVertexcolor[0][0]);

				//G
				afCoVariantGA = ((afarVertexcolor[2][1] - afarVertexcolor[0][1])*(afarVertex[1][Y] - afarVertex[0][Y]) - (afarVertexcolor[1][1] - afarVertexcolor[0][1])*(afarVertex[2][Y] - afarVertex[0][Y]));
				afCoVariantGB = ((afarVertexcolor[1][1] - afarVertexcolor[0][1])*(afarVertex[2][X] - afarVertex[0][X]) - (afarVertexcolor[2][1] - afarVertexcolor[0][1])*(afarVertex[1][X] - afarVertex[0][X]));
				afCoVariantGC = ((afarVertex[1][X] - afarVertex[0][X])*(afarVertex[2][Y] - afarVertex[0][Y]) - (afarVertex[1][Y] - afarVertex[0][Y])*(afarVertex[2][X] - afarVertex[0][X]));
				afCoVariantGD = -(afCoVariantGA*afarVertex[0][X] + afCoVariantGB*afarVertex[0][Y] + afCoVariantGC*afarVertexcolor[0][1]);

				//B
				afCoVariantBA = ((afarVertexcolor[2][2] - afarVertexcolor[0][2])*(afarVertex[1][Y] - afarVertex[0][Y]) - (afarVertexcolor[1][2] - afarVertexcolor[0][2])*(afarVertex[2][Y] - afarVertex[0][Y]));
				afCoVariantBB = ((afarVertexcolor[1][2] - afarVertexcolor[0][2])*(afarVertex[2][X] - afarVertex[0][X]) - (afarVertexcolor[2][2] - afarVertexcolor[0][2])*(afarVertex[1][X] - afarVertex[0][X]));
				afCoVariantBC = ((afarVertex[1][X] - afarVertex[0][X])*(afarVertex[2][Y] - afarVertex[0][Y]) - (afarVertex[1][Y] - afarVertex[0][Y])*(afarVertex[2][X] - afarVertex[0][X]));
				afCoVariantBD = -(afCoVariantBA*afarVertex[0][X] + afCoVariantBB*afarVertex[0][Y] + afCoVariantBC*afarVertexcolor[0][2]);
			}
			for (int anCntRangeX = int(afMinX); (float)anCntRangeX < (afMaxX); anCntRangeX++){
				for (int anCntRangeY = int(afMinY); (float)anCntRangeY < (afMaxY); anCntRangeY++){
					float anCntRangeXTemp = (float)anCntRangeX - render->Xoffset;
					float anCntRangeYTemp = (float)anCntRangeY - render->Yoffset;
					AfDiff1 = (afCoefA1*anCntRangeXTemp) + (afCoefB1*anCntRangeYTemp) + afCoefC1;
					AfDiff2 = (afCoefA2*anCntRangeXTemp) + (afCoefB2*anCntRangeYTemp) + afCoefC2;
					AfDiff3 = (afCoefA3*anCntRangeXTemp) + (afCoefB3*anCntRangeYTemp) + afCoefC3;
					anInterZ = -(afVar1*anCntRangeXTemp + afVar2*anCntRangeYTemp + afVar4) / afVar3;
					if (!(AfDiff1 < 0 || AfDiff2 < 0 || AfDiff3 < 0)){
						asR = 0;
						asG = 0;
						asB = 0;
						asAlpha = 0;
						anZ = 0;
						if (GZ_FAILURE == GzGetDisplay(render->display, anCntRangeXTemp, anCntRangeYTemp, &asR, &asG, &asB, &asAlpha, &anZ))
						{
							return GZ_FAILURE;
						}
						if (anZ == 0 || anInterZ < anZ)
						{
							int anCoefficientR, anCoefficientG, anCoefficientB;
							if (render->interp_mode == GZ_FLAT)
							{
								anCoefficientR = ctoi(render->flatcolor[RED]);
								anCoefficientG = ctoi(render->flatcolor[GREEN]);
								anCoefficientB = ctoi(render->flatcolor[BLUE]);
							}
							else if (render->interp_mode == GZ_COLOR)
							{

								//R=-((x+by+d)/c)
								float temp = -((afCoVariantRA*anCntRangeXTemp + afCoVariantRB * anCntRangeYTemp + afCoVariantRD) / afCoVariantRC);
								temp = VerifyColor(temp);
								anCoefficientR = ctoi(temp);
								//anCoefficientR = anCoefficientR * 255 / 4095;
								//int test = ctoi(anCoefficientR);
								//G=-((x+by+d)/c)
								float temp1 = -((afCoVariantGA*anCntRangeXTemp + afCoVariantGB * anCntRangeYTemp + afCoVariantGD) / afCoVariantGC);
								temp1 = VerifyColor(temp1);
								anCoefficientG = ctoi(temp1);
								//B=-((x+by+d)/c)
								float temp2 = -((afCoVariantBA*anCntRangeXTemp + afCoVariantBB * anCntRangeYTemp + afCoVariantBD) / afCoVariantBC);
								temp2 = VerifyColor(temp2);
								anCoefficientB = ctoi(temp2);

							}
							else if (render->interp_mode == GZ_NORMALS)
							{
								//node need to calculate from outside the for loop
								//Change color to norm
								//R for X
								afCoVariantRA = ((afarVertexNorm[2][0] - afarVertexNorm[0][0])*(afarVertex[1][Y] - afarVertex[0][Y]) - (afarVertexNorm[1][0] - afarVertexNorm[0][0])*(afarVertex[2][Y] - afarVertex[0][Y]));
								afCoVariantRB = ((afarVertexNorm[1][0] - afarVertexNorm[0][0])*(afarVertex[2][X] - afarVertex[0][X]) - (afarVertexNorm[2][0] - afarVertexNorm[0][0])*(afarVertex[1][X] - afarVertex[0][X]));
								afCoVariantRC = ((afarVertex[1][X] - afarVertex[0][X])*(afarVertex[2][Y] - afarVertex[0][Y]) - (afarVertex[1][Y] - afarVertex[0][Y])*(afarVertex[2][X] - afarVertex[0][X]));
								afCoVariantRD = -(afCoVariantRB*afarVertex[0][Y] + afCoVariantRA*afarVertex[0][X] + afCoVariantRC*afarVertexNorm[0][0]);

								//G for Y
								afCoVariantGA = ((afarVertexNorm[2][1] - afarVertexNorm[0][1])*(afarVertex[1][Y] - afarVertex[0][Y]) - (afarVertexNorm[1][1] - afarVertexNorm[0][1])*(afarVertex[2][Y] - afarVertex[0][Y]));
								afCoVariantGB = ((afarVertexNorm[1][1] - afarVertexNorm[0][1])*(afarVertex[2][X] - afarVertex[0][X]) - (afarVertexNorm[2][1] - afarVertexNorm[0][1])*(afarVertex[1][X] - afarVertex[0][X]));
								afCoVariantGC = ((afarVertex[1][X] - afarVertex[0][X])*(afarVertex[2][Y] - afarVertex[0][Y]) - (afarVertex[1][Y] - afarVertex[0][Y])*(afarVertex[2][X] - afarVertex[0][X]));
								afCoVariantGD = -(afCoVariantGA*afarVertex[0][X] + afCoVariantGB*afarVertex[0][Y] + afCoVariantGC*afarVertexNorm[0][1]);

								//B for Z
								afCoVariantBA = ((afarVertexNorm[2][2] - afarVertexNorm[0][2])*(afarVertex[1][Y] - afarVertex[0][Y]) - (afarVertexNorm[1][2] - afarVertexNorm[0][2])*(afarVertex[2][Y] - afarVertex[0][Y]));
								afCoVariantBB = ((afarVertexNorm[1][2] - afarVertexNorm[0][2])*(afarVertex[2][X] - afarVertex[0][X]) - (afarVertexNorm[2][2] - afarVertexNorm[0][2])*(afarVertex[1][X] - afarVertex[0][X]));
								afCoVariantBC = ((afarVertex[1][X] - afarVertex[0][X])*(afarVertex[2][Y] - afarVertex[0][Y]) - (afarVertex[1][Y] - afarVertex[0][Y])*(afarVertex[2][X] - afarVertex[0][X]));
								afCoVariantBD = -(afCoVariantBA*afarVertex[0][X] + afCoVariantBB*afarVertex[0][Y] + afCoVariantBC*afarVertexNorm[0][2]);

								float temp = -((afCoVariantRA*anCntRangeXTemp + afCoVariantRB * anCntRangeYTemp + afCoVariantRD) / afCoVariantRC);//norm X
								float temp1 = -((afCoVariantGA*anCntRangeXTemp + afCoVariantGB * anCntRangeYTemp + afCoVariantGD) / afCoVariantGC);//norm Y
								float temp2 = -((afCoVariantBA*anCntRangeXTemp + afCoVariantBB * anCntRangeYTemp + afCoVariantBD) / afCoVariantBC);//norm Z
								GzCoord asPointNorm = { temp, temp1, temp2 };
								GzColor asPintColor;
								GzShadingEquation(render, asPintColor, asPointNorm);

								anCoefficientR = ctoi(VerifyColor(asPintColor[0]));
								anCoefficientG = ctoi(VerifyColor(asPintColor[1]));
								anCoefficientB = ctoi(VerifyColor(asPintColor[2]));
							}
							if (GZ_FAILURE == GzPutDisplay(render->display, anCntRangeX, anCntRangeY, anCoefficientR, anCoefficientG, anCoefficientB, asAlpha, anInterZ))
							{
								return GZ_FAILURE;
							}
						}
					}
				}
			}
		}
		else if (nameList[nCnt] == GZ_NORMAL)
		{


			/*float afDiffuseLightX, afDiffuseLightY, afDiffuseLightZ, afAmbientLightX, afAmbientLightY, afAmbientLightZ;
			afAmbientLightX = afAmbientLightY = afAmbientLightZ = afDiffuseLightX = afDiffuseLightY = afDiffuseLightZ = 0;
			//How can I get E? It is acGzEye
			//To ensure NL and NE is the same positive or nagetive
			for (int anLights = 0; anLights < render->numlights; anLights)
			{
			float afResANL = render->lights[anLights].direction[X] * afarVertexNormalA[0] + render->lights[anLights].direction[Y] * afarVertexNormalA[1] + render->lights[anLights].direction[Z] * afarVertexNormalA[2];
			float afResANE = render->camera.lookat[0] * afarVertexNormalA[0] + render->camera.lookat[1] * afarVertexNormalA[1] + render->camera.lookat[2] * afarVertexNormalA[2];
			if (afResANL*afResANE < 0)
			{
			continue;
			}
			else if ((afResANL < 0) && (afResANE < 0))
			{

			}
			afDiffuseLightX += render->Kd[X] * render->lights[anLights].color[X] * (render->lights[anLights].direction[X] * afVar1 + render->lights[anLights].direction[Y] * afVar2 + render->lights[anLights].direction[Z] * afVar3);
			afDiffuseLightY += render->Kd[Y] * render->lights[anLights].color[Y] * (render->lights[anLights].direction[X] * afVar1 + render->lights[anLights].direction[Y] * afVar2 + render->lights[anLights].direction[Z] * afVar3);
			afDiffuseLightZ += render->Kd[Z] * render->lights[anLights].color[Z] * (render->lights[anLights].direction[X] * afVar1 + render->lights[anLights].direction[Y] * afVar2 + render->lights[anLights].direction[Z] * afVar3);
			GzCoord avR;
			avR[X] = (render->lights[anLights].direction[X] * afVar1 + render->lights[anLights].direction[Y] * afVar2 + render->lights[anLights].direction[Z] * afVar3) * 2 * afVar1 - render->lights[anLights].direction[X];
			avR[Y] = (render->lights[anLights].direction[X] * afVar1 + render->lights[anLights].direction[Y] * afVar2 + render->lights[anLights].direction[Z] * afVar3) * 2 * afVar2 - render->lights[anLights].direction[Y];
			avR[X] = (render->lights[anLights].direction[X] * afVar1 + render->lights[anLights].direction[Y] * afVar2 + render->lights[anLights].direction[Z] * afVar3) * 2 * afVar3 - render->lights[anLights].direction[Z];
			}
			afAmbientLightX = render->Ka[0] * render->ambientlight.color[0]; afAmbientLightY = render->Ka[1] * render->ambientlight.color[1]; afAmbientLightZ = render->Ka[2] * render->ambientlight.color[2];*/
		}
		else if (nameList[nCnt] == GZ_NULL_TOKEN)
		{
			continue;
		}
	}
	return GZ_SUCCESS;
}
