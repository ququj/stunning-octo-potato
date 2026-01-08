#include "ExecutorImpl.hpp"
#include "Command.hpp"
#include <memory>
#include <string>  // 添加string头文件

namespace adas
{
// 车辆配置定义 - 使用构造函数初始化
const CarConfig ExecutorImpl::normalCarConfig(1, 2, false, false);
const CarConfig ExecutorImpl::sportsCarConfig(2, 4, true, true);
const CarConfig ExecutorImpl::busCarConfig(1, 2, true, false); // 新增Bus配置

Executor* Executor::NewExecutor(const Pose& pose) noexcept
{
    return new (std::nothrow) ExecutorImpl(pose);
}

// 修改初始化列表顺序以匹配声明顺序
ExecutorImpl::ExecutorImpl(const Pose& pose) noexcept 
    : posehandler(pose),                    // 第一：按照声明顺序
      currentConfig(&normalCarConfig),      // 第二：按照声明顺序
      carType(CarType::NORMAL)              // 第四：按照声明顺序
{
    // commandTable会自动默认初始化
    InitializeNormalCarCommands();
}

// 修改Execute函数以处理U命令
void ExecutorImpl::Execute(const std::string& commands) noexcept
{
    for (size_t i = 0; i < commands.size(); ++i) {
        char cmd = commands[i];
        
        // 检查是否是TR指令
        if (cmd == 'T' && i + 1 < commands.size() && commands[i + 1] == 'R') {
            // 使用TR命令处理
            TurnRoundCommand turnRoundCmd;
            turnRoundCmd(posehandler);
            ++i; // 跳过R字符
        }
        // 处理N命令切换车辆
        else if (cmd == 'N') {
            if (carType != CarType::BUS) {  // Bus不能切换到跑车
                SwitchCarType();
            }
        }
        // 处理U命令切换到Bus
        else if (cmd == 'U') {
            if (carType != CarType::BUS) {
                SwitchToBus();
            } else {
                // 如果已经是Bus，则切换回普通车
                SwitchToNormal();
            }
        }
        // 处理其他命令
        else {
            auto it = commandTable.find(cmd);
            if (it != commandTable.end()) {
                it->second(posehandler);
            }
        }
    }
}

// 修改SwitchCarType函数
void ExecutorImpl::SwitchCarType()
{
    if (carType == CarType::NORMAL) {
        carType = CarType::SPORTS;
        currentConfig = &sportsCarConfig;
        InitializeSportsCarCommands();
    } else {
        carType = CarType::NORMAL;
        currentConfig = &normalCarConfig;
        InitializeNormalCarCommands();
    }
    
    // 切换车辆类型时重置fast和reverse状态
    // 注意：根据测试用例，切换车辆类型时状态应该重置
    if (posehandler.IsFast()) {
        posehandler.Fast(); // 调用Fast切换状态
    }
    if (posehandler.IsReverse()) {
        posehandler.Reverse(); // 调用Reverse切换状态
    }
}

// 新增：切换到Bus
void ExecutorImpl::SwitchToBus()
{
    carType = CarType::BUS;
    currentConfig = &busCarConfig;
    InitializeBusCommands();
    
    // 切换车辆类型时重置fast和reverse状态
    if (posehandler.IsFast()) {
        posehandler.Fast(); // 调用Fast切换状态
    }
    if (posehandler.IsReverse()) {
        posehandler.Reverse(); // 调用Reverse切换状态
    }
}

// 新增：从Bus切换回普通车
void ExecutorImpl::SwitchToNormal()
{
    carType = CarType::NORMAL;
    currentConfig = &normalCarConfig;
    InitializeNormalCarCommands();
    
    // 切换车辆类型时重置fast和reverse状态
    if (posehandler.IsFast()) {
        posehandler.Fast(); // 调用Fast切换状态
    }
    if (posehandler.IsReverse()) {
        posehandler.Reverse(); // 调用Reverse切换状态
    }
}

int ExecutorImpl::GetMoveDistance() const
{
    if (posehandler.IsFast()) {
        return currentConfig->fastMoveDistance;
    } else {
        return currentConfig->normalMoveDistance;
    }
}

void ExecutorImpl::HandleMove(PoseHandler& handler)
{
    bool isReverse = handler.IsReverse();
    int distance = GetMoveDistance();
    
    // 执行移动
    for (int i = 0; i < distance; ++i) {
        if (isReverse) {
            handler.MoveBackward();
        } else {
            handler.Move();
        }
    }
}

void ExecutorImpl::HandleTurnLeft(PoseHandler& handler)
{
    bool isReverse = handler.IsReverse();
    bool isFast = handler.IsFast();
    
    // 跑车转向逻辑
    if (carType == CarType::SPORTS) {
        // 加速状态下：先移动1格
        if (isFast) {
            if (isReverse) {
                handler.MoveBackward();
            } else {
                handler.Move();
            }
        }
        
        // 转向（倒车时左右相反）
        if (isReverse) {
            handler.TurnRight();  // reverse 时左转变成右转
        } else {
            handler.TurnLeft();
        }
        
        // 跑车总是会在转向后移动1格
        // 如果是加速状态，这是第二次移动；如果是普通状态，这是第一次移动
        if (isReverse) {
            handler.MoveBackward();
        } else {
            handler.Move();
        }
    } else {
        // 普通车使用原来的命令类，不会执行到这里
        // 为安全起见，调用原来的命令
        TurnLeftCommand cmd;
        cmd(handler);
    }
}

void ExecutorImpl::HandleTurnRight(PoseHandler& handler)
{
    bool isReverse = handler.IsReverse();
    bool isFast = handler.IsFast();
    
    // 跑车转向逻辑
    if (carType == CarType::SPORTS) {
        // 加速状态下：先移动1格
        if (isFast) {
            if (isReverse) {
                handler.MoveBackward();
            } else {
                handler.Move();
            }
        }
        
        // 转向（倒车时左右相反）
        if (isReverse) {
            handler.TurnLeft();  // reverse 时右转变成左转
        } else {
            handler.TurnRight();
        }
        
        // 跑车总是会在转向后移动1格
        // 如果是加速状态，这是第二次移动；如果是普通状态，这是第一次移动
        if (isReverse) {
            handler.MoveBackward();
        } else {
            handler.Move();
        }
    } else {
        // 普通车使用原来的命令类，不会执行到这里
        // 为安全起见，调用原来的命令
        TurnRightCommand cmd;
        cmd(handler);
    }
}

// Bus特有命令处理函数
void ExecutorImpl::HandleBusMove(PoseHandler& handler)
{
    bool isReverse = handler.IsReverse();
    int distance = GetMoveDistance();
    
    // 执行移动
    for (int i = 0; i < distance; ++i) {
        if (isReverse) {
            handler.MoveBackward();
        } else {
            handler.Move();
        }
    }
}

void ExecutorImpl::HandleBusTurnLeft(PoseHandler& handler)
{
    bool isReverse = handler.IsReverse();
    int distance = GetMoveDistance();
    
    // Bus转向逻辑：先移动，再转向
    // 执行移动
    for (int i = 0; i < distance; ++i) {
        if (isReverse) {
            handler.MoveBackward();
        } else {
            handler.Move();
        }
    }
    
    // 转向（倒车时左右相反）
    if (isReverse) {
        handler.TurnRight();  // reverse 时左转变成右转
    } else {
        handler.TurnLeft();
    }
}

void ExecutorImpl::HandleBusTurnRight(PoseHandler& handler)
{
    bool isReverse = handler.IsReverse();
    int distance = GetMoveDistance();
    
    // Bus转向逻辑：先移动，再转向
    // 执行移动
    for (int i = 0; i < distance; ++i) {
        if (isReverse) {
            handler.MoveBackward();
        } else {
            handler.Move();
        }
    }
    
    // 转向（倒车时左右相反）
    if (isReverse) {
        handler.TurnLeft();  // reverse 时右转变成左转
    } else {
        handler.TurnRight();
    }
}

void ExecutorImpl::HandleFast(PoseHandler& handler)
{
    handler.Fast();
}

void ExecutorImpl::HandleReverse(PoseHandler& handler)
{
    handler.Reverse();
}

void ExecutorImpl::InitializeNormalCarCommands()
{
    // 普通车使用原始的命令类
    commandTable.clear();
    commandTable['M'] = MoveCommand();
    commandTable['L'] = TurnLeftCommand();
    commandTable['R'] = TurnRightCommand();
    commandTable['F'] = FastCommand();
    commandTable['B'] = ReverseCommand();
}

void ExecutorImpl::InitializeSportsCarCommands()
{
    // 跑车使用自定义的处理函数
    commandTable.clear();
    
    // 使用lambda表达式绑定成员函数
    commandTable['M'] = [this](PoseHandler& handler) { this->HandleMove(handler); };
    commandTable['L'] = [this](PoseHandler& handler) { this->HandleTurnLeft(handler); };
    commandTable['R'] = [this](PoseHandler& handler) { this->HandleTurnRight(handler); };
    commandTable['F'] = [this](PoseHandler& handler) { this->HandleFast(handler); };
    commandTable['B'] = [this](PoseHandler& handler) { this->HandleReverse(handler); };
}

void ExecutorImpl::InitializeBusCommands()
{
    // Bus使用自定义的处理函数
    commandTable.clear();
    
    // 使用lambda表达式绑定成员函数
    commandTable['M'] = [this](PoseHandler& handler) { this->HandleBusMove(handler); };
    commandTable['L'] = [this](PoseHandler& handler) { this->HandleBusTurnLeft(handler); };
    commandTable['R'] = [this](PoseHandler& handler) { this->HandleBusTurnRight(handler); };
    commandTable['F'] = [this](PoseHandler& handler) { this->HandleFast(handler); };
    commandTable['B'] = [this](PoseHandler& handler) { this->HandleReverse(handler); };
}

Pose ExecutorImpl::Query(void) const noexcept
{
    return posehandler.Query();
}
}  // namespace adas