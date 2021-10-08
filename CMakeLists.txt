# CMakeList.txt: проект CMake для rogerhack; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("rogerhack")

# Добавьте источник в исполняемый файл этого проекта.
add_executable ("rogerhack" main.cpp binary.h BitField.cpp BitField.h Channel.cpp Channel.h common.cpp common.h Flags.cpp Flags.h Frequency.cpp Frequency.h Sin.cpp Sin.h Tone.cpp Tone.h getopt.c getopt.h)

# Включить поддержку тестирования:
enable_testing()

# Добавить тесты:
add_test(FirstTest rogerhack -x)
#add_test(SecondTest rogerhack -54 -785)
#add_test(ThirdTest rogerhack 85234)

set(MULTIPLY_TIMEOUT 1)

# Установить поведение тестов:
#set_tests_properties(FirstTest SecondTest ThirdTest
set_tests_properties(FirstTest
PROPERTIES TIMEOUT ${MULTIPLY_TIMEOUT})

set_tests_properties(FirstTest PROPERTIES
PASS_REGULAR_EXPRESSION "Unknown option -x"
FAIL_REGULAR_EXPRESSION "Error!")
