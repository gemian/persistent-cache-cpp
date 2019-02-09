#
# Tests listed here will not be run by the valgrind target,
# either because there is not point (we don't want to
# test that a python script doesn't leak), or because,
# under valgrind, the test runs too slowly to meet
# its timing constraints (or crashes valgrind).
#

SET(CTEST_CUSTOM_MEMCHECK_IGNORE
    copyright
    speed
    stand-alone-core-headers
    stand-alone-core-internal-headers
    clean-public-core-headers
    whitespace
)
