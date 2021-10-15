#include <gtest/gtest.h>

#include "../src/LibSlg/Interpreter.hpp"

using ::testing::internal::CaptureStdout;
using ::testing::internal::GetCapturedStdout;

TEST(InterpreterTest, SimpleExpression) {
	CaptureStdout();
	Slanguage::Interpreter::execute("print 1;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "1\n");
}

TEST(InterpreterTest, VarDeclAndAssignment) {
	CaptureStdout();
	Slanguage::Interpreter::execute("var a: Number = 1; print a;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "1\n");

	CaptureStdout();
	Slanguage::Interpreter::execute("var b: Number; print b;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "nothing\n");

	CaptureStdout();
	Slanguage::Interpreter::execute("b = 1; print b;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "1\n");

	EXPECT_ANY_THROW(Slanguage::Interpreter::execute("print notDeclared;", false, true));

	EXPECT_ANY_THROW(Slanguage::Interpreter::execute("var alreadyDeclared: Number; var alreadyDeclared: Number = 1;",
			false,
			true));

	// immutable vals
	EXPECT_ANY_THROW(Slanguage::Interpreter::execute("val immutable: Number = 1; immutable = 2;", false, true));
	EXPECT_ANY_THROW(Slanguage::Interpreter::execute("val immutableTwo: Number; immutableTwo = 2;", false, true));
}

TEST(InterpreterTest, SimpleBooleanComparison) {
	CaptureStdout();
	Slanguage::Interpreter::execute("print true == false;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");

	CaptureStdout();
	Slanguage::Interpreter::execute("print true < false;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");

	CaptureStdout();
	Slanguage::Interpreter::execute("print false < true;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");

	CaptureStdout();
	Slanguage::Interpreter::execute("print true < true;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");

	CaptureStdout();
	Slanguage::Interpreter::execute("print true <= true;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");
}

TEST(InterpreterTest, ArithmeticPrecedence) {
	CaptureStdout();
	Slanguage::Interpreter::execute("print 1 + 2 * 3;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "7\n");

	CaptureStdout();
	Slanguage::Interpreter::execute("print (1 + 2) * 3;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "9\n");

	CaptureStdout();
	Slanguage::Interpreter::execute("print -(1 + 2) * 3;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "-9\n");
}

TEST(InterpreterTest, Comparisons) {
	CaptureStdout();
	Slanguage::Interpreter::execute("print nothing == nothing;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");

	CaptureStdout();
	Slanguage::Interpreter::execute("print nothing == !nothing;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");

	CaptureStdout();
	Slanguage::Interpreter::execute("print 1 < 2;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");

	CaptureStdout();
	Slanguage::Interpreter::execute("print 1 <= 1;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");

	CaptureStdout();
	Slanguage::Interpreter::execute("print !(1 <= 1);");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");

	CaptureStdout();
	Slanguage::Interpreter::execute(R"(print "a" <= "a";)");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");

	CaptureStdout();
	Slanguage::Interpreter::execute(R"(print "a" < "a";)");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");
}

TEST(InterpreterTest, StringConcat) {
	CaptureStdout();
	Slanguage::Interpreter::execute(R"(print "first" + " " + "second";)");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "first second\n");

	CaptureStdout();
	Slanguage::Interpreter::execute(R"(print "ten times: " + "lol" * 10;)");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "ten times: lollollollollollollollollollol\n");

	CaptureStdout();
	Slanguage::Interpreter::execute(R"(print "zero times: " + "lol" * -10;)");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "zero times: \n");
}

TEST(InterpreterTest, BlockScope) {
	EXPECT_ANY_THROW(Slanguage::Interpreter::execute("var z: Number; var z: String;", false, true));
	EXPECT_NO_THROW(Slanguage::Interpreter::execute("var x: Number; {var x: Number;}", false, true));

	CaptureStdout();
	Slanguage::Interpreter::execute("var y: Number = 420; {print y;}");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "420\n");
}

TEST(InterpreterTest, Functions) {
	// Check Arity
	EXPECT_ANY_THROW(
			Slanguage::Interpreter::execute("fun(a: Number, b: Number)->Number{return a + b;}();", false, true));
	EXPECT_NO_THROW(
			Slanguage::Interpreter::execute("fun(a: Number, b: Number)->Number{return a + b;}(2, 2);", false, true));

	// Simple function exec
	CaptureStdout();
	Slanguage::Interpreter::execute("fun(a: String)->Nothing{print a;}(\"Hi\");");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "Hi\n");

	// Closures
	CaptureStdout();
	Slanguage::Interpreter::execute(
			"var closure: Function()->Number= fun(a: "
			"Number)->Function()->Number{return fun()->Number{return a;};}(10); print closure();");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "10\n");
}