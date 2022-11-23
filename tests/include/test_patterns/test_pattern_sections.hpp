#pragma once

#include "test_pattern.hpp"

namespace pl::test {

    class TestPatternSections : public TestPattern {
    public:
        TestPatternSections() : TestPattern("Sections") {
        }
        ~TestPatternSections() override = default;

        [[nodiscard]] std::string getSourceCode() const override {
            return R"(
#include <std/mem.pat>
#include <std/io.pat>
#include <std/core.pat>
#include <std/sys.pat>


std::mem::Section fSection = std::mem::create_section("f.stream");

u32 raw[1] @0 in fSection;
raw[0] = u32(-1);

bitfield bf
{
LSB : 8;
Mid : 8;
Top : 8;
MSB : 8;
};

bf tst @0 in fSection;
std::print("{:X}", tst.MSB);
std::print("{:X}", tst.Top);
std::print("{:X}", tst.Mid);
std::print("{:X}", tst.LSB);

std::assert(tst.MSB == 0xFF, "Bit error");
std::assert(tst.Top == 0xFF, "Bit error");
std::assert(tst.Mid == 0xFF, "Bit error");
std::assert(tst.LSB == 0xFF, "Bit error");

            )";
        }
    };

}