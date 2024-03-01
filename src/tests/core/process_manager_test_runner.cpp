#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../core/process_manager.hpp"

namespace Tests
{
    namespace Core
    {
        TEST(ProcessManagerTestSuite, WhenProcPidStatValidThenSuccess)
        {
            using SystemExplorer::Core::ProcessManager;

            ProcessManager sut;

            ASSERT_TRUE(true);
        }

    }
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}