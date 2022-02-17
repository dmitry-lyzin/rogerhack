# CMakeList.txt: проект CMake для rogerhack; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("rogerhack")

# Добавьте источник в исполняемый файл этого проекта.
add_executable ("rogerhack" main.cpp BCDnum.h BitSet.h common.cpp common.h getopt.c getopt.h)

# Включить поддержку тестирования:
enable_testing()

set(MULTIPLY_TIMEOUT 1)
set(testfiles ${CMAKE_CURRENT_SOURCE_DIR}/testfiles)

#--------------------------------------
#add_test(Unknown_option rogerhack -x)
#set_tests_properties(Unknown_option PROPERTIES
#	PASS_REGULAR_EXPRESSION "Unknown option -x")

# CMakeFiles - это папка
add_test(write_to_file_error1 rogerhack -p CMakeFiles)
set_tests_properties(write_to_file_error1 PROPERTIES
	 WILL_FAIL TRUE
)
add_test(write_to_file_error2 rogerhack -s CMakeFiles)
set_tests_properties(write_to_file_error2 PROPERTIES
	WILL_FAIL TRUE
)

#--------------------------------------
# 
#
add_test(empty_sin_save rogerhack -s 1.sin)
add_test(empty_sin_correct diff 1.sin ${testfiles}/ROGER_KP-23_empty.sin)
set_tests_properties(empty_sin_correct PROPERTIES
	DEPENDS empty_sin_save
	REQUIRED_FILES ${testfiles}/ROGER_KP-23_empty.sin
)


#--------------------------------------
# load-print-read-save diff
#
set(testfile "test1")

add_test(load_print rogerhack -l ${testfiles}/${testfile}.sin -p ${testfile}.txt)
set_tests_properties(load_print PROPERTIES
	REQUIRED_FILES ${testfiles}/${testfile}.sin
)
add_test(test_printed diff ${testfiles}/${testfile}.txt ${testfile}.txt)
set_tests_properties(test_printed PROPERTIES
	DEPENDS load_print
	REQUIRED_FILES ${testfiles}/${testfile}.txt
)
add_test(read_save rogerhack -r ${testfile}.txt -s ${testfile}.sin)
set_tests_properties(read_save PROPERTIES
	DEPENDS test_printed
	REQUIRED_FILES ${testfile}.txt
)
add_test(load_print_read_save_diff diff ${testfiles}/${testfile}.sin ${testfile}.sin)
set_tests_properties(load_print_read_save_diff PROPERTIES
	DEPENDS read_save
)

#--------------------------------------
# тест загрузки из текст
#
add_test(bed_text_1 rogerhack -r ${testfiles}/bed1.txt)
set_tests_properties(bed_text_1 PROPERTIES
	REQUIRED_FILES ${testfiles}/bed1.txt
	WILL_FAIL TRUE
)

add_test(bed_text_2 rogerhack -r ${testfiles}/bed2.txt)
set_tests_properties(bed_text_2 PROPERTIES
	REQUIRED_FILES ${testfiles}/bed2.txt
	WILL_FAIL TRUE
)

add_test(bed_text_3 rogerhack -r ${testfiles}/bed3.txt)
set_tests_properties(bed_text_3 PROPERTIES
	REQUIRED_FILES ${testfiles}/bed3.txt
	WILL_FAIL TRUE
)
