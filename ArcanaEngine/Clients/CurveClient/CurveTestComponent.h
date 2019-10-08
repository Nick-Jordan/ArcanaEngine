#ifndef CURVE_TEST_COMPONENT_H_
#define CURVE_TEST_COMPONENT_H_

#include "GeometryComponent.h"
#include "Curve.h"

using namespace Arcana;

class CurveTestComponent : public GeometryComponent
{
public:

	CurveTestComponent();

	virtual ~CurveTestComponent();

	virtual void initialize() override;

	virtual bool createRenderProcedure() override;

	virtual void update(double elapsedTime) override;

	virtual void updateRenderProcedure() override;

private:
	
	Curve _curve;
};

#endif // !CURVE_TEST_COMPONENT_H_