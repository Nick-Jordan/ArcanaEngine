#ifndef TASK_H_
#define TASK_H_

#include "IODefines.h"

#include "Object.h"

namespace Arcana
{
    class ARCANA_IO_API Task : public Object
    {
		friend class Scheduler;

    public:

        Task(const std::string& type);

        virtual ~Task();

        virtual void run() = 0;

		bool isDone() const;

	private:

		bool _done;
	};
}

#endif // !TASK_H_