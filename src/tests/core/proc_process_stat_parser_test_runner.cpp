#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <sstream>

#include <os/stat/parser/proc_process_stat_parser.hpp>

namespace Tests
{
    namespace Core
    {
        TEST(ProcProcessStatParserTestSuite, WhenProcProcessStatStreamCorreclyParsedThenSuccess)
        {
            using OS::Stat::Model::ProcProcessStat;
            using OS::Stat::Parser::ProcProcessStatParser;

            //  Arrage
            std::string test_data = "21953 (system explorer) R 11879 21953 11879 34819 21953 4194304 91 0 2 0 0 0 0 0 20 0 1 0 9019927 227045376 384 18446744073709551615 94022324645888 94022324662561 140734852468400 0 0 0 0 0 0 0 0 0 17 3 0 0 0 0 0 94022324677264 94022324678760 94022344351744 140734852473747 140734852473767 140734852473767 140734852476907 0";
            std::istringstream stream(test_data);

            ProcProcessStatParser sut;

            //  Act
            ProcProcessStat actual = sut.Parse(stream);

            //  Assert
            EXPECT_EQ(21953, actual.pid);
            EXPECT_EQ("(system explorer)", actual.comm);
            EXPECT_EQ('R', actual.state);
            EXPECT_EQ(11879, actual.ppid);
            EXPECT_EQ(21953, actual.pgrp);
            EXPECT_EQ(11879, actual.session);
            EXPECT_EQ(34819, actual.tty_nr);
            EXPECT_EQ(21953, actual.tpgid);
            EXPECT_EQ(4194304U, actual.flags);
            EXPECT_EQ(91UL, actual.minflt);
            EXPECT_EQ(0UL, actual.cminflt);
            EXPECT_EQ(2UL, actual.majflt);
            EXPECT_EQ(0UL, actual.cmajflt);
            EXPECT_EQ(0UL, actual.utime);
            EXPECT_EQ(0UL, actual.stime);
            EXPECT_EQ(0L, actual.cutime);
            EXPECT_EQ(0L, actual.cstime);
            EXPECT_EQ(20L, actual.priority);
            EXPECT_EQ(0L, actual.nice);
            EXPECT_EQ(1L, actual.num_threads);
            EXPECT_EQ(0L, actual.itrvalue);
            EXPECT_EQ(9019927UL, actual.starttime);
            EXPECT_EQ(227045376UL, actual.vsize);
            EXPECT_EQ(384L, actual.rss);
            EXPECT_EQ(18446744073709551615UL, actual.rsslim);
            EXPECT_EQ(94022324645888UL, actual.startcode);
            EXPECT_EQ(94022324662561UL, actual.endcode);
            EXPECT_EQ(140734852468400UL, actual.startstack);
            EXPECT_EQ(0UL, actual.kstesp);
            EXPECT_EQ(0UL, actual.ksteip);
            EXPECT_EQ(0UL, actual.signal);
            EXPECT_EQ(0UL, actual.blocked);
            EXPECT_EQ(0UL, actual.sigignore);
            EXPECT_EQ(0UL, actual.sigcatch);
            EXPECT_EQ(0UL, actual.wchan);
            EXPECT_EQ(0UL, actual.snwap);
            EXPECT_EQ(0UL, actual.cnswap);
            EXPECT_EQ(17, actual.exit_signal);
            EXPECT_EQ(3, actual.processor);
            EXPECT_EQ(0U, actual.rt_priority);
            EXPECT_EQ(0U, actual.policy);
            EXPECT_EQ(0UL, actual.delayacct_blkio_ticks);
            EXPECT_EQ(0UL, actual.guest_time);
            EXPECT_EQ(0L, actual.cguest_time);
            EXPECT_EQ(94022324677264UL, actual.start_data);
            EXPECT_EQ(94022324678760UL, actual.end_data);
            EXPECT_EQ(94022344351744UL, actual.stack_brk);
            EXPECT_EQ(140734852473747UL, actual.arg_start);
            EXPECT_EQ(140734852473767UL, actual.arg_end);
            EXPECT_EQ(140734852473767UL, actual.env_start);
            EXPECT_EQ(140734852476907UL, actual.env_end);
            EXPECT_EQ(0, actual.exit_code);
 

        }

    }
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}