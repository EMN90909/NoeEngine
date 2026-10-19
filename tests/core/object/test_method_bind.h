// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_METHOD_BIND_H
#define TEST_METHOD_BIND_H

#include "core/object/class_db.h"

#include "tests/test_macros.h"

namespace TestMethodBind {

class MethodBindTester : public Object {
	GDCLASS(MethodBindTester, Object);

public:
	enum Test {
		TEST_METHOD,
		TEST_METHOD_ARGS,
		TEST_METHODC,
		TEST_METHODC_ARGS,
		TEST_METHODR,
		TEST_METHODR_ARGS,
		TEST_METHODRC,
		TEST_METHODRC_ARGS,
		TEST_METHOD_DEFARGS,
		TEST_METHOD_OBJECT_CAST,
		TEST_MAX
	};

	class ObjectSubclass : public Object {
	public:
		int value = 1;
	};

	int test_num = 0;

	bool test_valid[TEST_MAX];

	void test_method() {
		test_valid[TEST_METHOD] = true;
	}

	void test_method_args(int p_arg) {
		test_valid[TEST_METHOD_ARGS] = p_arg == test_num;
	}

	void test_methodc() {
		test_valid[TEST_METHODC] = true;
	}

	void test_methodc_args(int p_arg) {
		test_valid[TEST_METHODC_ARGS] = p_arg == test_num;
	}

	int test_methodr() {
		test_valid[TEST_METHODR] = true; //temporary
		return test_num;
	}

	int test_methodr_args(int p_arg) {
		test_valid[TEST_METHODR_ARGS] = true; //temporary
		return p_arg;
	}

	int test_methodrc() {
		test_valid[TEST_METHODRC] = true; //temporary
		return test_num;
	}

	int test_methodrc_args(int p_arg) {
		test_valid[TEST_METHODRC_ARGS] = true; //temporary
		return p_arg;
	}

	void test_method_default_args(int p_arg1, int p_arg2, int p_arg3, int p_arg4, int p_arg5) {
		test_valid[TEST_METHOD_DEFARGS] = p_arg1 == 1 && p_arg2 == 2 && p_arg3 == 3 && p_arg4 == 4 && p_arg5 == 5; //temporary
	}

	void test_method_object_cast(ObjectSubclass *p_object) {
		test_valid[TEST_METHOD_OBJECT_CAST] = p_object->value == 1;
	}

	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("test_method"), &MethodBindTester::test_method);
		ClassDB::bind_method(D_METHOD("test_method_args"), &MethodBindTester::test_method_args);
		ClassDB::bind_method(D_METHOD("test_methodc"), &MethodBindTester::test_methodc);
		ClassDB::bind_method(D_METHOD("test_methodc_args"), &MethodBindTester::test_methodc_args);
		ClassDB::bind_method(D_METHOD("test_methodr"), &MethodBindTester::test_methodr);
		ClassDB::bind_method(D_METHOD("test_methodr_args"), &MethodBindTester::test_methodr_args);
		ClassDB::bind_method(D_METHOD("test_methodrc"), &MethodBindTester::test_methodrc);
		ClassDB::bind_method(D_METHOD("test_methodrc_args"), &MethodBindTester::test_methodrc_args);
		ClassDB::bind_method(D_METHOD("test_method_default_args"), &MethodBindTester::test_method_default_args, DEFVAL(9) /* wrong on purpose */, DEFVAL(4), DEFVAL(5));
		ClassDB::bind_method(D_METHOD("test_method_object_cast", "object"), &MethodBindTester::test_method_object_cast);
	}

	virtual void run_tests() {
		for (int i = 0; i < TEST_MAX; i++) {
			test_valid[i] = false;
		}
		//regular
		test_num = Math::rand();
		call("test_method");
		test_num = Math::rand();
		call("test_method_args", test_num);
		test_num = Math::rand();
		call("test_methodc");
		test_num = Math::rand();
		call("test_methodc_args", test_num);
		//return
		test_num = Math::rand();
		test_valid[TEST_METHODR] = int(call("test_methodr")) == test_num && test_valid[TEST_METHODR];
		test_num = Math::rand();
		test_valid[TEST_METHODR_ARGS] = int(call("test_methodr_args", test_num)) == test_num && test_valid[TEST_METHODR_ARGS];
		test_num = Math::rand();
		test_valid[TEST_METHODRC] = int(call("test_methodrc")) == test_num && test_valid[TEST_METHODRC];
		test_num = Math::rand();
		test_valid[TEST_METHODRC_ARGS] = int(call("test_methodrc_args", test_num)) == test_num && test_valid[TEST_METHODRC_ARGS];

		call("test_method_default_args", 1, 2, 3, 4);

		ObjectSubclass *obj = memnew(ObjectSubclass);
		call("test_method_object_cast", obj);
		memdelete(obj);
	}
};

TEST_CASE("[MethodBind] check all method binds") {
	MethodBindTester *mbt = memnew(MethodBindTester);

	mbt->run_tests();

	CHECK(mbt->test_valid[MethodBindTester::TEST_METHOD]);
	CHECK(mbt->test_valid[MethodBindTester::TEST_METHOD_ARGS]);
	CHECK(mbt->test_valid[MethodBindTester::TEST_METHODC]);
	CHECK(mbt->test_valid[MethodBindTester::TEST_METHODC_ARGS]);
	CHECK(mbt->test_valid[MethodBindTester::TEST_METHODR]);
	CHECK(mbt->test_valid[MethodBindTester::TEST_METHODR_ARGS]);
	CHECK(mbt->test_valid[MethodBindTester::TEST_METHODRC]);
	CHECK(mbt->test_valid[MethodBindTester::TEST_METHODRC_ARGS]);
	CHECK(mbt->test_valid[MethodBindTester::TEST_METHOD_DEFARGS]);
	CHECK(mbt->test_valid[MethodBindTester::TEST_METHOD_OBJECT_CAST]);

	memdelete(mbt);
}
} // namespace TestMethodBind

#endif // TEST_METHOD_BIND_H
