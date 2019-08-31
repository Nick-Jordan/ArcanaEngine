#ifndef PROCEDURAL_GENERATOR_H_
#define PROCEDURAL_GENERATOR_H_

#include "Types.h"
#include "Array.h"
#include "Task.h"
#include "Scheduler.h"

#include "ProceduralStep.h"

namespace Arcana
{
	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	class ProceduralGenerator;

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType = int32>
	class GenerationTask : public Task
	{
	public:

		GenerationTask(const ProceduralParametersType& params, ProceduralGenerator<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* generator);

		virtual ~GenerationTask();

		virtual void run() override;

		virtual void done() override;

		void finalizeDataGeneration();

		ProceduralObjectType* getObject() const;

	private:

		ProceduralParametersType _parameters;
		ProceduralObjectType* _generatedObject;

		ProceduralGenerator<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* _generator;
	};

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType = int32>
	class ProceduralGenerator
	{
	public:

		ProceduralGenerator();

		~ProceduralGenerator();

		void generate(const ProceduralParametersType& params, const ObjectIDType& id = ObjectIDType(), uint32 numObjects = 1);//seed

		ProceduralObjectType* get(const ObjectIDType& id = ObjectIDType());

		GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* getTask(const ObjectIDType& id = ObjectIDType());

		Array<ProceduralObjectType*> getMany(uint32 numObjects);

		virtual void generateObjectAsync(const ProceduralParametersType& params, ProceduralObjectType** object) {};

		virtual void generateObject(const ProceduralParametersType& params, ProceduralObjectType** object) {};


	private:

		KeyValueArray<ObjectIDType, GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>*> _generatedObjects;
		Scheduler* _scheduler;
	};
}

#include "ProceduralGenerator.inl"

#endif // !PROCEDURAL_GENERATOR_H_