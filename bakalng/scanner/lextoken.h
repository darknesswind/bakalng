#pragma once
#include "operation.h"

class LexToken
{
public:
	enum Type
	{
		tkUnknown,
		tkIdentifier,
		tkKeyword,
		tkOperation,
		tkString,
		tkComment,
		tkError,

		tkNumInt32,
		tkNumUInt32,
		tkNumInt64,
		tkNumUInt64,
		tkNumFloat,
		tkNumDouble,
	};
	struct Location
	{
		HString hSource;
		size_t row;
		size_t col;
		fpos_t pos;
	};
public:
	~LexToken() = default;
	LexToken(Operation op) : m_type(tkOperation), m_operation(op) { }
	LexToken(Errors err) : m_type(tkError), m_error(err) { }
	LexToken(HString hString, Type type = tkString) : m_type(type), m_hString(hString)
	{
		assert(type == tkString || type == tkComment);
	}
	LexToken(int32_t vsint) : m_type(tkNumInt32), m_int32(vsint) { }
	LexToken(uint32_t vuint) : m_type(tkNumUInt32), m_uint32(vuint) { }
	LexToken(int64_t vsint) : m_type(tkNumInt64), m_int64(vsint) { }
	LexToken(uint64_t vuint) : m_type(tkNumUInt64), m_uint64(vuint) { }
	LexToken(float f) : m_type(tkNumFloat), m_float32(f) { }
	LexToken(double dbl) : m_type(tkNumDouble), m_float64(dbl) { }

	Type type() const { return m_type; }
	const Location& location() const { return m_loc; }
	void setLocation(const Location& loc) { m_loc = loc; }

	int32_t int32() const { assert(m_type == tkNumInt32); return m_int32; }
	uint32_t uint32() const { assert(m_type == tkNumUInt32); return m_uint32; }
	int64_t int64() const { assert(m_type == tkNumInt64); return m_int64; }
	uint64_t uint64() const { assert(m_type == tkNumUInt64); return m_uint64; }
	float float32() const { assert(m_type == tkNumFloat); return m_float32; }
	double float64() const { assert(m_type == tkNumDouble); return m_float64; }

private:
	Type m_type{ tkUnknown };
	union
	{
		Operation m_operation;
		Errors m_error;
		HString m_hString;
		int32_t m_int32;
		uint32_t m_uint32;
		int64_t m_int64;
		uint64_t m_uint64;
		float m_float32;
		double m_float64;
	};

	Location m_loc;
};
typedef std::vector<LexToken> LexTokens;
