//
// Copyright (c) 2013-2016 Pavel Medvedev. All rights reserved.
//
// This file is part of v8pp (https://github.com/pmed/v8pp) project.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "v8pp/object.hpp"

#include "test.hpp"

void test_object()
{
	v8pp::context context;
	v8::Isolate* isolate = context.isolate();

	v8::HandleScope scope(isolate);

	v8::Local<v8::Object> obj = v8::Object::New(isolate);

	v8pp::set_option(isolate, obj, "a", 10);
	v8pp::set_option(isolate, obj, "b", true);
	v8pp::set_option(isolate, obj, "sub", v8::Object::New(isolate));
	v8pp::set_option(isolate, obj, "sub.x", "zzz");
	v8pp::set_const(isolate, obj, "pi", 3.1415926);

	int a;
	check("get obj.a", v8pp::get_option(isolate, obj, "a", a));
	check_eq("obj.a", a, 10);

	bool b;
	check("get obj.b", v8pp::get_option(isolate, obj, "b", b));
	check_eq("obj.b", b, true);

	std::string x;
	check("get obj.sub.x", v8pp::get_option(isolate, obj, "sub.x", x));
	check_eq("obj.sub.x", x, "zzz");

	double pi;
	check("get obj.pi", v8pp::get_option(isolate, obj, "pi", pi));
	check("obj.pi", abs(pi - 3.1415926) < 10e-6);

	v8::Local<v8::Object> sub;
	check("get obj.sub", v8pp::get_option(isolate, obj, "sub", sub)
		&& sub->IsObject());

	context.run_script("test = { test : function() { return 1; } }");
	obj = isolate->GetCurrentContext()->Global();
	v8::Local<v8::Function> fun;
	check("test.test", v8pp::get_option(isolate, obj, "test.test", fun)
		&& fun->IsFunction());
}
