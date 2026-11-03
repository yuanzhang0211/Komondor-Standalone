#include <cassert>
#include <limits>
#include "list_of_macros.h"
#include "structures/ap_observation.h"
int main() {
    assert(ApJsonString("A\"\\\n\t") == "\"A\\\"\\\\\\u000a\\u0009\"");
    assert(ApJsonNumber(std::numeric_limits<double>::infinity()) == "null");
    assert(ApJsonNumber(-std::numeric_limits<double>::infinity()) == "null");
    assert(ApJsonNumber(std::numeric_limits<double>::quiet_NaN()) == "null");
    assert(ApJsonNumber(0.125) == "0.125");
    assert(std::string(ApStateName(STATE_WAIT_ICR)) == "STATE_WAIT_ICR");
    assert(std::string(ApStateName(-999)) == "STATE_UNRECOGNIZED");
    ApObservation disabled;
    disabled.write("unread");
    assert(!disabled.enabled() && disabled.sequence==0);
}
