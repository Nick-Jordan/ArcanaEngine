
namespace Arcana
{
	template<class ProceduralObjectType, class ProceduralParametersType>
	ProceduralGenerator<ProceduralObjectType, ProceduralParametersType>::ProceduralGenerator()
		: _scheduler(new Scheduler()), _generationStepsSetup(false)
	{
	}

	template<class ProceduralObjectType, class ProceduralParametersType>
	ProceduralGenerator<ProceduralObjectType, ProceduralParametersType>::~ProceduralGenerator()
	{
		AE_DELETE(_scheduler);

		for (auto i = AsyncSteps.createIterator(); i; i++)
		{
			AE_DELETE(*i);
		}

		for (auto i = SyncSteps.createIterator(); i; i++)
		{
			AE_DELETE(*i);
		}
	}

	template<class ProceduralObjectType, class ProceduralParametersType>
	void ProceduralGenerator<ProceduralObjectType, ProceduralParametersType>::generate(const ProceduralParametersType& params, uint32 numObjects)
	{
		if (!_generationStepsSetup)
		{
			setupGenerationSteps();
			_generationStepsSetup = true;
		}

		while (numObjects > 0)
		{
			GenerationTask* generationTask = new GenerationTask(params, AsyncSteps, SyncSteps);
			_generatedObjects.add(generationTask);
			_scheduler->schedule(generationTask);
			numObjects--;
		}
	}

	template<class ProceduralObjectType, class ProceduralParametersType>
	ProceduralObjectType* ProceduralGenerator<ProceduralObjectType, ProceduralParametersType>::get()
	{
		if (!_generatedObjects.isEmpty())
		{
			GenerationTask* task = _generatedObjects.pop();
			task->wait();
			task->syncGeneration();
			ProceduralObjectType* object = task->getObject();
			AE_DELETE(task);
			return object;
		}

		return nullptr;
	}

	template<class ProceduralObjectType, class ProceduralParametersType>
	Array<ProceduralObjectType*> ProceduralGenerator<ProceduralObjectType, ProceduralParametersType>::getMany(uint32 numObjects)
	{
		Array<ProceduralObjectType*> objects;
		
		while (!_generatedObjects.isEmpty() && numObjects > 0)
		{
			GenerationTask* task = _generatedObjects.pop();
			task->wait();
			task->syncGeneration();
			objects.add(task->getObject());
			AE_DELETE(task);
			numObjects--;
		}

		return objects;
	}


	template<class ProceduralObjectType, class ProceduralParametersType>
	ProceduralGenerator<ProceduralObjectType, ProceduralParametersType>::GenerationTask::GenerationTask(
		const ProceduralParametersType& params, 
		const Array<ProceduralStep<ProceduralObjectType, ProceduralParametersType>*>& asyncSteps,
		const Array<ProceduralStep<ProceduralObjectType, ProceduralParametersType>*>& syncSteps)
		: _parameters(params), _generatedObject(nullptr), _asyncSteps(asyncSteps), _syncSteps(syncSteps)
	{

	}

	template<class ProceduralObjectType, class ProceduralParametersType>
	ProceduralGenerator<ProceduralObjectType, ProceduralParametersType>::GenerationTask::~GenerationTask()
	{

	}

	template<class ProceduralObjectType, class ProceduralParametersType>
	void ProceduralGenerator<ProceduralObjectType, ProceduralParametersType>::GenerationTask::run()
	{
		for (uint32 i = 0; i < _asyncSteps.size(); i++)
		{
			_asyncSteps[i]->perform(
				_parameters, 
				i > 0 ? _asyncSteps[i - 1] : nullptr,
				&_generatedObject
			);
		}
	}

	template<class ProceduralObjectType, class ProceduralParametersType>
	void ProceduralGenerator<ProceduralObjectType, ProceduralParametersType>::GenerationTask::done()
	{

	}

	template<class ProceduralObjectType, class ProceduralParametersType>
	void ProceduralGenerator<ProceduralObjectType, ProceduralParametersType>::GenerationTask::syncGeneration()
	{
		for (uint32 i = 0; i < _syncSteps.size(); i++)
		{
			_syncSteps[i]->perform(
				_parameters, 
				i > 0 ? _syncSteps[i - 1] : (!_asyncSteps.isEmpty() ? _asyncSteps.getLast() : nullptr),
				&_generatedObject
			);
		}
	}

	template<class ProceduralObjectType, class ProceduralParametersType>
	ProceduralObjectType* ProceduralGenerator<ProceduralObjectType, ProceduralParametersType>::GenerationTask::getObject() const
	{
		return _generatedObject;
	}
}
