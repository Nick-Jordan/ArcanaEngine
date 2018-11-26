#ifndef TASK_H_
#define TASK_H_

#include "IODefines.h"

#include "Object.h"

namespace Arcana
{
    class ARCANA_IO_API Task : public Object
    {
    public:

        Task(const std::string& type);

        virtual ~Task();

        virtual void run() = 0;
	};
}

#endif // !TASK_H_