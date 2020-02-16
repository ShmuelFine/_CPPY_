#include "Interpreter.h"
#include "object.h"
#include "list.h"
#include "Comparisons.h"
#include <regex>
#include <string>
#include "_Exceptions.h"

namespace py
{

	void ICommand::ParsePy(std::string const & line)
	{
		std::regex statement(R"(^(\s*)(.+?)(\s*)$)");
		if (!std::regex_match(line, statement))
		{
			Indentation = "";
			CmdText = "";
			SuffixSpace = "";
			return;
		}

		std::smatch matches;
		std::regex_search(line, matches, statement);

		Indentation = matches[1];
		CmdText = matches[2];
		SuffixSpace = matches[3];

		ParsePy_inner(CmdText);
	}

	std::string ICommand::Translate()
	{
		return Indentation + Translate_inner() + SuffixSpace;
	}
	
	std::set<std::string> ICommand::GetVariables() const
	{
		auto myVars = GetMyVariables();
		std::set<std::string> result(myVars.begin(), myVars.end());
		for (auto& child : Children)
		{
			auto toAdd = child->GetVariables();
			if (!toAdd.empty())
				result.insert(toAdd.begin(), toAdd.end());
		}
		return result;
	}

	//////////////////////////////////////////////////
	std::string GetVarialbeRegex()
	{
		return R"(^([a-zA-Z_]+[a-zA-Z0-9_]*)$)";
	}
	bool IsVarialbe(std::string const& line)
	{
		std::regex varName(GetVarialbeRegex());
		return std::regex_match(line, varName);
	}
	std::vector<std::string> ExtractVariables(std::string const& candidatesStr)
	{
		std::vector<std::string> result;
		auto varCandidates = pyStr(candidatesStr).split(R"(\,)");
		for (auto candidate : varCandidates)
		{
			if (IsVarialbe(candidate))
				result.push_back(candidate);
		}
		return result;
	}
	
	//////////////////////////////////////////////////

	bool SameLine::CanParse(std::string const& line) const
	{
		return true;
	}

	void SameLine::ParsePy_inner(std::string const &  line)
	{
		Line = line;
	}
	std::string SameLine::Translate_inner() const
	{
		return Line;
	}

	bool TwoSidesOperator::CanParse(std::string const& line) const
	{
		return pyStr(line).split(PyOperator(), 2).size() > 1;
	}

	void TwoSidesOperator::ParsePy_inner(std::string const &  line)
	{
		auto parts = pyStr(line).split(PyOperator(), 2);
		if (parts.size() > 1)
		{
			if (parts[0] == "")
				__debugbreak();

			Children.push_back(_parser->Parse(pyStr(parts[0])));
			Children.push_back(_parser->Parse(pyStr(parts[1])));
		}
	}

	std::string TwoSidesOperator::Translate_inner() const
	{
		return Children[0]->Translate() + CppOperator() + Children[1]->Translate();
	}
	
	//////////////////////////////////////////////
	std::vector<std::string> Assignment::GetMyVariables() const
	{
		return ExtractVariables(Children[0]->Translate_inner());
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
	   
	bool NegationOP::CanParse(std::string const& line) const
	{
		std::regex statement(R"(not (.*))");
		return std::regex_match(line, statement);
	}
	void NegationOP::ParsePy_inner(std::string const& line)
	{
		std::regex statement(R"(not (.*))");
		std::smatch matches;
		std::regex_search(line, matches, statement);

		Children.push_back(_parser->Parse(matches[1]));
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

		return "$"+s+"$";
	}

	
	///////////////////////////

	bool InnerScopeEscaperBase::CanParse(std::string const& line) const
	{
		std::regex statement(OuterExpressionRegex());
		return std::regex_match(line, statement);
	}
	
	void InnerScopeEscaperBase::ParsePy_inner(std::string const& line)
	{
		std::regex statement(OuterExpressionRegex());

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

	bool Container_ImplicitConstruction_Base::CanParse(std::string const& line) const
	{
		std::regex statement("(" + ScopeOpenerRGX() + ")" + R"((.*))" + ScopeCloserRGX());
		return std::regex_match(line, statement);
	}

	void Container_ImplicitConstruction_Base::ParsePy_inner(std::string const& line)
	{
		std::regex statement("(" + ScopeOpenerRGX() + ")" + R"((.*))" + ScopeCloserRGX());

		std::smatch matches;
		std::regex_search(line, matches, statement);
		
		_scopeOpener = matches[1];
		std::string elements = matches[2];
		ProcessMatch(elements);
	}

	void Container_ImplicitConstruction_Base::ProcessMatch(std::string const& match)
	{
		for (pyStr part : pyStr(match).split(","))
			elements.append(part.Clone());
	}

	std::string ListImplicitConstruction::Translate_inner() const
	{
		return pyStr("list({") + pyStr(",").join(elements) + pyStr("})");
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


	bool EmptyDictImplicitConstruction::CanParse(std::string const& line) const
	{
		std::regex statement("\\{\\s*\\}");
		return std::regex_match(line, statement);
	}
	void EmptyDictImplicitConstruction::ParsePy_inner(std::string const& line)
	{
		std::regex statement("\\{\\s*\\}");
		std::smatch matches;
		std::regex_search(line, matches, statement);
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

	////////////////////////////////////////////////////



	bool If_Statement::CanParse(std::string const& line) const
	{
		std::regex statement(R"(if\s+(.+)\:)");
		return std::regex_match(line, statement);
	}
	void If_Statement::ParsePy_inner(std::string const &  line)
	{
		std::regex statement(R"(if\s+(.+)\:)");
		std::smatch matches;
		std::regex_search(line, matches, statement);

		Children.push_back(_parser->Parse(matches[1]));
	}

	std::string If_Statement::Translate_inner() const
	{
		return pyStr("if ({})").format(Children[0]->Translate());
	}

	
	bool For_Statement::CanParse(std::string const& line) const
	{
		std::regex statement(R"(for\s+(.+)\sin\s+(.+)\:)");
		return std::regex_match(line, statement);
	}
	void For_Statement::ParsePy_inner(std::string const &  line)
	{
		std::regex statement(R"(for\s+(.+)\sin\s+(.+)\:)");
		std::smatch matches;
		std::regex_search(line, matches, statement);

		Children.push_back(_parser->Parse(matches[1]));
		Children.push_back(_parser->Parse(matches[2]));
	}
	std::string For_Statement::Translate_inner() const
	{
		return pyStr("for (auto {} : {})").format(Children[0]->Translate(), Children[1]->Translate());
	}
	std::vector<std::string> For_Statement::GetMyVariables() const
	{
		return ExtractVariables(Children[0]->Translate_inner());
	}


	bool While_Statement::CanParse(std::string const& line) const
	{
		std::regex statement(R"(while\s+(.+)\:)");
		return std::regex_match(line, statement);
	}
	void While_Statement::ParsePy_inner(std::string const &  line)
	{
		std::regex statement(R"(while\s+(.+)\:)");
		std::smatch matches;
		std::regex_search(line, matches, statement);

		Children.push_back(_parser->Parse(matches[1]));
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

		result += elements ? (pyStr)*elements.FetchByIdx(0) : pyStr("") ;

		if (IsRealString())
			result += pyStr(")\"");
		else
			result += pyStr(R"(")");

		return result;
	}

	/////////////////

	bool Else_Statement::CanParse(std::string const& line) const { return pyStr(line).strip() == pyStr("else:"); }
	void Else_Statement::ParsePy_inner(std::string const& line) { return ; }
	std::string Else_Statement::Translate_inner() const { return "else"; }

	/////////////////

	PyLineParser::PyLineParser()
	{
		ParsingChain.push_back(std::make_shared<CommentLine_InnerScope							>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<TripleQuote_StringLiteral_InnerScope			>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<TripleDoubleQuote_StringLiteral_InnerScope		>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<SingleQuote_REAL_StringLiteral_InnerScope		>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<SingleQuote_StringLiteral_InnerScope			>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<DoubleQuote_REAL_StringLiteral_InnerScope		>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<DoubleQuote_StringLiteral_InnerScope			>((PyLineParser *)this));

		ParsingChain.push_back(std::make_shared<TripleQuote_StringLiteral_OuterScope		 	>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<TripleDoubleQuote_StringLiteral_OuterScope	 	>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<SingleQuote_REAL_StringLiteral_OuterScope	 	>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<SingleQuote_StringLiteral_OuterScope		 	>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<DoubleQuote_REAL_StringLiteral_OuterScope	 	>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<DoubleQuote_StringLiteral_OuterScope		 	>((PyLineParser *)this));

		ParsingChain.push_back(std::make_shared<InnerScope_CurlyBraces_Escaper 					>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<InnerScope_SquareBraces_Escaper					>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<InnerScope_FunctionCall_Escaper					>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<InnerScope_RoundBraces_Escaper 					>((PyLineParser *)this));

		ParsingChain.push_back(std::make_shared<ListImplicitConstruction						>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<DictImplicitConstruction						>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<EmptyDictImplicitConstruction					>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<SetImplicitConstruction							>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<BytesImplicitConstruction						>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<FunctionCall									>((PyLineParser *)this));

		ParsingChain.push_back(std::make_shared<If_Statement			  						>((PyLineParser*)this));
		ParsingChain.push_back(std::make_shared<Else_Statement			  						>((PyLineParser*)this));
		ParsingChain.push_back(std::make_shared<For_Statement			  						>((PyLineParser*)this));
		ParsingChain.push_back(std::make_shared<While_Statement			  						>((PyLineParser*)this));


		ParsingChain.push_back(std::make_shared<Assignment										>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<Bool_AND_Statement								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<Bool_OR_Statement								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<Bitwise_AND_Statement							>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<Bitwise_OR_Statement							>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<Addition										>((PyLineParser*)this));
		ParsingChain.push_back(std::make_shared<Substraction									>((PyLineParser*)this));
		ParsingChain.push_back(std::make_shared<Division										>((PyLineParser*)this));
		ParsingChain.push_back(std::make_shared<Multiplication									>((PyLineParser*)this));

		ParsingChain.push_back(std::make_shared<LT				  								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<LTE				  								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<GT				  								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<GTE				  								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<Equal			  								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<NotEqual		  								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<ObjIdentity		  								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<NgatedObjIdentity 								>((PyLineParser *)this));
		ParsingChain.push_back(std::make_shared<NegationOP		  								>((PyLineParser *)this));

		
		ParsingChain.push_back(std::make_shared<SameLine				  						>((PyLineParser *)this));
	}

	ICommandPtr PyLineParser::Parse(std::string const& origLine)
	{
		for (auto parserPtr : ParsingChain)
		{
			auto line = origLine;
			if (parserPtr->CanParse(line))
			{
				auto result = parserPtr->Clone();
				result->ParsePy(line);
				return result;
			}
		}

		throw std::runtime_error("Shouldn't get here... What happened to the SameLine parser?");
	}
}


