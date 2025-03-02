# Tested classes

# MEMORY LEAK (HEAP) TESTS

### KENGINE::VATTRIBUTE

```
	{
		kengine::vattrib<float> a;
	}
```

- default construtor (ok)
- destructor (ok)

```
	{
		float data[] = {
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
		};

		kengine::vattrib<float> a = {
			data,
			16,
			4
		};
	}
```

- constructor with arguments call (ok)
- destructor (ok)

```
	kengine::vattrib<float> foo() {
		float data[] = {
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
		};

		kengine::vattrib<float> d = {
			data,
			16,
			4
		};

		return d;
	}

	{
		kengine::vattrib<float> a = foo();
	}
```

- constructor with arguments call (ok)
- destructor (ok)
- Move constructor (ok)
- destructor (ok)

```
	{
		float data[] = {
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
		};

		kengine::vattrib<float> a = {
			data,
			16,
			4
		};

		float data2[] = {
			1.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
		};

		kengine::vattrib<float> b = {
			data2,
			6,
			2
		};

		kengine::vattrib<float> c = b;
	}
```

- constructor with arguments call (ok)
- constructor with arguments call (ok)
- copy constructor call (ok)
- destructor (ok)
- destructor (ok)
- destructor (ok)

```
	void foo(kengine::vattrib<float> a) {
		...
	}

	{
		float data[] = {
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
		};

		kengine::vattrib<float> a = {
			data,
			16,
			4
		};

		foo(a);
	}
```

- constructor with arguments call (ok)
- copy constructor call (ok)
- destructor (ok)
- destructor (ok)

```
	{
		float data[] = {
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
		};

		kengine::vattrib<float> a = {
			data,
			16,
			4
		};

		float data1[] = {
			1.0f, 1.0f,
			1.0f, 1.0f,
		};

		kengine::vattrib<float> b = {
			data1,
			4,
			2
		};

		kengine::vattrib<float> c;

		c = b;
		c = a;
	}
```

- constructor with arguments call (ok)
- constructor with arguments call (ok)
- copy assignment call (ok)
- copy assignment call (ok)
- destructor (ok)
- destructor (ok)
- destructor (ok)

```
	{
		float data[] = {
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
		};

		kengine::vattrib<float> a = {
			data,
			16,
			4
		};

		float data1[] = {
			1.0f, 1.0f,
			1.0f, 1.0f,
		};

		kengine::vattrib<float> b = {
			data1,
			4,
			2
		};

		kengine::vattrib<float> c;

		c = std::move(b);
		c = std::move(a);
	}
```

- constructor with arguments call (ok)
- constructor with arguments call (ok)
- move assignment call (ok)
- move assignment call (ok)
- destructor (ok)
- destructor (ok)
- destructor (ok)


### KENGINE::MATRIX