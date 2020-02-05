#include "CollisionShape.h"

#include "StringUtils.h"
#include "Actor.h"

#include "ResourceManager.h"
#include "ResourceCreator.h"

namespace Arcana
{
	CollisionShape::Properties::Properties()
		: _collisionType(None), _isExplicit(false)
	{
		memset(&_shapeData, 0, sizeof(_shapeData));
	}
	CollisionShape::Properties::Properties(const Properties& properties) : _collisionType(properties._collisionType),	_isExplicit(properties._isExplicit)
	{
		memcpy(&_shapeData, &properties._shapeData, sizeof(_shapeData));
	}
	CollisionShape::Properties::~Properties()
	{

	}
	CollisionShape::Properties& CollisionShape::Properties::operator=(const Properties& properties)
	{
		_collisionType = properties._collisionType;
		_isExplicit = properties._isExplicit;

		memcpy(&_shapeData, &properties._shapeData, sizeof(_shapeData));

		//deal with heightfield/mesh

		return *this;
	}

	bool CollisionShape::Properties::isEmpty() const
	{
		return _collisionType == None;
	}

	CollisionShape::Properties CollisionShape::box()
	{
		Properties d;
		d._collisionType = Box;
		d._isExplicit = false;
		return d;
	}

	CollisionShape::Properties CollisionShape::box(const Vector3d& extents)
	{
		Properties d;
		d._collisionType = Box;
		memcpy(d._shapeData.box.extents, &extents.x, sizeof(double) * 3);
		d._isExplicit = true;
		return d;
	}

	CollisionShape::Properties CollisionShape::sphere()
	{
		Properties d;
		d._collisionType = Sphere;
		d._isExplicit = false;
		return d;
	}

	CollisionShape::Properties CollisionShape::sphere(double radius)
	{
		Properties d;
		d._collisionType = Sphere;
		d._shapeData.sphere.radius = radius;
		d._isExplicit = true;
		return d;
	}

	CollisionShape::Properties CollisionShape::capsule()
	{
		Properties d;
		d._collisionType = Capsule;
		d._isExplicit = false;
		return d;
	}

	CollisionShape::Properties CollisionShape::capsule(double radius, double height)
	{
		Properties d;
		d._collisionType = Capsule;
		d._shapeData.capsule.radius = radius;
		d._shapeData.capsule.height = height;
		d._isExplicit = true;
		return d;
	}


	CollisionShape::CollisionShape() : _shape(nullptr), _material(nullptr)
	{

	}

	CollisionShape::CollisionShape(const Properties& properties, const Vector3d& scale, bool dynamic, PhysicsMaterial* material, const Vector3d& centerOfMassOffset) : _shape(nullptr)
	{
		initialize(properties, scale, dynamic, material, centerOfMassOffset);
	}

	CollisionShape::CollisionShape(const CollisionShape& copy) : _properties(copy._properties), _shape(copy._shape)
	{
		if (_shape)
		{
			_shape->acquireReference();
		}
	}

	CollisionShape::~CollisionShape()
	{
		AE_RELEASE(_shape);
		AE_RELEASE(_material);
	}

	CollisionShape::Type CollisionShape::getType() const
	{
		return _properties._collisionType;
	}

	void CollisionShape::updateShape(Actor* actor)
	{
		if (!_properties._isExplicit && actor)
		{
			switch (_properties._collisionType)
			{
			case Box:
			{
				AxisAlignedBoundingBoxd box = actor->getBoundingBox();

				Vector3d scale = actor->getSceneComponent()->getWorldScale();
				Vector3d extents = box.getSize() * 0.5 * scale;

				_shape = PhysicsController::instance().getPhysicsCore()->createShape(
					physx::PxBoxGeometry(PXVEC3(extents)),
					*_material->getMaterial());
			}
			break;

			case Sphere:
			{
				Sphered sphere = actor->getBoundingSphere();

				double radius = sphere.getRadius() * actor->getSceneComponent()->getWorldScale().x;

				_shape = PhysicsController::instance().getPhysicsCore()->createShape(
					physx::PxSphereGeometry(sphere.getRadius()),
					*_material->getMaterial());
			}
			break;

			case Capsule:
			{
				AxisAlignedBoundingBoxd box = actor->getBoundingBox();

				double radius = Math::max((box.getMax().x - box.getMin().x) * 0.5, (box.getMax().z - box.getMin().z) * 0.5);
				double height = box.getMax().y - box.getMin().y;

				radius *= actor->getSceneComponent()->getWorldScale().x;
				height *= actor->getSceneComponent()->getWorldScale().y;

					_shape = PhysicsController::instance().getPhysicsCore()->createShape(
						physx::PxCapsuleGeometry(radius, height * 0.5),
						*_material->getMaterial());
			}
			break;
			}
		}
	}

	void CollisionShape::initialize(const Properties& properties, const Vector3d& scale, bool dynamic, PhysicsMaterial* material, const Vector3d& centerOfMassOffset)
	{
		_properties = properties;
		_material = material;

		AE_REFERENCE(_material);

		switch (properties._collisionType)
		{
		case Box:
		{
			if (properties._isExplicit)
			{
				_shape = PhysicsController::instance().getPhysicsCore()->createShape(
					physx::PxBoxGeometry(properties._shapeData.box.extents[0] / 2.0, properties._shapeData.box.extents[1] / 2.0, properties._shapeData.box.extents[2] / 2.0),
					*_material->getMaterial());
			}
		}
		break;

		case Sphere:
		{
			if (properties._isExplicit)
			{
				_shape = PhysicsController::instance().getPhysicsCore()->createShape(
					physx::PxSphereGeometry(properties._shapeData.sphere.radius),
					*_material->getMaterial());
			}
		}
		break;

		case Capsule:
		{
			if (properties._isExplicit)
			{
				_shape = PhysicsController::instance().getPhysicsCore()->createShape(
					physx::PxCapsuleGeometry(properties._shapeData.capsule.radius, properties._shapeData.capsule.height / 2.0),
					*_material->getMaterial());
			}
		}
		break;
		}
	}


	CollisionShape::Type CollisionShape::convertStringToShapeType(const std::string& shape)
	{
		std::string s = StringUtils::toLower(shape);

		if (s == "sphere")
		{
			return Sphere;
		}
		else if (s == "plane")
		{
			return Plane;
		}
		else if (s == "capsule")
		{
			return Capsule;
		}
		else if (s == "convexmesh")
		{
			return ConvexMesh;
		}
		else if (s == "trianglemesh")
		{
			return TriangleMesh;
		}
		else if (s == "heightfield")
		{
			return HeightField;
		}

		return Box;
	}


	physx::PxShape* CollisionShape::getShape() const
	{
		return _shape;
	}

	class CollisionShapeResource : public ResourceCreator<CollisionShape>
	{
	public:

		CollisionShapeResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<CollisionShape>(id, type, data, dependencyScheduler)
		{
			PhysicsMaterial* material = nullptr;

			LoadResourceTask<PhysicsMaterial>* materialTask = ResourceManager::instance().loadResource<PhysicsMaterial>(data.getResourceDependency("physicsMaterial"), dependencyScheduler);
			if (materialTask)
			{
				materialTask->wait();
				material = materialTask->get();
			}
			else
			{
				const ResourceData* materialData = data.getAdditionalData("physicsMaterial");

				if (materialData)
				{
					materialTask = ResourceManager::instance().buildResource<PhysicsMaterial>(GlobalObjectID(id.getName() + "::physicsMaterial"), "physicsMaterial", *materialData, dependencyScheduler);
					if (materialTask)
					{
						materialTask->wait();
						material = materialTask->get();
					}
				}
			}


			CollisionShape::Properties* properties = nullptr;

			LoadResourceTask<CollisionShape::Properties>* propertiesTask = ResourceManager::instance().loadResource<CollisionShape::Properties>(
				data.getResourceDependency("properties"), dependencyScheduler);
			if (propertiesTask)
			{
				propertiesTask->wait();
				properties = propertiesTask->get();
			}
			else
			{
				const ResourceData* propertiesData = data.getAdditionalData("properties");

				if (propertiesData)
				{
					propertiesTask = ResourceManager::instance().buildResource<CollisionShape::Properties>(
						GlobalObjectID(id.getName() + "::collisionShapeProperties"), "collisionShapeProperties", *propertiesData, dependencyScheduler);
					if (propertiesTask)
					{
						propertiesTask->wait();
						properties = propertiesTask->get();
					}
				}
			}

			std::string s = data.getStringParameter("scale", "1.0, 1.0, 1.0");
			Vector3d scale;

			size_t pos = s.find(",");
			scale.x = stod(s.substr(0, pos));
			s.erase(0, pos + 1);
			pos = s.find(",");
			scale.y = stod(s.substr(0, pos));
			s.erase(0, pos + 1);
			pos = s.find(",");
			scale.z = stod(s.substr(0, pos));
			s.erase(0, pos + 1);

			s = data.getStringParameter("centerOfMassOffset", "0.0, 0.0, 0.0");
			Vector3d centerOfMassOffset;

			pos = s.find(",");
			centerOfMassOffset.x = stod(s.substr(0, pos));
			s.erase(0, pos + 1);
			pos = s.find(",");
			centerOfMassOffset.y = stod(s.substr(0, pos));
			s.erase(0, pos + 1);
			pos = s.find(",");
			centerOfMassOffset.z = stod(s.substr(0, pos));
			s.erase(0, pos + 1);

			bool dynamic = data.getBoolParameter("dynamic");

			if (properties && material)
			{
				initialize(*properties, scale, dynamic, material, centerOfMassOffset);
			}
		}
	};

	class CollisionShapePropertiesResource : public ResourceCreator<CollisionShape::Properties>
	{
	public:

		CollisionShapePropertiesResource(const GlobalObjectID& id, const std::string& type, const ResourceData& data, Scheduler* dependencyScheduler)
			: ResourceCreator<CollisionShape::Properties>(id, type, data, dependencyScheduler)
		{

			CollisionShape::Type t = CollisionShape::convertStringToShapeType(data.getStringParameter("type", "box"));

			_collisionType = t;

			if (t == CollisionShape::Box)
			{
				std::string s = data.getStringParameter("extents", "no_extents");

				if (s == "no_extents")
				{
					_isExplicit = false;
				}
				else
				{
					Vector3d extents;

					size_t pos = s.find(",");
					extents.x = stod(s.substr(0, pos));
					s.erase(0, pos + 1);
					pos = s.find(",");
					extents.y = stod(s.substr(0, pos));
					s.erase(0, pos + 1);
					pos = s.find(",");
					extents.z = stod(s.substr(0, pos));
					s.erase(0, pos + 1);

					memcpy(_shapeData.box.extents, &extents.x, sizeof(double) * 3);
					_isExplicit = true;
				}
			}
			else if (t == CollisionShape::Sphere)
			{
				double radius = data.getDoubleParameter("radius", -1.0);

				if (radius < 0.0)
				{
					_isExplicit = false;
				}
				else
				{
					_shapeData.sphere.radius = radius;
					_isExplicit = true;
				}
			}
			else if (t == CollisionShape::Capsule)
			{
				double radius = data.getDoubleParameter("radius", -1.0);
				double height = data.getDoubleParameter("height", -1.0);

				if (radius < 0.0 || height < 0.0)
				{
					_isExplicit = false;
				}
				else
				{
					_shapeData.capsule.radius = radius;
					_shapeData.capsule.height = height;
					_isExplicit = true;
				}
			}
		}
	};

	Resource::Type<CollisionShapePropertiesResource> collisionShapePropertiesResource("collisionShapeProperties");
	Resource::Type<CollisionShapeResource> collisionShapeResource("collisionShape");

}