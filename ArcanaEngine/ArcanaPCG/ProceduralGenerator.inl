
namespace Arcana
{
	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	ProceduralGenerator<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::ProceduralGenerator()
		: _scheduler(new Scheduler())
	{
	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	ProceduralGenerator<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::~ProceduralGenerator()
	{
		AE_DELETE(_scheduler);
	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	void ProceduralGenerator<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::generate(const ProceduralParametersType& params, const ObjectIDType& id, uint32 numObjects)
	{
		while (numObjects > 0)
		{
			GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* generationTask = new GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>(params, this);
			_generatedObjects.add(MakePair(id, generationTask));
			_scheduler->schedule(generationTask);
			numObjects--;
		}
	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	ProceduralObjectType* ProceduralGenerator<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::get(const ObjectIDType& id)
	{
		if (!_generatedObjects.isEmpty())
		{
			GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* task = _generatedObjects.findByPredicate([&](const KeyValuePair< ObjectIDType, GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>*>& pair)
				{
					return(pair.key == id);
				})->value;
			task->wait();
			task->finalizeDataGeneration();
			ProceduralObjectType* object = task->getObject();
			AE_DELETE(task);
			return object;
		}

		return nullptr;
	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>*
		ProceduralGenerator<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::getTask(const ObjectIDType& id)
	{
		if (!_generatedObjects.isEmpty())
		{
			GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* task = _generatedObjects.findByPredicate([&](const KeyValuePair< ObjectIDType, GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>*>& pair)
				{
					return(pair.key == id);
				})->value;
			return task;
		}

		return nullptr;
	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	Array<ProceduralObjectType*> ProceduralGenerator<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::getMany(uint32 numObjects)
	{
		Array<ProceduralObjectType*> objects;
		
		while (!_generatedObjects.isEmpty() && numObjects > 0)
		{
			GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* task = _generatedObjects.pop().value;
			task->wait();
			task->finalizeDataGeneration();
			ProceduralObjectType* object = task->getObject();
			objects.add(object);
			AE_DELETE(task);
			numObjects--;
		}

		return objects;
	}


	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::GenerationTask(
		const ProceduralParametersType& params, 
		ProceduralGenerator<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* generator)
		: _parameters(params), _generatedObject(nullptr), _generator(generator)
	{

	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::~GenerationTask()
	{

	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	void GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::run()
	{
		_generator->generateObjectAsync(_parameters, &_generatedObject);
	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	void GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::done()
	{
	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	void GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::finalizeDataGeneration()
	{
		_generator->generateObject(_parameters, &_generatedObject);
	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	ProceduralObjectType* GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::getObject() const
	{
		return _generatedObject;
	}
}
