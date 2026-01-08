#include <gtest/gtest.h>
#include <memory>
#include "Executor.hpp"
#include "PoseEq.hpp"

namespace adas
{
// 测试套：跑车基本功能
TEST(ExecutorSportsCarTest, should_switch_to_sports_car_with_N_command)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("N");

    // then
    // 切换车辆类型后，初始状态应该保持不变
    const Pose target{0, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

// 跑车Normal状态测试
TEST(ExecutorSportsCarTest, sports_car_normal_M_should_move_2_blocks_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("NM");  // 切换到跑车并前进

    // then
    const Pose target{2, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorSportsCarTest, sports_car_normal_M_should_move_2_blocks_when_facing_N)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));

    // when
    executor->Execute("NM");  // 切换到跑车并前进

    // then
    const Pose target{0, 2, 'N'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorSportsCarTest, sports_car_normal_L_should_turn_left_and_move_1_block_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("NL");  // 切换到跑车并左转前进

    // then
    const Pose target{0, 1, 'N'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorSportsCarTest, sports_car_normal_R_should_turn_right_and_move_1_block_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("NR");  // 切换到跑车并右转前进

    // then
    const Pose target{0, -1, 'S'};
    ASSERT_EQ(target, executor->Query());
}

// 跑车B状态测试
TEST(ExecutorSportsCarTest, sports_car_B_M_should_move_backward_2_blocks_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("NBM");  // 切换到跑车，进入倒车状态，后退

    // then
    const Pose target{-2, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorSportsCarTest, sports_car_B_L_should_turn_right_and_move_backward_1_block_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("NBL");  // 切换到跑车，进入倒车状态，左转(实际右转)

    // then
    const Pose target{0, 1, 'S'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorSportsCarTest, sports_car_B_R_should_turn_left_and_move_backward_1_block_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("NBR");  // 切换到跑车，进入倒车状态，右转(实际左转)

    // then
    const Pose target{0, -1, 'N'};
    ASSERT_EQ(target, executor->Query());
}

// 跑车F状态测试
TEST(ExecutorSportsCarTest, sports_car_F_M_should_move_4_blocks_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("NFM");  // 切换到跑车，进入加速状态，前进

    // then
    const Pose target{4, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorSportsCarTest, sports_car_F_L_should_move_then_left_then_move_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("NFL");  // 切换到跑车，进入加速状态，左转

    // then
    const Pose target{1, 1, 'N'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorSportsCarTest, sports_car_F_R_should_move_then_right_then_move_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("NFR");  // 切换到跑车，进入加速状态，右转

    // then
    const Pose target{1, -1, 'S'};
    ASSERT_EQ(target, executor->Query());
}

// 跑车B&F状态测试
TEST(ExecutorSportsCarTest, sports_car_BF_M_should_move_backward_4_blocks_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("NFBM");  // 切换到跑车，进入加速和倒车状态，后退

    // then
    const Pose target{-4, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorSportsCarTest, sports_car_BF_L_should_move_backward_then_right_then_backward_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("NFBL");  // 切换到跑车，进入加速和倒车状态，左转(实际后退+右转+后退)

    // then
    const Pose target{-1, 1, 'S'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorSportsCarTest, sports_car_BF_R_should_move_backward_then_left_then_backward_when_facing_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("NFBR");  // 切换到跑车，进入加速和倒车状态，右转(实际后退+左转+后退)

    // then
    const Pose target{-1, -1, 'N'};
    ASSERT_EQ(target, executor->Query());
}

// 综合测试：切换回普通车
TEST(ExecutorSportsCarTest, should_switch_back_to_normal_car_with_second_N_command)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("NNM");  // 切换到跑车，再切换回普通车，前进

    // then
    // 普通车前进1格
    const Pose target{1, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

// 跑车复杂路径测试
TEST(ExecutorSportsCarTest, sports_car_complex_path_test)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));

    // when
    executor->Execute("NMLR");  // 切换到跑车，前进2格，左转前进1格，右转前进1格

    // then
    const Pose target{3, 1, 'E'};  // 需要根据实际计算结果调整
    ASSERT_EQ(target, executor->Query());
}

// 跑车与普通车对比测试
TEST(ExecutorSportsCarTest, compare_sports_car_and_normal_car_M_command)
{
    // given
    std::unique_ptr<Executor> normalCar(Executor::NewExecutor({0, 0, 'E'}));
    std::unique_ptr<Executor> sportsCar(Executor::NewExecutor({0, 0, 'E'}));

    // when
    normalCar->Execute("M");   // 普通车前进1格
    sportsCar->Execute("NM");  // 跑车前进2格

    // then
    const Pose normalTarget{1, 0, 'E'};
    const Pose sportsTarget{2, 0, 'E'};
    ASSERT_EQ(normalTarget, normalCar->Query());
    ASSERT_EQ(sportsTarget, sportsCar->Query());
}

}  // namespace adas