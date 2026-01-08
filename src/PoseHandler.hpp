#pragma once
#include "Executor.hpp"
#include "Direction.hpp"
#include "Point.hpp"

namespace adas
{
class PoseHandler final
{
public:
    PoseHandler(const Pose& pose) noexcept;
    PoseHandler(const PoseHandler&) = delete;
    PoseHandler& operator=(const PoseHandler&) = delete;

public:
    void Move(void) noexcept;
    void TurnLeft(void) noexcept;
    void TurnRight(void) noexcept;
    void Fast(void) noexcept;
    bool IsFast(void) const noexcept;
    void Reverse(void) noexcept;
    bool IsReverse(void) const noexcept;
    void MoveBackward() noexcept;
    Pose Query(void) const noexcept;

private:
    Point point;
    const Direction* facing;
    bool fast{false};
    bool reverse{false};
};
} // namespace adas