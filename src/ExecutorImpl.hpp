#pragma once
#include "Executor.hpp"
#include "PoseHandler.hpp"
#include <unordered_map>
#include <functional>

namespace adas
{
enum class CarType {
    NORMAL,
    SPORTS,
    BUS  // 新增Bus类型
};

// 车辆配置结构
struct CarConfig {
    int normalMoveDistance;     // 普通移动距离
    int fastMoveDistance;       // 加速移动距离
    bool turnWithMove;          // 转向时是否伴随移动
    bool complexTurnRound;      // 是否执行复杂的掉头逻辑
    
    // 添加构造函数以支持初始化
    CarConfig(int normalDist, int fastDist, bool turnMove, bool complexTurn)
        : normalMoveDistance(normalDist), fastMoveDistance(fastDist), 
          turnWithMove(turnMove), complexTurnRound(complexTurn) {}
};

class ExecutorImpl final : public Executor
{
public:
    explicit ExecutorImpl(const Pose& pose) noexcept;
    ~ExecutorImpl() noexcept = default;

    ExecutorImpl(const ExecutorImpl&) = delete;
    ExecutorImpl& operator=(const ExecutorImpl&) = delete;

public:
    void Execute(const std::string& command) noexcept override;
    Pose Query(void) const noexcept override;

private:
    using CommandHandler = std::function<void(PoseHandler&)>;
    
    // 初始化不同车辆类型的命令表
    void InitializeNormalCarCommands();
    void InitializeSportsCarCommands();
    void InitializeBusCommands();  // 新增Bus命令表初始化
    
    // 车辆类型切换
    void SwitchCarType();
    void SwitchToBus();      // 新增：切换到Bus
    void SwitchToNormal();   // 新增：从Bus切换回普通车
    
    // 获取当前车辆的移动距离
    int GetMoveDistance() const;
    
    // 命令处理辅助函数
    void HandleMove(PoseHandler& handler);
    void HandleTurnLeft(PoseHandler& handler);
    void HandleTurnRight(PoseHandler& handler);
    void HandleFast(PoseHandler& handler);
    void HandleReverse(PoseHandler& handler);
    
    // Bus特有命令处理函数
    void HandleBusMove(PoseHandler& handler);
    void HandleBusTurnLeft(PoseHandler& handler);
    void HandleBusTurnRight(PoseHandler& handler);
    
    // 按照初始化顺序调整声明顺序
    PoseHandler posehandler;               // 第一：需要在构造函数中初始化
    const CarConfig* currentConfig;        // 第二：依赖于posehandler
    std::unordered_map<char, CommandHandler> commandTable; // 第三：默认初始化
    CarType carType{CarType::NORMAL};      // 第四：有默认值
    
    // 车辆配置表
    static const CarConfig normalCarConfig;
    static const CarConfig sportsCarConfig;
    static const CarConfig busCarConfig;   // 新增Bus配置
};
}  // namespace adas