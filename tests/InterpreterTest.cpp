#include <gtest/gtest.h>

#include <Interpreter.hpp>

using ::testing::internal::CaptureStdout;
using ::testing::internal::GetCapturedStdout;

TEST(InterpreterTest, SimpleExpression) {
	CaptureStdout();
	Kyra::Interpreter::execute("print 1;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "1\n");
}

TEST(InterpreterTest, VarDeclAndAssignment) {
	CaptureStdout();
	Kyra::Interpreter::execute("var a: Number = 1; print a;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "1\n");

	CaptureStdout();
	Kyra::Interpreter::execute("var b: Number; print b;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "nothing\n");

	CaptureStdout();
	Kyra::Interpreter::execute("b = 1; print b;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "1\n");

	EXPECT_ANY_THROW(Kyra::Interpreter::execute("print notDeclared;", false, true));

	EXPECT_ANY_THROW(
			Kyra::Interpreter::execute("var alreadyDeclared: Number; var alreadyDeclared: Number = 1;", false, true));

	// immutable vals
	EXPECT_ANY_THROW(Kyra::Interpreter::execute("val immutable: Number = 1; immutable = 2;", false, true));
	EXPECT_ANY_THROW(Kyra::Interpreter::execute("val immutableTwo: Number; immutableTwo = 2;", false, true));
}

TEST(InterpreterTest, SimpleBooleanComparison) {
	CaptureStdout();
	Kyra::Interpreter::execute("print true == false;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");

	CaptureStdout();
	Kyra::Interpreter::execute("print true < false;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");

	CaptureStdout();
	Kyra::Interpreter::execute("print false < true;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");

	CaptureStdout();
	Kyra::Interpreter::execute("print true < true;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");

	CaptureStdout();
	Kyra::Interpreter::execute("print true <= true;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");
}

TEST(InterpreterTest, ArithmeticPrecedence) {
	CaptureStdout();
	Kyra::Interpreter::execute("print 1 + 2 * 3;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "7\n");

	CaptureStdout();
	Kyra::Interpreter::execute("print (1 + 2) * 3;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "9\n");

	CaptureStdout();
	Kyra::Interpreter::execute("print -(1 + 2) * 3;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "-9\n");
}

TEST(InterpreterTest, Comparisons) {
	CaptureStdout();
	Kyra::Interpreter::execute("print nothing == nothing;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");

	CaptureStdout();
	Kyra::Interpreter::execute("print nothing == !nothing;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");

	CaptureStdout();
	Kyra::Interpreter::execute("print 1 < 2;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");

	CaptureStdout();
	Kyra::Interpreter::execute("print 1 <= 1;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");

	CaptureStdout();
	Kyra::Interpreter::execute("print !(1 <= 1);");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");

	CaptureStdout();
	Kyra::Interpreter::execute(R"(print "a" <= "a";)");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");

	CaptureStdout();
	Kyra::Interpreter::execute(R"(print "a" < "a";)");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");
}

TEST(InterpreterTest, StringConcat) {
	CaptureStdout();
	Kyra::Interpreter::execute(R"(print "first" + " " + "second";)");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "first second\n");

	CaptureStdout();
	Kyra::Interpreter::execute(R"(print "ten times: " + "lol" * 10;)");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "ten times: lollollollollollollollollollol\n");

	CaptureStdout();
	Kyra::Interpreter::execute(R"(print "zero times: " + "lol" * -10;)");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "zero times: \n");
}

TEST(InterpreterTest, BlockScope) {
	EXPECT_ANY_THROW(Kyra::Interpreter::execute("var z: Number; var z: String;", false, true));
	EXPECT_NO_THROW(Kyra::Interpreter::execute("var x: Number; {var x: Number;}", false, true));

	CaptureStdout();
	Kyra::Interpreter::execute("var y: Number = 420; {print y;}");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "420\n");
}

TEST(InterpreterTest, Functions) {
	// Check Arity
	EXPECT_ANY_THROW(Kyra::Interpreter::execute("fun(a: Number, b: Number)->Number{return a + b;}();", false, true));
	EXPECT_NO_THROW(Kyra::Interpreter::execute("fun(a: Number, b: Number)->Number{return a + b;}(2, 2);", false, true));

	// Simple function exec
	CaptureStdout();
	Kyra::Interpreter::execute("fun(xy: String)->Nothing{print xy;}(\"Hi\");");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "Hi\n");

	// Closures
	CaptureStdout();
	Kyra::Interpreter::execute("var closure: Function()->Number= fun(xyz: "
							   "Number)->Function()->Number{return fun()->Number{return xyz;};}(10); print closure();");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "10\n");
}