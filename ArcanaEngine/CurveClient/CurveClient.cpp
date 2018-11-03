// CurveClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Curve.h"
#include "ArcanaLog.h"

//vld
#include <vld.h>

//dependencies
#include "ArcanaLog.h"
#include "ArcanaMath.h"

using namespace Arcana;

int main()
{

	Curve curveBezier = Curve(Curve::Bezier, 2);

	curveBezier.addPoint(0.0, new double[2]{ 3.71, 0.52 });// , new double[2]{ 3.71, 0.52 }, new double[2]{ 7.75, 4.0 });
	curveBezier.addPoint(0.25, new double[2]{ 7.75, 4.0 });// , new double[2]{ 3.71, 0.52 }, new double[2]{ 5.87, 7.46 });
	curveBezier.addPoint(0.75, new double[2]{ 5.87, 7.46 });// , new double[2]{ 7.75, 4.0 }, new double[2]{ 2, 6.34 });
	curveBezier.addPoint(1.0, new double[2]{ 2, 6.34 });// , new double[2]{ 5.87, 7.46 }, new double[2]{ 2, 6.34 });

	double valueBezier[2];

	curveBezier.getValue(0.5, 0.0, valueBezier);

	LOGF(Info, CoreEngine, "Bezier: %f, %f", valueBezier[0], valueBezier[1]);


	Curve curveHermite = Curve(Curve::Hermite, 2);

	curveHermite.addPoint(0.0, new double[2]{ 0.137, 0.184 });// , new double[2]{ 1.37, 0.184 }, new double[2]{ 1.515, 0.077 });
	curveHermite.addPoint(0.25, new double[2]{ 1.515, 0.077 });// , new double[2]{ 1.37, 0.184 }, new double[2]{ 1.9, -0.004 });
	curveHermite.addPoint(0.75, new double[2]{ 1.9, -0.004 });// , new double[2]{ 1.515, 0.077 }, new double[2]{ 1, 1 });
	curveHermite.addPoint(1.0, new double[2]{ 1, 1 });// , new double[2]{ 1.9, -0.004 }, new double[2]{ 1, 1 });

	double valueHermite[2];

	curveHermite.getValue(0.5, 0.0, valueHermite);

	LOGF(Info, CoreEngine, "Hermite: %f, %f", valueHermite[0], valueHermite[1]);

	system("pause");

    return 0;
}

