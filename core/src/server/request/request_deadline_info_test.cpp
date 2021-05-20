#include <server/request/request_deadline_info.hpp>

#include <utest/utest.hpp>

TEST(RequestDeadlineInfo, SetGet) {
  RunInCoro([]() {
    auto deadline = engine::Deadline::FromDuration(std::chrono::seconds(2));
    auto start_time = std::chrono::steady_clock::now();
    server::request::RequestDeadlineInfo deadline_info(start_time, deadline);

    EXPECT_EQ(server::request::GetCurrentRequestDeadlineInfoUnchecked(),
              nullptr);
    SetCurrentRequestDeadlineInfo(deadline_info);

    const auto& stored_deadline_info =
        server::request::GetCurrentRequestDeadlineInfo();
    EXPECT_EQ(stored_deadline_info.GetStartTime(),
              deadline_info.GetStartTime());
    EXPECT_EQ(stored_deadline_info.GetDeadline(), deadline_info.GetDeadline());

    server::request::ResetCurrentRequestDeadlineInfo();
    EXPECT_EQ(server::request::GetCurrentRequestDeadlineInfoUnchecked(),
              nullptr);
  });
}

TEST(RequestDeadlineInfo, BaseTypeCast) {
  RunInCoro([]() {
    auto deadline = engine::Deadline::FromDuration(std::chrono::seconds(2));
    auto start_time = std::chrono::steady_clock::now();
    server::request::RequestDeadlineInfo deadline_info(start_time, deadline);

    EXPECT_EQ(server::request::GetCurrentRequestDeadlineInfoUnchecked(),
              nullptr);
    EXPECT_EQ(engine::GetCurrentTaskInheritedDeadlineUnchecked(), nullptr);
    SetCurrentRequestDeadlineInfo(deadline_info);

    const auto* stored_deadline_opt =
        engine::GetCurrentTaskInheritedDeadlineUnchecked();
    ASSERT_NE(stored_deadline_opt, nullptr);
    EXPECT_EQ(stored_deadline_opt->GetDeadline(), deadline_info.GetDeadline());

    const auto& stored_deadline_info =
        server::request::GetCurrentRequestDeadlineInfo();
    EXPECT_EQ(stored_deadline_info.GetStartTime(),
              deadline_info.GetStartTime());
    EXPECT_EQ(stored_deadline_info.GetDeadline(), deadline_info.GetDeadline());
  });
}