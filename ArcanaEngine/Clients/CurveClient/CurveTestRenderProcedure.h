#ifndef CURVE_TEST_RENDER_PROCEDURE_H_
#define CURVE_TEST_RENDER_PROCEDURE_H_

#include "SceneDefines.h"

#include "MeshRenderProcedure.h"
#include "Curve.h"
#include "ImmediateRenderer.h"

using namespace Arcana;

struct CurveTestVertex
{
	Vector2f Position;
	Vector3f Color;
};

class CurveTestRenderProcedure : public RenderProcedure
{
public:

	CurveTestRenderProcedure(Curve* curve);

	virtual ~CurveTestRenderProcedure();

	virtual void render() override;

	virtual bool isValidProcedure() override;

private:

	Curve* _curve;
	ImmediateRenderer* _lineRenderer;
	ImmediateRenderer* _pointRenderer;
	Material* _material;
};

#endif // !DECAL_RENDER_PROCEDURE_H_

