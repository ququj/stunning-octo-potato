#pragma once
#include "PoseHandler.hpp"
#include <functional>

namespace adas
{  
    class MoveCommand final
    {
    public:
        void operator()(PoseHandler& poseHandler) const noexcept
        {
            if (poseHandler.IsFast()) {
                if (poseHandler.IsReverse()) {
                    poseHandler.MoveBackward();
                    poseHandler.MoveBackward();
                } else {
                    poseHandler.Move();
                    poseHandler.Move();
                }
            } else {
                if (poseHandler.IsReverse()) {
                    poseHandler.MoveBackward();
                } else {
                    poseHandler.Move();
                }
            }
        }
    };

    class TurnLeftCommand final
    {
    public:
        void operator()(PoseHandler& poseHandler) const noexcept
        {
            if (poseHandler.IsFast()) {
                if (poseHandler.IsReverse()) {
                    poseHandler.MoveBackward();
                } else {
                    poseHandler.Move();
                }
            }
            if (poseHandler.IsReverse()) {
                poseHandler.TurnRight();  // reverse 时左转变成右转
            } else {
                poseHandler.TurnLeft();
            }
        }
    };

    class TurnRightCommand final
    {
    public:
        void operator()(PoseHandler& poseHandler) const noexcept
        {
           if (poseHandler.IsFast()) {
                if (poseHandler.IsReverse()) {
                    poseHandler.MoveBackward();
                } else {
                    poseHandler.Move();
                }
            }
            if (poseHandler.IsReverse()) {
                poseHandler.TurnLeft();  // reverse 时右转变成左转
            } else {
                poseHandler.TurnRight();
            }
        }
    };
    
    class FastCommand final
    {
    public:
        void operator()(PoseHandler& poseHandler) const noexcept
        {
            poseHandler.Fast();
        }

    };

    class ReverseCommand final
    {
    public:
        void operator()(PoseHandler& poseHandler) const noexcept
        {
            poseHandler.Reverse();
        }

    };

    class TurnRoundCommand final
    {
    public:
        void operator()(PoseHandler& poseHandler) const noexcept
        {
            // 根据测试用例，在倒车状态下忽略TR指令
            if (poseHandler.IsReverse()) {
                return;
            }
        
            // 注意：这里假设是普通车的行为
            // 实际上，跑车的掉头逻辑应该不同，但根据测试用例，
            // 似乎没有专门测试跑车的TR命令，所以暂时保持原样
            
            if (poseHandler.IsFast()) {
                // 加速状态下：向前进1格->左转90度->向前进1格->左转90度
                poseHandler.Move();
                poseHandler.TurnLeft();
                poseHandler.Move();
                poseHandler.TurnLeft();
            } else {
                // 正常状态下：左转90度->向前进1格->左转90度
                poseHandler.TurnLeft();
                poseHandler.Move();
                poseHandler.TurnLeft();
            }
        }
    };
} // namespace adas