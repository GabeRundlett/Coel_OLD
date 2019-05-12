#pragma once

namespace Coel
{
namespace RenderAPI
{
class Context
{
public:
	virtual void init() = 0;
	virtual void swap() = 0;
};
} // namespace RenderAPI
} // namespace Coel
