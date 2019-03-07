#ifndef MESH_PARTICLE_EMITTER_COMPONENT_H_
#define MESH_PARTICLE_EMITTER_COMPONENT_H_

#include "SceneDefines.h"

#include "Timer.h"
#include "Field.h"
#include "GeometryComponent.h"

namespace Arcana
{
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

		void setTexture(Texture* texture);

		Texture* getTexture() const;

		virtual void update(double elapsedTime) override;

		virtual bool createRenderProcedure() override;

		virtual void updateRenderData(Matrix4d view, Matrix4d projection, Vector3d eyePosition) override;

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
		MeshRenderProperties _renderProperties;
		Matrix4d _rotation;
		Vector3d _position;
		Vector3d _velocity;
		Vector3d _acceleration;
		Vector3d _rotationAxis;
		VertexFormat _instanceFormat;
		VectorField* _velocityField;
		Texture* _texture;
	};
}
#endif // !MESH_PARTICLE_EMITTER_COMPONENT_H_
