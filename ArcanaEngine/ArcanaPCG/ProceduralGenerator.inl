
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

		//fix thiss
		/*for (auto iter = _generatedObjects.createIterator(); iter; iter++)
		{
			GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* task = (*iter).value;
			task->wait();
			task->finalizeDataGeneration();
			AE_DELETE(task);
		}*/
	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* ProceduralGenerator<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::generate(const ProceduralParametersType& params, const ObjectIDType& id)
	{
		GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* generationTask = new GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>(params, this);
		//_generatedObjects.add(MakePair(id, generationTask));
		//_generatedObjects.insert(std::make_pair(id, generationTask));
		_scheduler->schedule(generationTask);

		return generationTask;
	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	ProceduralObjectType* ProceduralGenerator<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::get(const ObjectIDType& id, bool wait)
	{
		//if (!_generatedObjects.isEmpty())
		if(!_generatedObjects.empty())
		{
			/*GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* task = _generatedObjects.findByPredicate([&](const KeyValuePair< ObjectIDType, GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>*>& pair)
				{
					return(pair.key == id);
				})->value;*/

			GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* task = _generatedObjects.at(id);

			if (task)
			{
				if (wait)
				{
					task->wait();
				}
				if (task->isDone())
				{
					task->finalizeDataGeneration();
					ProceduralObjectType* object = task->getResult();//get()
					AE_DELETE(task);
					return object;
				}
			}
		}

		return nullptr;
	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>*
		ProceduralGenerator<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::getTask(const ObjectIDType& id)
	{
		//if (!_generatedObjects.isEmpty())
		if (!_generatedObjects.empty())
		{
			/*GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* task = _generatedObjects.findByPredicate([&](const KeyValuePair< ObjectIDType, GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>*>& pair)
				{
					return(pair.key == id);
				})->value;*/
			GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* task = _generatedObjects.at(id);
			return task;
		}

		return nullptr;
	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	bool ProceduralGenerator<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::isDone(const ObjectIDType& id)
	{
		GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* task = getTask(id);
		if (task)
		{
			return task->isDone();
		}

		return true;
	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::GenerationTask(
		const ProceduralParametersType& params,
		ProceduralGenerator<ProceduralObjectType, ProceduralParametersType, ObjectIDType>* generator)
		: _parameters(params), _generator(generator), _procObject(nullptr)
	{

	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::~GenerationTask()
	{

	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	void GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::run()
	{
		ProceduralObjectType** pointer = &_procObject;//??task result
		_generator->generateObjectAsync(_parameters, pointer);
	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	void GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::done()
	{
	}

	template<class ProceduralObjectType, class ProceduralParametersType, typename ObjectIDType>
	void GenerationTask<ProceduralObjectType, ProceduralParametersType, ObjectIDType>::finalizeDataGeneration()
	{
		ProceduralObjectType** pointer = &_procObject;
		_generator->generateObject(_parameters, pointer);
	}
}
