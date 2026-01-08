#include <gtest/gtest.h>
#include <memory>
#include "Executor.hpp"
#include "PoseEq.hpp"

namespace adas
{
// 测试套：公共汽车基本功能
TEST(ExecutorBusTest, should_switch_to_bus_with_U_command)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("U");  // 切换到公共汽车

    // then
    // 切换车辆类型后，初始状态应该保持不变
    const Pose target{0, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

// Bus Normal状态测试
TEST(ExecutorBusTest, bus_normal_M_should_move_1_block_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("UM");  // 切换到Bus并前进

    // then
    const Pose target{1, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorBusTest, bus_normal_M_should_move_1_block_when_facing_N)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));

    // when
    executor->Execute("UM");  // 切换到Bus并前进

    // then
    const Pose target{0, 1, 'N'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorBusTest, bus_normal_M_should_move_1_block_when_facing_W)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'W'}));

    // when
    executor->Execute("UM");  // 切换到Bus并前进

    // then
    const Pose target{-1, 0, 'W'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorBusTest, bus_normal_M_should_move_1_block_when_facing_S)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'S'}));

    // when
    executor->Execute("UM");  // 切换到Bus并前进

    // then
    const Pose target{0, -1, 'S'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorBusTest, bus_normal_L_should_move_1_block_and_turn_left_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("UL");  // 切换到Bus并左转前进

    // then
    // 前进1格：从(0,0)到(1,0)，然后左转，方向从E变为N
    const Pose target{1, 0, 'N'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorBusTest, bus_normal_R_should_move_1_block_and_turn_right_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("UR");  // 切换到Bus并右转前进

    // then
    // 前进1格：从(0,0)到(1,0)，然后右转，方向从E变为S
    const Pose target{1, 0, 'S'};
    ASSERT_EQ(target, executor->Query());
}

// Bus B状态测试（倒车状态）
TEST(ExecutorBusTest, bus_B_M_should_move_backward_1_block_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("UBM");  // 切换到Bus，进入倒车状态，后退

    // then
    // 后退1格：从(0,0)到(-1,0)
    const Pose target{-1, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorBusTest, bus_B_L_should_move_backward_1_block_and_turn_right_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("UBL");  // 切换到Bus，进入倒车状态，左转（实际右转）

    // then
    // 后退1格：从(0,0)到(-1,0)，然后右转（因为倒车状态左转变成右转），方向从E变为S
    const Pose target{-1, 0, 'S'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorBusTest, bus_B_R_should_move_backward_1_block_and_turn_left_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("UBR");  // 切换到Bus，进入倒车状态，右转（实际左转）

    // then
    // 后退1格：从(0,0)到(-1,0)，然后左转（因为倒车状态右转变成左转），方向从E变为N
    const Pose target{-1, 0, 'N'};
    ASSERT_EQ(target, executor->Query());
}

// Bus F状态测试（加速状态）
TEST(ExecutorBusTest, bus_F_M_should_move_2_blocks_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("UFM");  // 切换到Bus，进入加速状态，前进

    // then
    // 前进1格，再前进1格：从(0,0)到(2,0)
    const Pose target{2, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorBusTest, bus_F_L_should_move_2_blocks_and_turn_left_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("UFL");  // 切换到Bus，进入加速状态，左转

    // then
    // 前进1格到(1,0)，再前进1格到(2,0)，然后左转，方向从E变为N
    const Pose target{2, 0, 'N'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorBusTest, bus_F_R_should_move_2_blocks_and_turn_right_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("UFR");  // 切换到Bus，进入加速状态，右转

    // then
    // 前进1格到(1,0)，再前进1格到(2,0)，然后右转，方向从E变为S
    const Pose target{2, 0, 'S'};
    ASSERT_EQ(target, executor->Query());
}

// Bus B&F状态测试（加速+倒车状态）
TEST(ExecutorBusTest, bus_BF_M_should_move_backward_2_blocks_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("UFBM");  // 切换到Bus，进入加速和倒车状态，后退

    // then
    // 后退1格，再后退1格：从(0,0)到(-2,0)
    const Pose target{-2, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorBusTest, bus_BF_L_should_move_backward_2_blocks_and_turn_right_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("UFBL");  // 切换到Bus，进入加速和倒车状态，左转（实际右转）

    // then
    // 后退1格到(-1,0)，再后退1格到(-2,0)，然后右转（倒车状态左转变成右转），方向从E变为S
    const Pose target{-2, 0, 'S'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorBusTest, bus_BF_R_should_move_backward_2_blocks_and_turn_left_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("UFBR");  // 切换到Bus，进入加速和倒车状态，右转（实际左转）

    // then
    // 后退1格到(-1,0)，再后退1格到(-2,0)，然后左转（倒车状态右转变成左转），方向从E变为N
    const Pose target{-2, 0, 'N'};
    ASSERT_EQ(target, executor->Query());
}

// 综合测试：切换回普通车
TEST(ExecutorBusTest, should_switch_back_to_normal_car_with_second_U_command)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("UUM");  // 切换到Bus，再切换回普通车，前进

    // then
    // 普通车前进1格
    const Pose target{1, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

// Bus复杂路径测试
TEST(ExecutorBusTest, bus_complex_path_test)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("UMLR");  // 切换到Bus，前进1格，左转前进1格，右转前进1格

    // then
    // M: 前进1格 → (1,0,'E')
    // L: 前进1格到(2,0,'E')，左转到(2,0,'N')
    // R: 前进1格到(2,1,'N')，右转到(2,1,'E')
    const Pose target{2, 1, 'E'};
    ASSERT_EQ(target, executor->Query());
}

// Bus与普通车对比测试
TEST(ExecutorBusTest, compare_bus_and_normal_car_M_command)
{
    // given
    std::unique_ptr<Executor> normalCar(Executor::NewExecutor({0, 0, 'E'}));
    std::unique_ptr<Executor> bus(Executor::NewExecutor({0, 0, 'E'}));

    // when
    normalCar->Execute("M");   // 普通车前进1格
    bus->Execute("UM");        // Bus前进1格

    // then
    const Pose normalTarget{1, 0, 'E'};
    const Pose busTarget{1, 0, 'E'};
    ASSERT_EQ(normalTarget, normalCar->Query());
    ASSERT_EQ(busTarget, bus->Query());
}

// Bus与跑车对比测试
TEST(ExecutorBusTest, compare_bus_and_sports_car_M_command)
{
    // given
    std::unique_ptr<Executor> sportsCar(Executor::NewExecutor({0, 0, 'E'}));
    std::unique_ptr<Executor> bus(Executor::NewExecutor({0, 0, 'E'}));

    // when
    sportsCar->Execute("NM");  // 跑车前进2格
    bus->Execute("UM");        // Bus前进1格

    // then
    const Pose sportsTarget{2, 0, 'E'};
    const Pose busTarget{1, 0, 'E'};
    ASSERT_EQ(sportsTarget, sportsCar->Query());
    ASSERT_EQ(busTarget, bus->Query());
}

// Bus在加速状态下连续转向测试
TEST(ExecutorBusTest, bus_F_LR_combined_test)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("UFLR");  // 切换到Bus，加速状态，左转，右转

    // then
    // FL: 前进2格到(2,0,'E')，左转到(2,0,'N')
    // R: 前进2格到(2,2,'N')，右转到(2,2,'E')
    const Pose target{2, 2, 'E'};
    ASSERT_EQ(target, executor->Query());
}

// Bus在不同方向上的综合测试
TEST(ExecutorBusTest, bus_multi_direction_test)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));

    // when
    executor->Execute("UMRB");  // 切换到Bus，前进1格，右转前进1格，进入倒车状态

    // then
    // M: 向北前进1格 → (0,1,'N')
    // R: 前进1格到(0,2,'N')，右转到(0,2,'E')
    // B: 切换倒车状态（位置方向不变）
    const Pose target{0, 2, 'E'};
    ASSERT_EQ(target, executor->Query());
    
    // 验证倒车状态已激活
    // 注意：这里假设我们可以查询状态，但当前接口不支持，所以只验证位置方向
}

}  // namespace adas