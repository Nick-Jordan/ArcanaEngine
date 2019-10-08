#include "CurveTestRenderProcedure.h"

CurveTestRenderProcedure::CurveTestRenderProcedure(Curve* curve) : _curve(curve), _lineRenderer(nullptr), _pointRenderer(nullptr), _material(nullptr)
{
	Properties.RendererStage = "TransparentObjectStage";

	VertexFormat::Attribute attribs[2] =
	{
		VertexFormat::Attribute(VertexFormat::Semantic::Position, 2),
		VertexFormat::Attribute(VertexFormat::Semantic::Color, 3)
	};

	VertexFormat format(2, attribs);

	_lineRenderer = new ImmediateRenderer(format, Mesh::LineStrip, false);
	_pointRenderer = new ImmediateRenderer(format, Mesh::Points, false);

	_material = new Material("color_material");
	Shader shader;
	shader.createProgram(Shader::Vertex, "resources/color_vert.glsl");
	shader.createProgram(Shader::Fragment, "resources/color_frag.glsl");
	_material->addTechnique(new Technique(shader));

	_lineRenderer->setMaterial(_material);
	_pointRenderer->setMaterial(_material);
}

CurveTestRenderProcedure::~CurveTestRenderProcedure()
{
	_curve = nullptr;
}

void CurveTestRenderProcedure::render()
{
	uint32 steps = 100;

	std::vector<CurveTestVertex> points;
	for (int i = 0; i < _curve->getNumPoints(); i++)
	{
		Vector4d val = _curve->getPoint(i).getValue();
		CurveTestVertex v;
		v.Position = Vector2f(val.x, val.y);
		v.Color = Vector3f(1.0f, 0.5f, 0.5f);
		points.push_back(v);
	}

	std::vector<CurveTestVertex> lines;
	for (uint32 i = 0; i <= steps; i++)
	{
		Curve::Value val = _curve->getValue((double)i / (double)steps);

		CurveTestVertex v;
		v.Position = Vector2f(val.Position.x, val.Position.y);
		v.Color = Vector3f(1.0f, 1.0f, 1.0f);
		lines.push_back(v);

		v.Color = Vector3f(val.Tangent.x, val.Tangent.y, val.Tangent.z);
		points.push_back(v);
	}

	Technique* technique = _material->getCurrentTechnique();
	if (technique)
	{
		for (uint32 i = 0; i < technique->getPassCount(); i++)
		{
			Shader* pass = technique->getPass(i);
			if (pass)
			{
				pass->bind();

				glPointSize(10.0);
				pass->getUniform("u_ProjectionMatrix").setValue(Projection.cast<float>());

				if (lines.size() > 0)
				{
					_lineRenderer->start();
					_lineRenderer->add(&lines[0], lines.size());
					_lineRenderer->draw();
					_lineRenderer->finish();
				}

				if (points.size() > 0)
				{
					_pointRenderer->start();
					_pointRenderer->add(&points[0], points.size());
					_pointRenderer->draw();
					_pointRenderer->finish();
				}

				pass->unbind();
			}
		}
	}
}

bool CurveTestRenderProcedure::isValidProcedure() 
{
	return _curve != nullptr;
}