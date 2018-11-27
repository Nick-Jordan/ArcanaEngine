#include "Cursor.h"

#include "CursorContext.h"

namespace Arcana
{
	Cursor::Cursor(Type type) : _type(type), _context(new CursorContext(type))
	{

	}

	Cursor::~Cursor()
	{
		AE_DELETE(_context);
	}

	Cursor::Type Cursor::getType() const
	{
		return _type;
	}

	const CursorContext& Cursor::getContext() const
	{
		return *_context;
	}
}