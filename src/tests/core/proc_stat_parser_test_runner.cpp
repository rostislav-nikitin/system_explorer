#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <sstream>

#include <os/stat/parser/proc_stat_parser.hpp>

namespace Tests
{
    namespace Core
    {
        TEST(ProcStatParserTestSuite, WhenProcStatStreamCorreclyParsedThenSuccess)
        {
            using OS::Stat::Model::ProcStat;
            using OS::Stat::Parser::ProcStatParser;

            //  Arrage
            std::string test_data =
	            "cpu  521660 236 150535 3206122 10880 28893 10547 0 0 0\n"
	            "cpu0 122577 79 37017 1356568 4299 13391 4596 0 0 0\n"
                "cpu1 137727 64 37684 612728 2174 5037 1950 0 0 0\n"
                "cpu2 132222 50 38139 616286 2337 5625 2131 0 0 0\n"
                "cpu3 129133 42 37693 620539 2069 4839 1868 0 0 0\n"
                "intr 20262014\n";

            std::istringstream stream(test_data);

            ProcStatParser sut;

            //  Act
            ProcStat actual = sut.Parse(stream);

            //  Assert
            int expected_cpus_stat_count = 1 + 4;
            EXPECT_EQ(expected_cpus_stat_count, actual.proc_cpu_stat.size());

            // Whole CPU (all cores) stat
            EXPECT_EQ(521660, actual.proc_cpu_stat[0].user);
            EXPECT_EQ(236, actual.proc_cpu_stat[0].nice);
            EXPECT_EQ(150535, actual.proc_cpu_stat[0].system);
            EXPECT_EQ(3206122, actual.proc_cpu_stat[0].idle);
            EXPECT_EQ(10880, actual.proc_cpu_stat[0].iowait);
            EXPECT_EQ(28893, actual.proc_cpu_stat[0].irq);
            EXPECT_EQ(10547, actual.proc_cpu_stat[0].softirq);
            EXPECT_EQ(0, actual.proc_cpu_stat[0].steal);
            EXPECT_EQ(0, actual.proc_cpu_stat[0].guest);
            EXPECT_EQ(0, actual.proc_cpu_stat[0].guest_nice);

            // Core 0
            EXPECT_EQ(122577, actual.proc_cpu_stat[1].user);
            EXPECT_EQ(79, actual.proc_cpu_stat[1].nice);
            EXPECT_EQ(37017, actual.proc_cpu_stat[1].system);
            EXPECT_EQ(1356568, actual.proc_cpu_stat[1].idle);
            EXPECT_EQ(4299, actual.proc_cpu_stat[1].iowait);
            EXPECT_EQ(13391, actual.proc_cpu_stat[1].irq);
            EXPECT_EQ(4596, actual.proc_cpu_stat[1].softirq);
            EXPECT_EQ(0, actual.proc_cpu_stat[1].steal);
            EXPECT_EQ(0, actual.proc_cpu_stat[1].guest);
            EXPECT_EQ(0, actual.proc_cpu_stat[1].guest_nice);

            // Core 1
            EXPECT_EQ(137727, actual.proc_cpu_stat[2].user);
            EXPECT_EQ(64, actual.proc_cpu_stat[2].nice);
            EXPECT_EQ(37684, actual.proc_cpu_stat[2].system);
            EXPECT_EQ(612728, actual.proc_cpu_stat[2].idle);
            EXPECT_EQ(2174, actual.proc_cpu_stat[2].iowait);
            EXPECT_EQ(5037, actual.proc_cpu_stat[2].irq);
            EXPECT_EQ(1950, actual.proc_cpu_stat[2].softirq);
            EXPECT_EQ(0, actual.proc_cpu_stat[2].steal);
            EXPECT_EQ(0, actual.proc_cpu_stat[2].guest);
            EXPECT_EQ(0, actual.proc_cpu_stat[2].guest_nice);

            // Core 2
            EXPECT_EQ(132222, actual.proc_cpu_stat[3].user);
            EXPECT_EQ(50, actual.proc_cpu_stat[3].nice);
            EXPECT_EQ(38139, actual.proc_cpu_stat[3].system);
            EXPECT_EQ(616286, actual.proc_cpu_stat[3].idle);
            EXPECT_EQ(2337, actual.proc_cpu_stat[3].iowait);
            EXPECT_EQ(5625, actual.proc_cpu_stat[3].irq);
            EXPECT_EQ(2131, actual.proc_cpu_stat[3].softirq);
            EXPECT_EQ(0, actual.proc_cpu_stat[3].steal);
            EXPECT_EQ(0, actual.proc_cpu_stat[3].guest);
            EXPECT_EQ(0, actual.proc_cpu_stat[3].guest_nice);

            // Core 2
            EXPECT_EQ(129133, actual.proc_cpu_stat[4].user);
            EXPECT_EQ(42, actual.proc_cpu_stat[4].nice);
            EXPECT_EQ(37693, actual.proc_cpu_stat[4].system);
            EXPECT_EQ(620539, actual.proc_cpu_stat[4].idle);
            EXPECT_EQ(2069, actual.proc_cpu_stat[4].iowait);
            EXPECT_EQ(4839, actual.proc_cpu_stat[4].irq);
            EXPECT_EQ(1868, actual.proc_cpu_stat[4].softirq);
            EXPECT_EQ(0, actual.proc_cpu_stat[4].steal);
            EXPECT_EQ(0, actual.proc_cpu_stat[4].guest);
            EXPECT_EQ(0, actual.proc_cpu_stat[4].guest_nice);
        }

    }
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}