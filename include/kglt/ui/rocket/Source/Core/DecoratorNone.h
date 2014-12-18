/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef ROCKETCOREDECORATORNONE_H
#define ROCKETCOREDECORATORNONE_H

#include "../../Include/Rocket/Core/Decorator.h"

namespace Rocket {
namespace Core {

/**
	A decorator used to represent a 'none' decorator; that is, a decorator that does nothing.

	@author Peter Curry
 */

class DecoratorNone : public Decorator
{
public:
	virtual ~DecoratorNone();

	/// Called on a decorator to generate any required per-element data for a newly decorated element.
	/// @param element[in] The newly decorated element.
	/// @return A handle to a decorator-defined data handle, or NULL if none is needed for the element.
	virtual DecoratorDataHandle GenerateElementData(Element* element);
	/// Called to release element data generated by this decorator.
	/// @param element_data[in] The element data handle to release.
	virtual void ReleaseElementData(DecoratorDataHandle element_data);

	/// Called to render the decorator on an element.
	/// @param element[in] The element to render the decorator on.
	/// @param element_data[in] The handle to the data generated by the decorator for the element.
	virtual void RenderElement(Element* element, DecoratorDataHandle element_data);
};

}
}

#endif
