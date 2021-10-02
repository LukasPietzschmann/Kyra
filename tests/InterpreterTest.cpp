#include <gtest/gtest.h>

#include "../src/LibSlg/Interpreter.hpp"

using ::testing::internal::CaptureStdout;
using ::testing::internal::GetCapturedStdout;

class InterpreterTest : public ::testing::Test {
protected:
	LibSlg::Interpreter* m_interpreter;

	void SetUp() override { m_interpreter = &LibSlg::Interpreter::getInstance(); }
	void TearDown() override { m_interpreter = nullptr; }
};

TEST_F(InterpreterTest, SimpleExpression) {
	CaptureStdout();
	m_interpreter->execute("print 1;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "1\n");
}

TEST_F(InterpreterTest, VarDeclAndAssignment) {
	CaptureStdout();
	m_interpreter->execute("var a: Number = 1; print a;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "1\n");

	CaptureStdout();
	m_interpreter->execute("var b: Number; print b;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "nothing\n");

	CaptureStdout();
	m_interpreter->execute("b = 1; print b;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "1\n");

	EXPECT_ANY_THROW(m_interpreter->execute("print notDeclared;", false, true));

	EXPECT_ANY_THROW(
			m_interpreter->execute("var alreadyDeclared: Number; var alreadyDeclared: Number = 1;", false, true));

	// immutable vals
	EXPECT_ANY_THROW(m_interpreter->execute("val immutable: Number = 1; immutable = 2;", false, true));
	EXPECT_ANY_THROW(m_interpreter->execute("val immutableTwo: Number; immutableTwo = 2;", false, true));
}

TEST_F(InterpreterTest, SimpleBooleanComparison) {
	CaptureStdout();
	m_interpreter->execute("print true == false;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");

	CaptureStdout();
	m_interpreter->execute("print true < false;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");

	CaptureStdout();
	m_interpreter->execute("print false < true;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");

	CaptureStdout();
	m_interpreter->execute("print true < true;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");

	CaptureStdout();
	m_interpreter->execute("print true <= true;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");
}

TEST_F(InterpreterTest, ArithmeticPrecedence) {
	CaptureStdout();
	m_interpreter->execute("print 1 + 2 * 3;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "7\n");

	CaptureStdout();
	m_interpreter->execute("print (1 + 2) * 3;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "9\n");

	CaptureStdout();
	m_interpreter->execute("print -(1 + 2) * 3;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "-9\n");
}

TEST_F(InterpreterTest, Comparisons) {
	CaptureStdout();
	m_interpreter->execute("print nothing == nothing;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");

	CaptureStdout();
	m_interpreter->execute("print nothing == !nothing;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");

	CaptureStdout();
	m_interpreter->execute("print 1 < 2;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");

	CaptureStdout();
	m_interpreter->execute("print 1 <= 1;");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");

	CaptureStdout();
	m_interpreter->execute("print !(1 <= 1);");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");

	CaptureStdout();
	m_interpreter->execute(R"(print "a" <= "a";)");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "true\n");

	CaptureStdout();
	m_interpreter->execute(R"(print "a" < "a";)");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "false\n");
}

TEST_F(InterpreterTest, StringConcat) {
	CaptureStdout();
	m_interpreter->execute(R"(print "first" + " " + "second";)");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "first second\n");

	CaptureStdout();
	m_interpreter->execute(R"(print "ten times: " + "lol" * 10;)");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "ten times: lollollollollollollollollollol\n");

	CaptureStdout();
	m_interpreter->execute(R"(print "zero times: " + "lol" * -10;)");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "zero times: \n");
}

TEST_F(InterpreterTest, BlockScope) {
	EXPECT_ANY_THROW(m_interpreter->execute("var z: Number; var z: String;", false, true));
	EXPECT_NO_THROW(m_interpreter->execute("var x: Number; {var x: Number;}", false, true));

	CaptureStdout();
	m_interpreter->execute("var y: Number = 420; {print y;}");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "420\n");
}

TEST_F(InterpreterTest, Functions) {
	// Check Arity
	EXPECT_ANY_THROW(m_interpreter->execute("fun(a: Number, b: Number)->Number{return a + b;}();", false, true));
	EXPECT_NO_THROW(m_interpreter->execute("fun(a: Number, b: Number)->Number{return a + b;}(2, 2);", false, true));

	// Simple function exec
	CaptureStdout();
	m_interpreter->execute("fun(a: String)->Nothing{print a;}(\"Hi\");");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "Hi\n");

	// Closures
	CaptureStdout();
	m_interpreter->execute("var closure: Function()->Number= fun(a: "
						   "Number)->Function()->Number{return fun()->Number{return a;};}(10); print closure();");
	EXPECT_STREQ(GetCapturedStdout().c_str(), "10\n");
}