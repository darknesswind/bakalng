#pragma once

// https://docs.microsoft.com/en-us/cpp/cpp/cpp-built-in-operators-precedence-and-associativity
class Operation
{
	template <uint32_t wide> struct Wide { static const uint32_t fwide = ((wide & 0b11) << 30); static_assert(wide <= 0b11); };
	template <uint32_t prec> struct Precedence { static const uint32_t fPrec = ((prec & 0b11111) << 25); static_assert(prec <= 0b11111); };
	template <typename TWide, typename TPrecedence>
	struct Prop
	{
		static const uint32_t Value = TWide::fwide | TPrecedence::fPrec;
	};
public:
	enum Type : uint32_t
	{
		None		= Prop<Wide<0>, Precedence<0>>::Value,

		Scope		= Prop<Wide<2>, Precedence<1>>::Value,	// ::

		SelMemberO	= Prop<Wide<1>, Precedence<2>>::Value,	// .
		SelMemberP	= Prop<Wide<2>, Precedence<2>>::Value,	// ->

		Compl		= Prop<Wide<1>, Precedence<3>>::Value,	// ~
		Not,												// !
		AddressOf,											// &Id
		Indirection,										// *Id

		Mul			= Prop<Wide<1>, Precedence<5>>::Value,	// *
		Div,												// /
		Mod,												// %

		Add			= Prop<Wide<1>, Precedence<6>>::Value,	// +
		Sub,												// -

		ShiftLeft	= Prop<Wide<2>, Precedence<7>>::Value,	// <<
		ShiftRight,											// >>

		LessThen	= Prop<Wide<1>, Precedence<8>>::Value,	// <
		GreaterThen,										// >
		LessEqual	= Prop<Wide<2>, Precedence<8>>::Value,	// <=
		GreaterEqual,										// >=

		Equal		= Prop<Wide<2>, Precedence<9>>::Value,	// ==
		NotEqual,											// !=

		And			= Prop<Wide<1>, Precedence<10>>::Value,	// &
		Xor			= Prop<Wide<1>, Precedence<11>>::Value,	// ^
		Or			= Prop<Wide<1>, Precedence<12>>::Value,	// |
		LogicAnd	= Prop<Wide<2>, Precedence<13>>::Value,	// &&
		LogicOr		= Prop<Wide<2>, Precedence<14>>::Value,	// ||

		Conditional = Prop<Wide<1>, Precedence<15>>::Value,	// ?:

		Assign		= Prop<Wide<1>, Precedence<16>>::Value,	// =
		AddAssign	= Prop<Wide<2>, Precedence<16>>::Value,	// +=
		SubAssign,	// -=
		MulAssign,	// *=
		DivAssign,	// /=
		ModAssign,	// %=
		AndAssign,	// &=
		OrAssign,	// |=
		XorAssign,	// ^=
		SHLAssign	= Prop<Wide<3>, Precedence<16>>::Value,	// <<=
		SHRAssign,	// >>=

		Comma		= Prop<Wide<1>, Precedence<18>>::Value,	// ,

		EndExpr		= Prop<Wide<1>, Precedence<31>>::Value,	// ;
	};

public:
	Operation() : m_type(None) {}
	Operation(Type type) : m_type(type) {}
	operator Type() const { return m_type; }
 	size_t size() const { return m_flags.fWide; }
	size_t precedence() const { return m_flags.fPrecedence; }

private:
	union
	{
		Type m_type;
		struct 
		{
			uint32_t reserved : 25;
			uint32_t fPrecedence : 5;
			uint32_t fWide : 2;
		} m_flags;
	};
};

