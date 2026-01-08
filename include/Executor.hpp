#pragma once
#include <string>

namespace adas
{
struct Pose
{
    int x;
    int y;
    char heading;
};

class Executor
{
public:
    virtual ~Executor() = default;
    virtual void Execute(const std::string& command) noexcept = 0;
    virtual Pose Query(void) const noexcept = 0;
    
    static Executor* NewExecutor(const Pose& pose = {0, 0, 'N'}) noexcept;
};
}  // namespace adas