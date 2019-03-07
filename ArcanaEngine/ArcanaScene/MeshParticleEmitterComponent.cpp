#include "MeshParticleEmitterComponent.h"

#include "Random.h"
#include "MeshParticleEmitterRenderProcedure.h"

namespace Arcana
{
	MeshParticleEmitterComponent::MeshParticleEmitterComponent() : _mesh(nullptr), _particles(nullptr), _running(false)
	{
	}

	MeshParticleEmitterComponent::MeshParticleEmitterComponent(Mesh* mesh, uint32 maxParticles, const ParticleEmitterProperties& properties, const Shader& shader) : _particles(nullptr)
	{
		initialize(mesh, maxParticles, properties, shader);
	}

	MeshParticleEmitterComponent::~MeshParticleEmitterComponent()
	{
		AE_DELETE_ARRAY(_particles);
		AE_RELEASE(_mesh);
		AE_RELEASE(_material);
		AE_RELEASE(_texture);
	}

	void MeshParticleEmitterComponent::initialize(Mesh* mesh, uint32 maxParticles, const ParticleEmitterProperties& properties, const Shader& shader)
	{
		_mesh = mesh;
		setMaxParticles(maxParticles);
		_properties = properties;
		_running = false;
		_position = Vector3d::zero();
		_velocity = Vector3d::zero();
		_acceleration = Vector3d::zero();
		_rotationAxis = Vector3d::zero();
		_rotation = Matrix4d::IDENTITY;
		_numParticles = 0;
		_timePerEmission = 1000.0 / (double)_properties.emissionRate;
		_emitTime = 0.0;
		_texture = nullptr;

		_renderProperties.rendererStage = "TransparentObjectStage";
		_renderProperties.lightProperties.CastsDynamicShadow = false;

		_renderProperties.renderState.setCullEnabled(true);
		_renderProperties.renderState.setCullFaceSide(RenderState::Back);
		//_renderProperties.renderState.setDepthTestEnabled(true);????????????????????????
		_renderProperties.renderState.setBlendEnabled(true);
		_renderProperties.renderState.setBlendSrc(RenderState::Blend::SrcAlpha);
		_renderProperties.renderState.setBlendDst(RenderState::Blend::OneMinusSrcAlpha);

		if (_mesh)
		{
			VertexFormat::Attribute instanceAttribs[] =
			{
				VertexFormat::Attribute(VertexFormat::Semantic::Position, 3),
				//VertexFormat::Attribute(VertexFormat::Semantic::TexCoord0, 2),
				VertexFormat::Attribute(VertexFormat::Semantic::Normal, 3),
				VertexFormat::Attribute(VertexFormat::Semantic::Color, 4),

				VertexFormat::Attribute(VertexFormat::Semantic::Binormal, 3),
				VertexFormat::Attribute(VertexFormat::Semantic::Binormal, 3),
				VertexFormat::Attribute(VertexFormat::Semantic::Binormal, 3),
			};
			_instanceFormat = VertexFormat(6, instanceAttribs, 1);
			_mesh->setInstanceBuffer(_instanceFormat, 0, true);

			_material = new Material("particle_component");
			Technique* technique = new Technique(shader);
			_material->addTechnique(technique);

			_mesh->reference();
			_material->reference();
		}

		GeometryComponent::initialize();
	}

	void MeshParticleEmitterComponent::setMaxParticles(uint32 maxParticles)
	{
		if (_maxParticles != maxParticles)
		{
			_maxParticles = maxParticles;
			AE_DELETE_ARRAY(_particles);

			if (_maxParticles > 0)
			{
				_particles = new Particle[_maxParticles];
			}
		}
	}

	uint32 MeshParticleEmitterComponent::getMaxParticles() const
	{
		return _maxParticles;
	}

	uint32 MeshParticleEmitterComponent::getNumParticles() const
	{
		return _numParticles;
	}

	void MeshParticleEmitterComponent::start()
	{
		_running = true;
		_lastUpdated = 0;
	}

	void MeshParticleEmitterComponent::stop()
	{
		_running = false;
	}

	bool MeshParticleEmitterComponent::isRunning() const
	{
		return _running;
	}

	void generateVectorInRect(const Vector3d& base, const Vector3f& variance, Vector3d* dst)
	{
		dst->x = base.x + variance.x * (Random<float>::random() * 2.0 - 1.0);
		dst->y = base.y + variance.y * (Random<float>::random() * 2.0 - 1.0);
		dst->z = base.z + variance.z *(Random<float>::random() * 2.0 - 1.0);
	}

	void generateVectorInEllipsoid(const Vector3d& center, const Vector3f& scale, Vector3d* dst)
	{
		do
		{
			dst->x = (Random<float>::random() * 2.0 - 1.0);
			dst->y = (Random<float>::random() * 2.0 - 1.0);
			dst->z = (Random<float>::random() * 2.0 - 1.0);
		} while (dst->magnitude() > 1.0f);

		dst->x *= scale.x;
		dst->y *= scale.y;
		dst->z *= scale.z;

		dst->x += center.x;
		dst->y += center.y;
		dst->z += center.z;
	}

	void generateVector(const Vector3d& base, const Vector3f& variance, Vector3d* dst, bool ellipsoid)
	{
		if (ellipsoid)
		{
			generateVectorInEllipsoid(base, variance, dst);
		}
		else
		{
			generateVectorInRect(base, variance, dst);
		}
	}

	void generateColor(const Vector4f& base, const Vector4f& variance, Vector4f* dst)
	{
		dst->x = base.x + variance.x * (Random<float>::random() * 2.0 - 1.0);
		dst->y = base.y + variance.y * (Random<float>::random() * 2.0 - 1.0);
		dst->z = base.z + variance.z * (Random<float>::random() * 2.0 - 1.0);
		dst->w = base.w + variance.w * (Random<float>::random() * 2.0 - 1.0);
	}

	void MeshParticleEmitterComponent::emit(uint32 numParticles)
	{
		if (_particles)
		{

			if (numParticles + _numParticles > _maxParticles)
			{
				numParticles = _maxParticles - _numParticles;
			}

			Vector3d translation = getWorldPosition();
			Transform worldTransform = getWorldTransform();
			worldTransform.setTranslation(Vector3d::zero());
			Matrix4d world = worldTransform.getMatrix();

			for (uint32 i = 0; i < numParticles; i++)
			{
				Particle* p = &_particles[_numParticles];

				generateColor(_properties.colorStart, _properties.colorStartVar, &p->colorStart);
				generateColor(_properties.colorEnd, _properties.colorEndVar, &p->colorEnd);
				p->color.set(p->colorStart);

				p->energy = p->energyStart = Random<double>::random(_properties.energyMin, _properties.energyMax);
				p->size = p->sizeStart = Random<float>::random(_properties.sizeStartMin, _properties.sizeStartMax);
				p->sizeEnd = Random<float>::random(_properties.sizeEndMin, _properties.sizeEndMax);
				p->rotationPerParticleSpeed = Random<double>::random(_properties.rotationPerParticleSpeedMin, _properties.rotationPerParticleSpeedMax);
				p->angle = Random<double>::random(0.0, p->rotationPerParticleSpeed);
				p->rotationSpeed = Random<float>::random(_properties.rotationSpeedMin, _properties.rotationSpeedMax);

				generateVector(_position, _properties.positionVar, &p->position, _properties.ellipsoid);
				if (!_properties.useVelocityField)
				{
					generateVector(_velocity, _properties.velocityVar, &p->velocity, false);
				}
				else
				{
					p->velocity = _velocityField->get(_position);
				}
				generateVector(_acceleration, _properties.accelerationVar, &p->acceleration, false);
				generateVector(_properties.rotationAxis, _properties.rotationAxisVar, &p->rotationAxis, false);


				if (_properties.orbitPosition)
				{
					p->position = world * p->position;
				}

				if (_properties.orbitVelocity)
				{
					p->velocity = world * p->velocity;
				}

				if (_properties.orbitAcceleration)
				{
					p->acceleration = world * p->acceleration;
				}

				if (p->rotationSpeed != 0.0f && !p->rotationAxis.isZero())
				{
					p->rotationAxis = world * p->rotationAxis;
				}

				p->position += (translation);

				if (_properties.frameRandomOffset > 0)
				{
					p->frame = (uint32)Random<double>::random(0, _properties.frameRandomOffset);
				}
				else
				{
					p->frame = 0;
				}

				p->timeOnCurrentFrame = 0.0f;

				_numParticles++;
			}
		}
	}

	void MeshParticleEmitterComponent::setVelocityVectorField(VectorField* field)
	{
		_velocityField = field;
	}

	void MeshParticleEmitterComponent::setTexture(Texture* texture)
	{
		_texture = texture;
		if (_texture)
		{
			_texture->reference();
		}
	}

	Texture* MeshParticleEmitterComponent::getTexture() const
	{
		return _texture;
	}

	void MeshParticleEmitterComponent::update(double elapsedTime)
	{
		static double runningTime = 0;
		runningTime += elapsedTime;
		if (runningTime < _properties.maxUpdateRate)
		{
			return;
		}

		double elapsedMs = runningTime * 1000.0;
		runningTime = 0;

		if (_running && _properties.emissionRate)
		{
			_emitTime += elapsedMs;

			uint32 count = (uint32)(_emitTime / _timePerEmission);

			if (count)
			{
				if ((int32)_timePerEmission > 0)
				{
					_emitTime = fmod((double)_emitTime, (double)_timePerEmission);
				}
				emit(count);
			}
		}

		for (uint32 particlesIndex = 0; particlesIndex < _numParticles; particlesIndex++)
		{
			Particle* p = &_particles[particlesIndex];
			p->energy -= elapsedMs;

			if (p->energy > 0.0)
			{
				if (p->rotationSpeed != 0.0f && !p->rotationAxis.isZero())
				{
					_rotation = _rotation * Matrix4d::createRotation(p->rotationAxis, p->rotationSpeed * elapsedTime);

					p->velocity = _rotation * p->velocity;
					p->acceleration = _rotation * p->acceleration;
				}

				if (!_properties.useVelocityField)
				{
					p->velocity.x += p->acceleration.x * elapsedTime;
					p->velocity.y += p->acceleration.y * elapsedTime;
					p->velocity.z += p->acceleration.z * elapsedTime;
				}
				else
				{
					p->velocity = _velocityField->get(p->position);
				}

				p->position.x += p->velocity.x * elapsedTime;
				p->position.y += p->velocity.y * elapsedTime;
				p->position.z += p->velocity.z * elapsedTime;

				p->angle += p->rotationPerParticleSpeed * elapsedTime;

				double percentLifetime = 1.0 - (p->energy / p->energyStart);

				p->color.x = p->colorStart.x + (p->colorEnd.x - p->colorStart.x) * percentLifetime;
				p->color.y = p->colorStart.y + (p->colorEnd.y - p->colorStart.y) * percentLifetime;
				p->color.z = p->colorStart.z + (p->colorEnd.z - p->colorStart.z) * percentLifetime;
				p->color.w = p->colorStart.w + (p->colorEnd.w - p->colorStart.w) * percentLifetime;

				p->size = p->sizeStart + (p->sizeEnd - p->sizeStart) * percentLifetime;
			}
			else
			{
				if (particlesIndex != _numParticles - 1)
				{
					_particles[particlesIndex] = _particles[_numParticles - 1];
				}
				_numParticles = _numParticles - 1;
			}
		}
	}

	bool MeshParticleEmitterComponent::createRenderProcedure()
	{
		_renderProcedure = new MeshParticleEmitterRenderProcedure(_mesh, _material, _renderProperties);
		_renderProcedure->reference();

		_renderProcedure->createRenderData();
		return false;
	}

	void MeshParticleEmitterComponent::updateRenderData(Matrix4d view, Matrix4d projection, Vector3d eyePosition)
	{
		MeshParticleEmitterRenderDataUpdate update;
		update.view = view;
		update.projection = projection;
		update.eyePosition = eyePosition;
		update.transform.set(getWorldTransform());
		update.particles = _particles;
		update.instanceFormat = _instanceFormat;
		update.numParticles = _numParticles;
		update.texture = _texture;

		_renderProcedure->updateRenderData(update);
	}
}
