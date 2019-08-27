#ifndef PROCEDURAL_GENERATOR_H_
#define PROCEDURAL_GENERATOR_H_

#include "Types.h"
#include "Array.h"
#include "Task.h"
#include "Scheduler.h"

#include "ProceduralStep.h"

namespace Arcana
{
	template<class ProceduralObjectType, class ProceduralParametersType>
	class ProceduralGenerator
	{
	private:

		class GenerationTask : public Task
		{
		public:

			GenerationTask(const ProceduralParametersType& params, 
				const Array<ProceduralStep<ProceduralObjectType, ProceduralParametersType>*>& asyncSteps, 
				const Array<ProceduralStep<ProceduralObjectType, ProceduralParametersType>*>& syncSteps);

			virtual ~GenerationTask();

			virtual void run() override;

			virtual void done() override;

			void syncGeneration();

			ProceduralObjectType* getObject() const;

		private:

			ProceduralParametersType _parameters;
			ProceduralObjectType* _generatedObject;

			Array<ProceduralStep<ProceduralObjectType, ProceduralParametersType>*> _asyncSteps;
			Array<ProceduralStep<ProceduralObjectType, ProceduralParametersType>*> _syncSteps;
		};

	public:

		ProceduralGenerator();

		~ProceduralGenerator();

		void generate(const ProceduralParametersType& params, uint32 numObjects = 1);

		ProceduralObjectType* get();

		Array<ProceduralObjectType*> getMany(uint32 numObjects);

	private:

		Array<GenerationTask*> _generatedObjects;
		Scheduler* _scheduler;
		bool _generationStepsSetup;

	protected:

		virtual void setupGenerationSteps() {};

		Array<ProceduralStep<ProceduralObjectType, ProceduralParametersType>*> AsyncSteps;
		Array<ProceduralStep<ProceduralObjectType, ProceduralParametersType>*> SyncSteps;
	};
}

#include "ProceduralGenerator.inl"

#endif // !PROCEDURAL_GENERATOR_H_