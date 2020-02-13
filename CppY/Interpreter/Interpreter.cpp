#include "Interpreter.h"
#include "object.h"
#include "list.h"
#include "Comparisons.h"
#include <regex>
#include <string>
#include "_Exceptions.h"

namespace py
{

	bool ICommand::ParsePy(std::string const & line)
	{
		std::regex statement(R"(^(s*)(.+?)(\s*)$)");
		if (!std::regex_match(line, statement))
			return false;

		std::smatch matches;
		std::regex_search(line, matches, statement);

		Indentation = matches[1];
		CmdText = matches[2];
		SuffixSpace = matches[3];
		return ParsePy_inner(CmdText);
	}

	std::string ICommand::Translate()
	{
		return Indentation + Translate_inner() + SuffixSpace;
	}
	
	//////////////////////////////////////////////////

	bool SameLine::ParsePy_inner(std::string const &  line)
	{
		Line = line;
		return true;
	}
	std::string SameLine::Translate_inner() const
	{
		return Line;
	}

	bool TwoSidesOperator::ParsePy_inner(std::string const &  line)
	{
		auto parts = pyStr(line).split(PyOperator(), 2);
		if (parts.size() > 1)
		{
			if (parts[0] == "")
				__debugbreak();

			Children.push_back(_parser->Parse(pyStr(parts[0])));
			Children.push_back(_parser->Parse(pyStr(parts[1])));
			return true;
		}
		return false;
	}

	std::string TwoSidesOperator::Translate_inner() const
	{
		return Children[0]->Translate() + CppOperator() + Children[1]->Translate();
	}
	
	//////////////////////////////////////////////

	std::string ObjIdentity::CppOperator() const { NOT_IMPL; }
	std::string ObjIdentity::Translate_inner() const
	{
		return pyStr("is_same(") + Children[0]->Translate() + pyStr(",") + Children[1]->Translate() + pyStr(")");
	}

	std::string NgatedObjIdentity::CppOperator() const { NOT_IMPL; }
	std::string NgatedObjIdentity::Translate_inner() const
	{
		return pyStr("is_not(") + Children[0]->Translate() + pyStr(",") + Children[1]->Translate() + pyStr(")");
	}
	   
	bool NegationOP::ParsePy_inner(std::string const &  line)
	{
		std::regex statement(R"(not (.*))");
		if (!std::regex_match(line, statement))
			return false;

		std::smatch matches;
		std::regex_search(line, matches, statement);

		Children.push_back(_parser->Parse(matches[1]));
		return true;
	}

	std::string NegationOP::Translate_inner() const
	{
		return "! " + Children[0]->Translate();
	}

	//////////// Pre and post escaping: ///////////////
	std::string UUID()
	{
		std::string s(16, 0);
		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

		for (int i = 0; i < s.size(); ++i) {
			s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
		}

		return s;
	}

	
	///////////////////////////

	bool InnerScopeEscaperBase::ParsePy_inner(std::string const &  line)
	{
		std::regex statement(OuterExpressionRegex());
		if (!std::regex_match(line, statement))
			return false;

		std::smatch matches;
		std::regex_search(line, matches, statement);
		std::string innerExpression_str = matches[1];

		_UUID = UUID();
		std::string modifiedLine =
			std::string(line.begin(), line.begin() + matches.position(1))
			+ _UUID +
			std::string(line.begin() + matches.position(1) + matches.length(1), line.end());

		Children.push_back(_parser->Parse(modifiedLine));
		ParsePy_innerScope(innerExpression_str);

		return true;
	}
	
	std::string InnerScopeEscaperBase::Translate_inner() const
	{
		std::string outer = Children[0]->Translate();
		std::string inner = Children[1]->Translate();
		
		auto uuidIndex = outer.find(_UUID, 0);
		THROW_IF(std::string::npos == uuidIndex, "Parsing error: Where did the substring UUID go?!");
		outer.replace(uuidIndex, _UUID.size(), inner);

		return outer;
	}

	void InnerScopeEscaperBase::ParsePy_innerScope(std::string const& text) { Children.push_back(_parser->Parse(text)); }

	
	////////////////////////////////////////////////////////////

	bool Container_ImplicitConstruction_Base::ParsePy_inner(std::string const &  line)
	{
		std::regex statement("\\(" + ScopeOpenerRGX() + "\\)" + R"((.*))" + ScopeCloserRGX());
		if (!std::regex_match(line, statement))
			return false;

		std::smatch matches;
		std::regex_search(line, matches, statement);
		
		_scopeOpener = matches[1];
		std::string elements = matches[1];
		ProcessMatch(elements);

		return true;
	}

	void Container_ImplicitConstruction_Base::ProcessMatch(std::string const& match)
	{
		for (pyStr part : pyStr(match).split(","))
			elements.append(part.Clone());
	}

	std::string ListImplicitConstruction::Translate_inner() const
	{
		return pyStr("list({") + pyStr(",").join(elements) + pyStr("});");
	}

	std::string DictImplicitConstruction::Translate_inner() const
	{
		pyStr result = pyStr("dict({");
		for (auto elemObj : elements)
		{
			pyStr elem = (std::string)*elemObj;
			auto parts = elem.split(":");
			THROW_UNLESS(parts.size() == 2, "Can't construct dict value from " + std::to_string(parts.size()) + " parts");
			result += pyStr("{ {} , {} } , ").format(parts[0], parts[1]);
		}
		result += pyStr("});");
		return result;
	}

	std::string FunctionCall::Translate_inner() const
	{
		pyStr result = pyStr("");

		for (auto elemObj : elements)
		{
			pyStr elem = (std::string) * elemObj;
			auto parts = elem.split("=");
			THROW_UNLESS(parts.size() <= 2, "Can't construct dict value from " + std::to_string(parts.size()) + " parts");
			result += result ? pyStr(".A") : pyStr("ARGS");
			if (parts.size() == 2)
				result += pyStr("({} , {})").format(parts[0], parts[1]);
			else // single part:
				result += pyStr("({})").format(parts[0]);
		}
		
		return pyStr(_scopeOpener) + pyStr("(") + result  + pyStr(");");
	}


	bool EmptyDictImplicitConstruction::ParsePy_inner(std::string const &  line)
	{
		std::regex statement("\\{\\s*\\}");
		if (!std::regex_match(line, statement))
			return false;

		std::smatch matches;
		std::regex_search(line, matches, statement);

		return true;
	}

	std::string EmptyDictImplicitConstruction::Translate_inner() const
	{
		return "dict();";
	}

	std::string SetImplicitConstruction::Translate_inner() const
	{
		return pyStr("set({") + pyStr(",").join(elements) + pyStr("});");
	}

	std::string BytesImplicitConstruction::Translate_inner() const
	{
		return pyStr("bytes(\"") + pyStr(",").join(elements) + pyStr("\")");
	}

	//////////////////////////////////////////////////

	bool VariableUsage::ParsePy_inner(std::string const &  line)
	{
		std::regex varName(R"(^([a-zA-Z_]+[a-zA-Z0-9_]*)\W?)");
		bool isVar = std::regex_match(line, varName);

		if (isVar)
		{
			THROW_UNLESS(_parser->ScopeMD(), "Can't process Vars without scope info!");
			VarName = line;
			IsDefinedFirstTime = _parser->ScopeMD()->IsDefined(VarName);
			_parser->ScopeMD()->Add(VarName);
		}
		return isVar;
	}

	std::string VariableUsage::Translate_inner() const
	{
		return IsDefinedFirstTime ? "object " + VarName : VarName;
	}

	bool If_Statement::ParsePy_inner(std::string const &  line)
	{
		std::regex statement(R"(if\s+([\w\s]+)\:)");
		if (!std::regex_match(line, statement))
			return false;

		std::smatch matches;
		std::regex_search(line, matches, statement);

		Children.push_back(_parser->Parse(matches[1]));
		return true;
	}

	std::string If_Statement::Translate_inner() const
	{
		return pyStr("if ({})").format(Children[0]->Translate());
	}

	bool For_Statement::ParsePy_inner(std::string const &  line)
	{
		std::regex statement(R"(for\s+(.+)\sin\s+(.+)\:)");
		if (!std::regex_match(line, statement))
			return false;

		std::smatch matches;
		std::regex_search(line, matches, statement);

		Children.push_back(_parser->Parse(matches[1]));
		Children.push_back(_parser->Parse(matches[2]));

		return true;

	}

	std::string For_Statement::Translate_inner() const
	{
		return pyStr("for (auto {} : {})").format(Children[0]->Translate(), Children[1]->Translate());
	}

	bool While_Statement::ParsePy_inner(std::string const &  line)
	{
		std::regex statement(R"(while\s+(.+)\:)");
		if (!std::regex_match(line, statement))
			return false;

		std::smatch matches;
		std::regex_search(line, matches, statement);

		Children.push_back(_parser->Parse(matches[1]));

		return true;
	}

	std::string While_Statement::Translate_inner() const
	{
		return pyStr("while ({})").format(Children[0]->Translate());
	}

	//void InnerScope_FunctionCall_Escaper::ParsePy_innerScope(std::string const& text)
	//{
	//	// Surround with $ which is not used in python (https://docs.python.org/3/reference/lexical_analysis.html)
	//	// for invoking fucn. - ARGs creation:
	//	Children.push_back(_parser->Parse("$" + text + "$"));
	//}

	inline void StringLiterals_OuterScope_EscaperBase::ProcessMatch(std::string const& match)
	{
		elements.append(pyStr(match).Clone());
	}

	std::string StringLiterals_OuterScope_EscaperBase::Translate_inner() const
	{
		pyStr result;
		if (IsRealString())
			result += pyStr(R"(R"()");
		else
			result += pyStr(R"(")");

		result += elements ? pyStr("") : (pyStr)*elements.FetchByIdx(0);

		if (IsRealString())
			result += pyStr(")\"");
		else
			result += pyStr(R"(")");

		return result;
	}

	/////////////////
	VariablesMetaDataPtr PyParser::ScopeMD()
	{
		return ScopeMDs_Stack.front();
	}

	PyParser::PyParser()
	{
		ScopeMDs_Stack.push_back(VariablesMetaDataPtr(new VariablesMetaData()));
		Indintations.push_back(0);

		ParsingChain.push_back(std::make_shared<CommentLine_InnerScope							>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<TripleQuote_StringLiteral_InnerScope			>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<TripleDoubleQuote_StringLiteral_InnerScope		>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<SingleQuote_REAL_StringLiteral_InnerScope		>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<SingleQuote_StringLiteral_InnerScope			>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<DoubleQuote_REAL_StringLiteral_InnerScope		>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<DoubleQuote_StringLiteral_InnerScope			>((PyParser *)this));

		ParsingChain.push_back(std::make_shared<TripleQuote_StringLiteral_OuterScope		 			>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<TripleDoubleQuote_StringLiteral_OuterScope	 			>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<SingleQuote_REAL_StringLiteral_OuterScope	 			>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<SingleQuote_StringLiteral_OuterScope		 			>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<DoubleQuote_REAL_StringLiteral_OuterScope	 			>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<DoubleQuote_StringLiteral_OuterScope		 			>((PyParser *)this));

		ParsingChain.push_back(std::make_shared<InnerScope_CurlyBraces_Escaper 			>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<InnerScope_SquareBraces_Escaper			>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<InnerScope_FunctionCall_Escaper			>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<InnerScope_RoundBraces_Escaper 			>((PyParser *)this));

		ParsingChain.push_back(std::make_shared<ListImplicitConstruction					>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<DictImplicitConstruction					>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<EmptyDictImplicitConstruction				>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<SetImplicitConstruction						>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<BytesImplicitConstruction					>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<FunctionCall								>((PyParser *)this));

		ParsingChain.push_back(std::make_shared<Assignment									>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<Bool_AND_Statement							>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<Bool_OR_Statement							>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<Bitwise_AND_Statement						>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<Bitwise_OR_Statement						>((PyParser *)this));

		ParsingChain.push_back(std::make_shared<LT				  						>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<LTE				  						>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<GT				  						>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<GTE				  						>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<Equal			  						>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<NotEqual		  						>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<ObjIdentity		  						>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<NgatedObjIdentity 						>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<NegationOP		  						>((PyParser *)this));

		ParsingChain.push_back(std::make_shared<VariableUsage			  				>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<If_Statement			  				>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<For_Statement			  				>((PyParser *)this));
		ParsingChain.push_back(std::make_shared<While_Statement			  				>((PyParser *)this));
	}

	ICommandPtr PyParser::Parse(std::string const& origLine)
	{
		for (auto parserPtr : ParsingChain)
		{
			auto line = origLine;
			if (parserPtr->ParsePy(line))
			{
				return parserPtr->Clone();
			}
		}
		auto result = std::make_shared<SameLine>((PyParser*)this);
		auto line = origLine;
		result->ParsePy(line);
		return result;
	}
}


