#include "CurveTestComponent.h"

#include "CurveTestRenderProcedure.h"

CurveTestComponent::CurveTestComponent()
{
	initialize();
}

CurveTestComponent::~CurveTestComponent()
{
}

void CurveTestComponent::initialize()
{
	_curve = Curve(Curve::InterpolationMethod::BSpline);
	_curve.addPoint(Vector4d(0, 0, 0, 0));
	_curve.addPoint(Vector4d(500, 0, 0.0, 0));
	_curve.addPoint(Vector4d(500, 200, 0.0, 0));
	_curve.addPoint(Vector4d(200, -200, 0.0, 0));
	_curve.addPoint(Vector4d(-200, -100, 0.0, 0));
	_curve.addPoint(Vector4d(-300, 600, 0.0, 0));
	_curve.addPoint(Vector4d(300, 400, 0.0, 0));
	_curve.addPoint(Vector4d(0, 0, 0, 0));
	GeometryComponent::initialize();
}

bool CurveTestComponent::createRenderProcedure()
{
	_renderProcedure = new CurveTestRenderProcedure(&_curve);
	_renderProcedure->reference();

	return true;
}

void CurveTestComponent::update(double elapsedTime)
{
}

void CurveTestComponent::updateRenderProcedure()
{
}