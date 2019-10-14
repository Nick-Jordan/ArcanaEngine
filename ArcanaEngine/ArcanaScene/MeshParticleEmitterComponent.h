#ifndef MESH_PARTICLE_EMITTER_COMPONENT_H_
#define MESH_PARTICLE_EMITTER_COMPONENT_H_

#include "SceneDefines.h"

#include "Timer.h"
#include "Field.h"
#include "GeometryComponent.h"
#include "Mesh.h"
#include "Curve.h"
#include "ForceUser.h"

namespace Arcana
{
	class ARCANA_SCENE_API MeshParticleEmitterRenderProcedure;

	struct ARCANA_SCENE_API Particle
	{
		Vector3d position;
		Vector3d velocity;
		Vector3d acceleration;
		Vector4f colorStart;
		Vector4f colorEnd;
		Vector4f color;
		double rotationPerParticleSpeed;
		Vector3d rotationAxis;
		float rotationSpeed;
		double angle;
		double energyStart;
		double energy;
		float sizeStart;
		float sizeEnd;
		float size;
		uint32 frame;
		float timeOnCurrentFrame;
	};

	struct ARCANA_SCENE_API ParticleEmitterProperties
	{
		double maxUpdateRate;

		uint32 emissionRate;
		bool ellipsoid; //shape enum???
		float sizeStartMin;
		float sizeStartMax;
		float sizeEndMin;
		float sizeEndMax;
		double energyMin;
		double energyMax;
		double particleMass;
		Vector4f colorStart;
		Vector4f colorStartVar;
		Vector4f colorEnd;
		Vector4f colorEndVar;
		Vector3f positionVar;
		Vector3f velocityVar;
		Vector3f accelerationVar;
		float rotationPerParticleSpeedMin;
		float rotationPerParticleSpeedMax;
		float rotationSpeedMin;
		float rotationSpeedMax;
		Vector3f rotationAxisVar;
		Vector3d rotationAxis;
		bool orbitPosition;
		bool orbitVelocity;
		bool orbitAcceleration;
		uint32 frameRandomOffset;
		bool useVelocityField;
		bool useAccelerationField;

		ParticleEmitterProperties()
			: maxUpdateRate(0.0),
			emissionRate(100),
			ellipsoid(false),
			sizeStartMin(0.01f),
			sizeStartMax(0.1f),
			sizeEndMin(0.01f),
			sizeEndMax(0.02f),
			energyMin(1000.0),
			energyMax(2000.0),
			particleMass(1.0),
			colorStart(Vector4f::one()),
			colorStartVar(Vector4f::zero()),
			colorEnd(Vector4f::zero()),
			colorEndVar(Vector4f::zero()),
			positionVar(Vector3f::zero()),
			velocityVar(Vector3f::one()),
			accelerationVar(Vector3f::zero()),
			rotationPerParticleSpeedMin(0.0f),
			rotationPerParticleSpeedMax(0.0f),
			rotationSpeedMin(0.0f),
			rotationSpeedMax(0.0f),
			rotationAxisVar(Vector3f::zero()),
			rotationAxis(Vector3d::unitZ()),
			orbitPosition(false),
			orbitVelocity(false),
			orbitAcceleration(false),
			frameRandomOffset(0),
			useVelocityField(false),
			useAccelerationField(false) { }
	};

	class ARCANA_SCENE_API MeshParticleEmitterComponent : public GeometryComponent
	{
	public:

		MeshParticleEmitterComponent();

		MeshParticleEmitterComponent(Mesh* mesh, uint32 maxParticles, const ParticleEmitterProperties& properties, const Shader& shader);

		~MeshParticleEmitterComponent();

		void initialize(Mesh* mesh, uint32 maxParticles, const ParticleEmitterProperties& properties, const Shader& shader);

		void setMaxParticles(uint32 maxParticles);

		uint32 getMaxParticles() const;

		uint32 getNumParticles() const;

		void start();

		void stop();

		bool isRunning() const;

		void emit(uint32 numParticles);

		void setVelocityVectorField(VectorField* field);

		void setVelocityCurve(Curve* curve);

		void setAccelerationVectorField(VectorField* field);

		void setTexture(Texture* texture);

		Texture* getTexture() const;

		virtual void update(double elapsedTime) override;

		virtual bool createRenderProcedure() override;

		virtual void updateRenderProcedure() override;

		void setOrbitVelocity(const Vector3d& velocity, bool orbit = true);

		void setOrbitAcceleration(const Vector3d& acceleration, bool orbit = true);

		void setOrbitPosition(bool orbit);

	private:

		uint32 _maxParticles;
		uint32 _numParticles;
		Particle* _particles;
		bool _running;
		double _lastUpdated;
		double _emitTime;
		double _timePerEmission;
		ParticleEmitterProperties _properties;
		Mesh* _mesh;
		Material* _material;
		RenderProcedure::RenderProperties _renderProperties;
		Matrix4d _rotation;
		Vector3d _position;
		Vector3d _velocity;
		Vector3d _acceleration;
		Vector3d _rotationAxis;
		VertexFormat _instanceFormat;
		VectorField* _velocityField;
		VectorField* _accelerationField;
		Curve* _velocityCurve;
		Texture* _texture;

		MeshParticleEmitterRenderProcedure* _meshParticleEmitterRenderProcedure;
	};
}
#endif // !MESH_PARTICLE_EMITTER_COMPONENT_H_
