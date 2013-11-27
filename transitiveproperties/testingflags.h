/** \file testingflags.h
 *  @brief Constant testing flags, used to enable or disable certain test functionality/output.
 *  @author Andrew Ring
 */

#ifndef TESTINGFLAGS_H
#define TESTINGFLAGS_H

/**
 * @brief A collection of flags used to turn on and off certain tests/debug output.
 * @author Andrew Ring
 */
namespace testingFlags {
    static const bool TEST_MAP_CREATOR = false;
    static const bool TEST_MAP_EDITOR = false;
    static const bool TEST_COLOR_TERRITORIES_RANDOMLY = false;
    static const bool TEST_HIGHLIGHT_CYCLE = false;
    static const bool TEST_CALC_RUNTIME = true;
}

#endif // TESTINGFLAGS_H
